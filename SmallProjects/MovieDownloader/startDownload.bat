@echo off
start "" "C:\Program Files\NordVPN\NordVPN.exe"
timeout /t 22 >nul
start "" "C:\Program Files\qBittorrent\qbittorrent.exe" 
timeout /t 8 >nul
start "" python "%~dp0downloadNyaa.py"
exit
