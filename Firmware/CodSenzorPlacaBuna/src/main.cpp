#include <Arduino.h>
#include <Wire.h>             // For I2C communication
#include <Adafruit_GFX.h>     // Graphics library
#include <Adafruit_SSD1306.h> // OLED driver library
#include <DHT.h>              // DHT sensor library
#include <Adafruit_Sensor.h>  // Unified sensor library

// Define OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define DHT sensor pin and type
#define DHTPIN 15     // GPIO pin where the sensor's DATA pin is connected
#define DHTTYPE DHT22 // AM2301 is compatible with DHT22

// Create an instance of the SSD1306 display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Create an instance of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Start serial communication for debugging
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // 0x3C is the typical I2C address for the OLED
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ; // Stop execution
  }

  // Clear the display buffer
  display.clearDisplay();

  // Initialize the DHT sensor
  dht.begin();

  // Display initial message
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Reading temp & humidity...");
  display.display();
}

void loop()
{
  // Read temperature and humidity from the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if readings are valid
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Error reading sensor!");
    display.display();
    delay(2000);
    return;
  }

  // Display the temperature and humidity on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 30);
  display.print("Hum: ");
  display.print(humidity);
  display.println(" %");

  display.display();

  // Debug output to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Wait 2 seconds before updating the readings
  delay(2000);
}
