import { useState } from "react";
import "./App.css";

function App() {
  // Example state variables for sensor data and hatch control
  const [temperature, setTemperature] = useState<number | null>(null);
  const [humidity, setHumidity] = useState<number | null>(null);
  const [pressure, setPressure] = useState<number | null>(null);
  const [isRaining, setIsRaining] = useState<boolean | null>(null);
  const [hatchStatus, setHatchStatus] = useState<string>("Closed");

  // Example functions to simulate sensor updates and hatch control
  const handleOpenHatch = () => {
    setHatchStatus("Open");
    // Add logic to send the open command to the microcontroller
  };

  const handleCloseHatch = () => {
    setHatchStatus("Closed");
    // Add logic to send the close command to the microcontroller
  };

  return (
    <div className="App">
      <header className="header">
        <h1>Weather Sensor Dashboard</h1>
      </header>
      <main className="dashboard">
        <div className="card">
          <h2>Temperature</h2>
          <p>{temperature !== null ? `${temperature} °C` : "Unknown..."}</p>
        </div>
        <div className="card">
          <h2>Humidity</h2>
          <p>{humidity !== null ? `${humidity} %` : "Uknown..."}</p>
        </div>
        <div className="card">
          <h2>Pressure</h2>
          <p>{pressure !== null ? `${pressure} hPa` : "Unknown..."}</p>
        </div>
        <div className="card">
          <h2>Rain Status</h2>
          <p>
            {isRaining !== null
              ? isRaining
                ? "Raining"
                : "No Rain"
              : "Unknow..."}
          </p>
        </div>
        <div className="card">
          <h2>Hatch Status</h2>
          <p>{hatchStatus}</p>
        </div>
        <div className="controls">
          <button onClick={handleOpenHatch}>Open Hatch</button>
          <button onClick={handleCloseHatch}>Close Hatch</button>
        </div>
      </main>
    </div>
  );
}

export default App;
