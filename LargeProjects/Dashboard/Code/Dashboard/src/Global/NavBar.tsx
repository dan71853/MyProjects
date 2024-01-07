// NavBar.tsx
import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import './NavBar.css';
import { timerButtons } from '../Pages/TimerPage/TimerPageButtons';

interface MenuObject {
  [key: string]: (JSX.Element | null)[][];
}

export const handleButtonClick = (path: string, navigate: Function, setCurrentSet: Function) => {
  if (path.startsWith("/")) {
    navigate(path);
  } else {
    setCurrentSet(path);
  }
};

const NavBar: React.FC = () => {
  const navigate = useNavigate();
  const [currentSet, setCurrentSet] = useState("mainMain");

  const menus: MenuObject = {
    mainMain: [
      <button key="timer" onClick={() => handleButtonClick("/timer")}>Timer</button>,
      null,
      <button key="home" onClick={() => handleButtonClick("/")}>Home</button>,
      null,
      <button key="customButtonA" onClick={() => handleButtonClick("mainSub1")}>AAAAAAA</button>,
      <button key="customButtonB" onClick={() => console.log("BBBB")}>BBBBB</button>,
      null,
      null,
    ],
    mainSub1: [
      null,
      null,
      null,
      null,
      null,
      null,
      null,
      <button key="back1" onClick={() => handleButtonClick("mainMain")}>Back</button>,
    ],
    timerMain: [...timerButtons],
    // ... add other pages as needed
  };

  // const handleButtonClick = (path: string) => {
  //   if (path.startsWith("/")) {
  //     navigate(path);
  //   } else {
  //     setCurrentSet(path);
  //   }
  // };

  const buttons = menus[currentSet];

  return (
    <div className="navbar">
      {/* Map over the buttons array to render each button */}
      {buttons.map((button, index) => (
        <div className="navbar-button" key={index}>
          {button}
        </div>
      ))}
    </div>
  );
};

export default NavBar;
