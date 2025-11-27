#!/bin/bash
nohup qbittorrent &

sleep 2
source /home/dan/Documents/Git/MyProjects/SmallProjects/MovieDownloader/.venv/bin/activate
python3 /home/dan/Documents/Git/MyProjects/SmallProjects/MovieDownloader/downloadNyaa.py
