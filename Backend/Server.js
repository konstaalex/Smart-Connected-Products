const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
app.use(bodyParser.json());
app.use(cors());

// Store the latest sensor data
let latestData = { temperature: null, humidity: null };

// Endpoint to handle uplink data from TTN
app.post('/ttn-uplink', (req, res) => {
  try {
    const payload = req.body.uplink_message.frm_payload; // Base64-encoded payload
    const buffer = Buffer.from(payload, 'base64'); // Decode Base64 to bytes

    // Parse temperature and humidity from the payload
    // Assuming payload structure: [temperature, humidity]
    const temperature = buffer[0]; // First byte
    const humidity = buffer[1];    // Second byte

    // Update latest data
    latestData = { temperature, humidity };
    console.log(`Updated Data: Temperature = ${temperature}°C, Humidity = ${humidity}%`);

    res.status(200).send('Data received');
  } catch (err) {
    console.error('Error processing TTN uplink:', err);
    res.status(400).send('Invalid uplink data');
  }
});

// Endpoint to serve latest sensor data to the dashboard
app.get('/latest-data', (req, res) => {
  res.json(latestData);
});

// Start the server
const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Backend is running on http://localhost:${PORT}`);
});
