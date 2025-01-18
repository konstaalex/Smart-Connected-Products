#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>  // DHT sensor library



// DHT Sensor settings (AM2301A / DHT22)
#define DHTPIN 2         // GPIO2 or GPIO15
#define DHTTYPE DHT22    // AM2301A is DHT22
DHT dht(DHTPIN, DHTTYPE);

// Motor control pins
#define MOTOR_ENABLE 14  // Enable pin for PWM
#define MOTOR_IN1 27     // Control pin 1
#define MOTOR_IN2 26     // Control pin 2

// PWM settings
#define PWM_CHANNEL 0
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8  // 8-bit resolution (0-255)


// LoRaWAN AppEUI, DevEUI, and AppKey
static const u1_t PROGMEM APPEUI[8] = {0x34, 0x12, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
static const u1_t PROGMEM DEVEUI[8] = {0xD2, 0xC6, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70};
static const u1_t PROGMEM APPKEY[16] = {0xCA, 0x42, 0x45, 0xF7, 0x17, 0xD4, 0x98, 0xD5, 0x79, 0x00, 0xD9, 0x4A, 0x22, 0xCF, 0x2B, 0x4C};


void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }


// Pin mapping for LMIC
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 23,
    .dio = {26, 33, 32},
};


osjob_t sendjob;
const unsigned TX_INTERVAL = 60;  // Interval for sending data (in seconds)


// Global motor state
bool motorRanForward = false;  // Tracks if the motor has already run forward


// Forward declarations
float readTemperature();
float readHumidity();
void stopMotor();
void runMotorForward();
void runMotorReverse();


// Function to initialize motor control
void setupMotor() {

  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  // Setup PWM for the motor
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_ENABLE, PWM_CHANNEL);

  // Ensure motor starts off
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  ledcWrite(PWM_CHANNEL, 0);

}


// Function to stop the motor
void stopMotor() {

  Serial.println("Motor stopped.");

  digitalWrite(MOTOR_IN1, LOW);

  digitalWrite(MOTOR_IN2, LOW);

  ledcWrite(PWM_CHANNEL, 0);  // Stop PWM

}


// Function to run the motor forward
void runMotorForward() {

  Serial.println("Motor running forward for 5 seconds...");

  digitalWrite(MOTOR_IN1, HIGH);

  digitalWrite(MOTOR_IN2, LOW);

  ledcWrite(PWM_CHANNEL, 255);  // Full speed

  delay(5000);  // Run for 5 seconds

  stopMotor();

}


// Function to run the motor backward
void runMotorReverse() {

  Serial.println("Motor running reverse for 5 seconds...");

  digitalWrite(MOTOR_IN1, LOW);

  digitalWrite(MOTOR_IN2, HIGH);

  ledcWrite(PWM_CHANNEL, 255);  // Full speed

  delay(5000);  // Run for 5 seconds

  stopMotor();

}


// Function to read temperature from the DHT sensor
float readTemperature() {

  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Failed to read temperature from sensor.");
    return -1;
  }

  return temp;
}


// Function to read humidity from the DHT sensor
float readHumidity() {

  float hum = dht.readHumidity();

  if (isnan(hum)) {
    Serial.println("Failed to read humidity from sensor.");
    return -1;
  }

  return hum;
}


// Function to control the motor based on humidity
void controlMotor(float humidity) {

  if (humidity > 90.0 && !motorRanForward) {

    runMotorForward();
    motorRanForward = true;  // Mark motor as having run forward

  } else if (humidity < 90.0 && motorRanForward) {

    runMotorReverse();
    motorRanForward = false;  // Reset forward state

  }
}


// Function to send data via LoRaWAN
void do_send(osjob_t *j) {

  float temperature = readTemperature();
  float humidity = readHumidity();

  if (temperature != -1 && humidity != -1) {

    // Print data to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Control the motor based on humidity
    controlMotor(humidity);

    // Prepare payload for LoRaWAN
    uint8_t payload[4];
    int tempInt = (int)(temperature * 100);
    uint16_t humInt = (uint16_t)(humidity * 100);

    payload[0] = (tempInt >> 8) & 0xFF;
    payload[1] = tempInt & 0xFF;
    payload[2] = (humInt >> 8) & 0xFF;
    payload[3] = humInt & 0xFF;

    LMIC_setTxData2(1, payload, sizeof(payload), 0);
    Serial.println("Packet queued.");

  } else {
    Serial.println("Failed to read from DHT sensor.");
  }
  os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
}


void onEvent(ev_t ev) {

  if (ev == EV_TXCOMPLETE) {

    Serial.println("LoRaWAN transmission complete.");
    os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);

  }

}


// Function to initialize the DHT sensor and motor
void setup() {

  Serial.begin(115200);
  dht.begin();

  setupMotor();

  Serial.println("Starting...");

  SPI.begin(5, 19, 27, lmic_pins.nss);
  os_init_ex((const void *)&lmic_pins);
  LMIC_reset();

  do_send(&sendjob);
}


void loop() {

  os_runloop_once();
  
}