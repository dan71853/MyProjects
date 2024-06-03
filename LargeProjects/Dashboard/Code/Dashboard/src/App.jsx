// App.tsx
import React from 'react';
import { BrowserRouter as Router, Route, Routes  } from 'react-router-dom'; // Import Routes

import './App.css'; // Import global styles
// import NavBar from './Global/NavBar';
import EpisodeTracker from './Pages/EpisodeTracker/EpisodeTracker';
import MainPage from './Pages/MainPage/MainPage';
import TimerPage from './Pages/TimerPage/TimerPage';

import NordVPNControlProps from "./Pages/EpisodeTracker/VPNConnect"

function App() {
  return (
    <Router>
      <div className="app-container">
        <Routes >
          <Route path="/index.html" element={<EpisodeTracker/>} />
          <Route path="/" element={<MainPage />} />          
          <Route path="/timer" element={<TimerPage />} />
        </Routes>
        <NordVPNControlProps/>
      </div>
    </Router>
  );
}

export default App;