@echo off
start "" "C:\Program Files\NordVPN\NordVPN.exe"
timeout /t 18 >nul
start "" "C:\Program Files\qBittorrent\qbittorrent.exe" 
timeout /t 5 >nul
start "" python "%~dp0downloadNyaa.py"
exit
