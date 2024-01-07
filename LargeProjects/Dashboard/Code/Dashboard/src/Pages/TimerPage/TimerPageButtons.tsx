// TimerPageButtons.tsx
import React from 'react';

import { handleButtonClick } from '../../Global/NavBar';

export const timerButtons: (JSX.Element | null)[][] = [
  [
    <button key="timer" onClick={() => handleButtonClick("/timer")}>Timer</button>,
    null,
    <button key="home" onClick={() => handleButtonClick("/")}>Home</button>,
    null,
    <button key="customButtonC" onClick={() => handleButtonClick("timerCum1")}>AAAAAAA</button>,
    <button key="customButtonB" onClick={() => console.log("BBBB")}>BBBBB</button>,
    null,
    null,
  ],
  [
    null,
    null,
    null,
    null,
    null,
    null,
    null,
    <button key="back1" onClick={() => handleButtonClick("timerMain")}>Back</button>,
  ],
];
