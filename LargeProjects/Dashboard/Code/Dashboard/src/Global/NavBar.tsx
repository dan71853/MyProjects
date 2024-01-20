// NavBar.tsx
import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import './NavBar.css';

import {useEvent, useList} from 'react-use';


const NavBar: React.FC = () => {
  const [currentSet, setCurrentSet] = useState("mainMain");
  const navigate = useNavigate();

  const handleButtonClick = (path: string) => {
    if (path.startsWith("/")) {
      navigate(path);
    } else {
      setCurrentSet(path);
    }
  };


  const menus: { [key: string]: Array<React.ReactNode | null> } = {
    mainMain: [
      <NavBarButton displayText='Main menu' path='/' onClick={() => handleButtonClick('/')} />,
      null,
      <NavBarButton displayText='Timer page' path='/timer' onClick={() => handleButtonClick('/timer')} />,
      null,
      <NavBarButton displayText='mainSub1' path='mainSub1' onClick={() => handleButtonClick('mainSub1')} />,
      <NavBarButton displayText='mainSub2' path='mainSub2' onClick={() => handleButtonClick('mainSub2')} />,
      null,
      <NavBarButton displayText='Increment counter' path='mainMain' onClick={() => {console.log("AAAAA")}} />,
    ],
    mainSub1: [
      null,
      null,
      null,
      null,
      null,
      null,
      null,
      <NavBarButton displayText='Back' path='mainMain' onClick={() => handleButtonClick('mainMain')} />,
    ],
  };

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

export const NavBarButton: React.FC<{ path?: string; displayText?: string; onClick?: () => void }> = ({ path, displayText, onClick }) => {
  if (path) {
    return (
      <button onClick={onClick}>{displayText}</button>
    );
  } else {
    return (null);
  }
};

export default NavBar;
