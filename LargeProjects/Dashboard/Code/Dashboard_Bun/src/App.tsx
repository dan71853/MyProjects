// App.tsx
import React from 'react';
import { BrowserRouter as Router, Route, Routes  } from 'react-router-dom'; // Import Routes

import './App.css'; // Import global styles
import NavBar from './Global/NavBar';
import MainPage from './Pages/MainPage/MainPage';
import TimerPage from './Pages/TimerPage/TimerPage';

function App() {
  return (
    <Router>
      <div className="app-container">
        <Routes >
          <Route path="/index.html" element={<MainPage/>} />
          <Route path="/" element={<MainPage />} />          
          <Route path="/timer" element={<TimerPage />} />
        </Routes>
        <NavBar />
      </div>
    </Router>
  );
}

export default App;