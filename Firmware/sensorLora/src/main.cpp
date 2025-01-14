#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>  // DHT sensor library
 
// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT Sensor settings (AM2301A / DHT22)
#define DHTPIN 2         // GPIO2 or GPIO15
#define DHTTYPE DHT22    // AM2301A is DHT22
DHT dht(DHTPIN, DHTTYPE);

// LoRaWAN AppEUI, DevEUI, and AppKey (replace with real values)
static const u1_t PROGMEM APPEUI[8] = {0x34, 0x12, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
static const u1_t PROGMEM DEVEUI[8] = {0xD2, 0xC6, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
static const u1_t PROGMEM APPKEY[16] = {0xCA, 0x42, 0x45, 0xF7, 0x17, 0xD4, 0x98, 0xD5, 0x79, 0x00, 0xD9, 0x4A, 0x22, 0xCF, 0x2B, 0x4C};
void os_getDevKey(u1_t *buf) {
  memcpy_P(buf, APPKEY, 16);  // Copy APPKEY to the provided buffer
}

void os_getArtEui(u1_t *buf) {
  memcpy_P(buf, APPEUI, 8);  // Copy APPEUI to the provided buffer
}

void os_getDevEui(u1_t *buf) {
  memcpy_P(buf, DEVEUI, 8);  // Copy DEVEUI to the provided buffer
}

// Pin mapping for LMIC
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26, 33, 32},
};

osjob_t sendjob;  // Job for sending data
const unsigned TX_INTERVAL = 60;  // Interval for sending data (in seconds)

// Function to display sensor data on OLED
void displaySensorData(float temperature, float humidity) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.print(F("Temp: "));
  display.print(temperature);
  display.println(F(" C"));

  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(F(" %"));

  display.display();
}

// Function to read data from DHT sensor
float readTemperature() {
  float temp = dht.readTemperature();  // Read temperature in Celsius
  if (isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor"));
    return -1;
  }
  return temp;
}

float readHumidity() {
  float hum = dht.readHumidity();  // Read humidity
  if (isnan(hum)) {
    Serial.println(F("Failed to read from DHT sensor"));
    return -1;
  }
  return hum;
}

// Function to send data via LoRaWAN
void do_send(osjob_t *j) {
  // Read temperature and humidity
  float temperature = readTemperature();
  float humidity = readHumidity();

  if (temperature != -1 && humidity != -1) {
    // Prepare payload
    uint8_t payload[4];
    int tempInt = (int)(temperature * 100);  // Convert to integer (centi-degrees)
    uint16_t humInt = (uint16_t)(humidity * 100);  // Convert to integer (humidity in percentage)

    payload[0] = (tempInt >> 8) & 0xFF;  // High byte of temperature
    payload[1] = tempInt & 0xFF;         // Low byte of temperature
    payload[2] = (humInt >> 8) & 0xFF;   // High byte of humidity
    payload[3] = humInt & 0xFF;          // Low byte of humidity

    // Display data on OLED screen
    displaySensorData(temperature, humidity);

    // Send payload via LoRaWAN
    LMIC_setTxData2(1, payload, sizeof(payload), 0);
    Serial.println(F("Packet queued"));
  }

  // Schedule next transmission after TX_COMPLETE
  os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
}

void onEvent(ev_t ev) {
  // Event handling code
  switch (ev) {
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE"));
      if (LMIC.txrxFlags & TXRX_ACK) {
        Serial.println(F("Received ack"));
      }
      if (LMIC.dataLen) {
        Serial.print(F("Received "));
        Serial.print(LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
      }
      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      break;
    default:
      break;
  }
}

// Initialize display and begin serial communication
void runDisplay() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 5);
  display.println(F("Initializing..."));
  display.display();
}

void setup() {
  runDisplay();
  dht.begin();  // Initialize the DHT sensor

  Serial.begin(9600);
  Serial.println(F("Starting"));

  SPI.begin(5, 19, 27, lmic_pins.nss);

  // LMIC init
  os_init_ex((const void *)&lmic_pins);
  LMIC_reset();

  // Start sending data (OTAA join + data transmission)
  do_send(&sendjob);
}

void loop() {
  os_runloop_once();
}