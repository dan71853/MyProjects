## Setup

- `python3 -m venv .venv`
- `source .venv/bin/activate.fish`
- `pip install -r requirements.txt`


## Auto Startup

- `sudo nano /etc/systemd/user/python-dashboard.service`
 
```
[Unit]
Description=Dashboard Python Service

[Service]
ExecStart=/usr/bin/python3 /home/dan/Projects/MyRepos/MyProjects/SmallProjects/PyQt_Dashboard/main.py
WorkingDirectory=/home/dan/Projects/MyRepos/MyProjects/SmallProjects/PyQt_Dashboard/
Restart=always

Environment=DISPLAY=:0
Environment=XAUTHORITY=/home/dan/.Xauthority

[Install]
WantedBy=default.target
```

enable with
``` 
systemctl --user daemon-reload
systemctl --user enable python-dashboard.service
systemctl --user start python-dashboard.service
```

check with
`systemctl --user status python-dashboard.service`


## Force Location Wayland 
Import the `./kwin-riles-backup` file as a kwin rule