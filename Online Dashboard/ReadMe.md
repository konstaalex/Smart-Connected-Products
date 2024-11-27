# Weather Sensor Dashboard

This project is a modern, responsive web application for monitoring and controlling a weather-based hatch system. The application uses TTGO LILYGO microcontroller sensors and actuators, LoRaWAN, and The Things Network (TTN) for data communication, and integrates with a database to store historical sensor data and device logs. 

## Features

- **Live Weather Data**: Displays temperature, humidity, pressure, and rain status from the TTGO microcontroller.
- **Hatch Control**: Open and close the hatch directly from the dashboard.
- **Hatch Status Monitoring**: Real-time feedback on the current hatch state.
- **Responsive Design**: Optimized for desktop and mobile devices.
- **Data Persistence**: Stores sensor data and logs in a database for analysis and troubleshooting.

---

## Technologies Used

### Frontend
- **React + Vite**: React for building the user interface and Vite for fast bundling and development.
- **CSS**: Custom CSS for a clean and modern dashboard design.

### Backend
- **Node.js with Express**: Handles API requests, communicates with TTN, and manages interactions with the database.

### Connectivity
- **LoRaWAN**: Low-power, wide-area network (LPWAN) protocol used for transmitting sensor data.
- **The Things Network (TTN)**: Manages LoRaWAN communication, acting as the middleware between the microcontroller and the backend.

### Database
- **Firebase Realtime Database**: Provides an easy-to-implement, scalable, and real-time database solution. It stores historical sensor data and device logs for analysis.

### Microcontroller
- **TTGO LILYGO T-Beam**: Equipped with sensors and an actuator, connects to LoRaWAN for transmitting weather data and receiving hatch control commands.

---

## System Architecture

1. **Microcontroller**: TTGO collects weather data (temperature, humidity, pressure, rain status) and sends it to TTN using LoRaWAN. It also receives commands (open/close hatch) from TTN.
2. **The Things Network (TTN)**: Routes the data from the TTGO to the backend API and forwards commands from the dashboard to the microcontroller.
3. **Backend**:
   - Receives data from TTN via a webhook.
   - Stores data in Firebase for persistence.
   - Processes requests from the frontend to control the hatch via TTN.
4. **Frontend**:
   - Fetches real-time data from the backend API.
   - Provides an interactive dashboard to monitor and control the system.

---

## Installation

### Prerequisites
- Node.js installed on your system.
- Firebase account setup.
- TTN account and application setup for LoRaWAN communication.

### Steps
1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/weather-sensor-dashboard.git
   cd weather-sensor-dashboard
