#!/bin/bash
nohup qbittorrent &

sleep 2
source /home/dan/Projects/MyRepos/MyProjects/SmallProjects/MovieDownloader/.venv/bin/activate
python3 /home/dan/Projects/MyRepos/MyProjects/SmallProjects/MovieDownloader/downloadNyaa.py
