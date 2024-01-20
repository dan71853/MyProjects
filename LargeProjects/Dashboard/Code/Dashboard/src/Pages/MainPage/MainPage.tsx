import React, { useState, useEffect } from 'react';
import si from 'systeminformation';

import './MainPage.css';

const MainPage: React.FC = () => {
  const [cpuTemperature, setCpuTemperature] = useState<number | null>(null);
  const [gpuTemperature, setGpuTemperature] = useState<number | null>(null);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const getTemperatures = async () => {
      try {
        // const cpuTempInfo = await si.cpuTemperature();
        // const gpuTempInfo = await si.graphicsTemperature();

        // setCpuTemperature(cpuTempInfo.main);
        // setGpuTemperature(gpuTempInfo.main);
      } catch (error) {
        console.log("error",error)
        setError(error.message);
      }
    };

    getTemperatures();


    // Refresh temperatures every 5 seconds (adjust as needed)
    const intervalId = setInterval(() => {
      getTemperatures();
    }, 5000);

    return () => clearInterval(intervalId); // Cleanup on component unmount
  }, []);

  return (
    <div className="main-page">
      <h1>Main Page</h1>
      
      {error ? (
        <p>Error fetching temperatures: {error}</p>
      ) : (
        <div>
          <p>CPU Temperature: {cpuTemperature !== null ? cpuTemperature.toFixed(2) : 'Loading...'} °C</p>
          <p>GPU Temperature: {gpuTemperature !== null ? gpuTemperature.toFixed(2) : 'Loading...'} °C</p>
        </div>
      )}
    </div>
  );
};

export default MainPage;
