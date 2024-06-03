// NordVPNControl.tsx

import React, { useState } from 'react';

interface NordVPNControlProps {}

const NordVPNControl: React.FC<NordVPNControlProps> = () => {
  const [status, setStatus] = useState<string>('');

  const executeNordVPNCommand = (command: string, args: string[]) => {
    // Check if running in Electron environment
    if (window.require) {
      const electron = window.require('electron');
      if (electron) {
        const ipcRenderer = electron.ipcRenderer;
        if (ipcRenderer) {
          // Send a message to the main process to execute the command
          ipcRenderer.send('execute-command', { command, args });

          // Handle the response from the main process
          ipcRenderer.on('command-executed', (event, response) => {
            const { stdout, stderr, code } = response;
            console.log(`stdout: ${stdout}`);
            console.error(`stderr: ${stderr}`);
            console.log(`child process exited with code ${code}`);
            setStatus(`stdout: ${stdout}\nstderr: ${stderr}\nchild process exited with code ${code}`);
          });
        } else {
          console.error('ipcRenderer is undefined');
        }
      } else {
        console.error('Electron is not available');
      }
    } else {
      // Handle non-electron environment (e.g., Vite development server)
      console.error('Cannot execute child_process in the browser environment.');
    }
  };

  const handleConnect = () => {
    executeNordVPNCommand('nordvpn', ['connect']);
  };

  const handleDisconnect = () => {
    executeNordVPNCommand('nordvpn', ['disconnect']);
  };

  return (
    <div>
      <h2>NordVPN Control</h2>
      <button onClick={handleConnect}>Connect to NordVPN</button>
      <button onClick={handleDisconnect}>Disconnect from NordVPN</button>
      <div>Status: {status}</div>
    </div>
  );
};

export default NordVPNControl;
