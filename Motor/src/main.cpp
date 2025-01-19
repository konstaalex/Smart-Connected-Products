#include <Arduino.h>

// Motor Driver Pins
#define ENABLE_A 17 // Connected to ENABLE A pin on the driver
#define IN1 16      // Connected to IN1 on the driver
#define IN2 4       // Connected to IN2 on the driver

void setup()
{
  // Initialize serial for debugging
  Serial.begin(9600);
  Serial.println("Motor Control Starting...");

  // Set motor control pins as outputs
  pinMode(ENABLE_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Ensure motor is stopped initially
  digitalWrite(ENABLE_A, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop()
{
  // 1. Run Motor Forward for 10 Seconds
  Serial.println("Running Motor Forward...");
  digitalWrite(ENABLE_A, HIGH); // Enable motor
  digitalWrite(IN1, HIGH);      // IN1 HIGH
  digitalWrite(IN2, LOW);       // IN2 LOW
  delay(10000);                 // Run for 10 seconds

  // 2. Stop Motor for 10 Seconds
  Serial.println("Stopping Motor...");
  digitalWrite(ENABLE_A, LOW); // Disable motor
  digitalWrite(IN1, LOW);      // IN1 LOW
  digitalWrite(IN2, LOW);      // IN2 LOW
  delay(10000);                // Wait for 10 seconds

  // 3. Run Motor Backward for 10 Seconds
  Serial.println("Running Motor Backward...");
  digitalWrite(ENABLE_A, HIGH); // Enable motor
  digitalWrite(IN1, LOW);       // IN1 LOW
  digitalWrite(IN2, HIGH);      // IN2 HIGH
  delay(10000);                 // Run for 10 seconds

  // 4. Stop Motor for 10 Seconds
  Serial.println("Stopping Motor...");
  digitalWrite(ENABLE_A, LOW); // Disable motor
  digitalWrite(IN1, LOW);      // IN1 LOW
  digitalWrite(IN2, LOW);      // IN2 LOW
  delay(10000);                // Wait for 10 seconds
}
