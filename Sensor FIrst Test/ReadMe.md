# TTGO LoRa32 V1 - Temperature and Humidity Display

This project uses a **TTGO LoRa32 V1** microcontroller to read temperature and humidity data from an **ASAIR AM2301 (DHT22)** sensor and display it on the built-in TFT screen. The menu system allows users to interact using the onboard buttons.

---

## Hardware

### Microcontroller
- **TTGO LoRa32 V1**

### Sensor
- **ASAIR AM2301 (DHT22)**

### Connections
| **Sensor Wire** | **TTGO Pin**   |
|------------------|----------------|
| VCC (Red)        | 3.3V           |
| GND (Black)      | GND            |
| DATA (Yellow)    | GPIO 36        |

---

## Software

### Libraries Used
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) (for TFT screen)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) (for AM2301 sensor)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)

### Platform
- Developed using **VSCode with PlatformIO**. Configuration is provided in `platformio.ini`.

---

## Usage

1. **Hardware Setup**:
   - Connect the DHT22 sensor to the TTGO board as shown in the "Connections" table.
   - Power the board via USB.

2. **Software Setup**:
   - Clone the repository and open it in PlatformIO.
   - Install the required libraries using the `platformio.ini` configuration.

3. **Upload Code**:
   - Compile and upload the code to the TTGO LoRa32 V1 board.

4. **Run**:
   - The TFT screen will display the temperature and humidity readings.

---

## Notes

- This is the **first version** of the project and was designed for the TTGO LoRa32 V1 board.
- Tested with the **ASAIR AM2301 (DHT22)** sensor.
