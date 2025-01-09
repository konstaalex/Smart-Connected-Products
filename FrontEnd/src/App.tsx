// import React, { useState, useEffect } from 'react';
import { useEffect, useState } from 'react';
import './App.css';

function App() {
  const [temperature, setTemperature] = useState<number | null>(null);
  const [humidity, setHumidity] = useState<number | null>(null);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await fetch('http://localhost:3000/latest-data'); // Replace with your backend API endpoint
        if (!response.ok) {
          throw new Error('Failed to fetch sensor data');
        }
        const data = await response.json();
        setTemperature(data.temperature);
        setHumidity(data.humidity);
      } catch (err: any) {
        setError(err.message);
      }
    };

    const interval = setInterval(fetchData, 5000); // Fetch every 5 seconds
    return () => clearInterval(interval); // Cleanup interval on unmount
  }, []);

  return (
    <div className="App">
      <header className="header">
        <h1>Sensor Dashboard</h1>
      </header>
      <div className="dashboard">
        <div className="card">
          <h2>Temperature</h2>
          <p>{temperature !== null ? `${temperature}°C` : 'Loading...'}</p>
        </div>
        <div className="card">
          <h2>Humidity</h2>
          <p>{humidity !== null ? `${humidity}%` : 'Loading...'}</p>
        </div>
      </div>
      {error && <p className="error">{error}</p>}
    </div>
  );
}

export default App;
