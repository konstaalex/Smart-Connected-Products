#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT sensor settings
#define DHTPIN 2      // GPIO2 for DHT sensor
#define DHTTYPE DHT22 // DHT22 sensor
DHT dht(DHTPIN, DHTTYPE);

// Button settings
#define BUTTON_PIN 14 // GPIO14 for the button
bool motorRunning = false;

// Motor settings
#define ENABLE_A 17   // Connected to ENABLE A pin on the driver
#define IN1 16        // Connected to IN1 on the driver
#define IN2 4         // Connected to IN2 on the driver
int motorSpeed = 255; // Initial motor speed (0-255)

// Variables
unsigned long lastActionTime = 0;
bool motorDirection = true; // true = forward, false = backward

// Function prototype for displayMotorStatus()
void displayMotorStatus();

void setup()
{
  // Serial setup
  Serial.begin(9600);
  Serial.println("Motor Control with OLED Display Starting...");

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // Initialize DHT sensor
  dht.begin();

  // Initialize button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize motor pins
  pinMode(ENABLE_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Ensure motor is stopped initially
  analogWrite(ENABLE_A, 0); // Motor speed = 0
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  displayMotorStatus();
}

void loop()
{
  // Read humidity and temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // If DHT sensor fails
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("DHT Error");
    display.display();
    return;
  }

  // Button press logic
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    delay(200);                   // Debounce delay
    motorRunning = !motorRunning; // Toggle motor state
    if (!motorRunning)
    {
      // Stop motor
      analogWrite(ENABLE_A, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }
    displayMotorStatus();
  }

  // Humidity trigger logic
  if (humidity > 60) // Example threshold
  {
    motorRunning = true;               // Start motor if humidity > 60%
    motorDirection = true;             // Run forward
    analogWrite(ENABLE_A, motorSpeed); // Set motor speed
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    displayMotorStatus();
  }

  // Motor operation
  if (motorRunning)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - lastActionTime > 10000) // 10 seconds
    {
      lastActionTime = currentMillis;
      motorDirection = !motorDirection; // Toggle direction

      if (motorDirection)
      {
        // Run forward
        analogWrite(ENABLE_A, motorSpeed); // Set motor speed
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
      }
      else
      {
        // Run backward
        analogWrite(ENABLE_A, motorSpeed); // Set motor speed
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
      }
      displayMotorStatus();
    }
  }

  delay(500); // Slow down loop for stability
}

// Function to display motor status on OLED
void displayMotorStatus()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  if (motorRunning)
  {
    display.println("Motor: RUNNING");
    display.print("Direction: ");
    display.println(motorDirection ? "FORWARD" : "BACKWARD");
    display.print("Speed: ");
    display.println(motorSpeed);
  }
  else
  {
    display.println("Motor: STOPPED");
  }
  display.print("Temp: ");
  display.print(dht.readTemperature());
  display.println(" C");
  display.print("Humidity: ");
  display.print(dht.readHumidity());
  display.println(" %");
  display.display();
}
