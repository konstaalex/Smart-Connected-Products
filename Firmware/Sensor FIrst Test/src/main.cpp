#include <Arduino.h>
#include <TFT_eSPI.h>
#include "DHT.h"

// Set up the display
TFT_eSPI tft = TFT_eSPI();

// Define DHT sensor pin and type
#define DHTPIN 36        // GPIO pin connected to the Data pin
#define DHTTYPE DHT22     // AM2301 is compatible with DHT22

DHT dht(DHTPIN, DHTTYPE);

// Define button GPIOs
const int buttonLeftPin = 0;   
const int buttonRightPin = 35; 

void displayMenu();
void handleButtonPress();
void displayTemperature();
// Define menu items
const char* menuItems[] = {"Scan Start", "Display Test", "Show Temp"};
int menuIndex = 0;

void setup() {
    // Initialize display
    Serial.begin(115200);  // Start Serial for debugging
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    
    // Initialize DHT sensor
    dht.begin();

    // Initialize buttons
    pinMode(buttonLeftPin, INPUT_PULLUP);
    pinMode(buttonRightPin, INPUT_PULLUP);

    // Display initial menu
    displayMenu();
}

void loop() {
    // Check button presses for navigation
    handleButtonPress();

    // If "Show Temp" is selected, display temperature and humidity
    if (menuIndex == 2) {
        displayTemperature();
    }

    delay(100);
}

// Function to handle button presses and navigate menu
void handleButtonPress() {
    if (digitalRead(buttonLeftPin) == LOW) { 
        menuIndex = (menuIndex == 0) ? 2 : menuIndex - 1;
        displayMenu();
        delay(200); // Debounce delay
    } else if (digitalRead(buttonRightPin) == LOW) { 
        menuIndex = (menuIndex + 1) % 3;
        displayMenu();
        delay(200); // Debounce delay
    }
}

// Function to display menu options with the selected option highlighted
void displayMenu() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Select Option:", 10, 10, 2);
    
    for (int i = 0; i < 3; i++) {
        if (i == menuIndex) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK); 
        } else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.drawString(menuItems[i], 10, 30 + i * 20, 2); 
    }
}

// Function to read and display temperature and humidity
void displayTemperature() {
    // Read temperature and humidity
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if readings are valid
    if (isnan(humidity) || isnan(temperature)) {
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("Error reading sensor", 10, 10, 2);
        return;
    }

    // Display temperature and humidity on the screen
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Temperature:", 10, 10, 2);
    tft.drawFloat(temperature, 1, 10, 30, 2);
    tft.drawString("C", 70, 30, 2);

    tft.drawString("Humidity:", 10, 50, 2);
    tft.drawFloat(humidity, 1, 10, 70, 2);
    tft.drawString("%", 70, 70, 2);
}
