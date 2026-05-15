## Setup

- `python3 -m venv .venv`
- `source .venv/bin/activate.fish`
- `pip install -r requirements.txt`


## Auto Startup

- `sudo nano /etc/systemd/system/python-dashboard.service`

```
[Unit]
Description=Dashboard Python Service
After=network.target

[Service]
Type=simple
ExecStart=/usr/bin/python3 /home/dan/Projects/MyRepos/MyProjects/SmallProjects/PyQt_Dashboard/main.py
WorkingDirectory=/home/dan/Projects/MyRepos/MyProjects/SmallProjects/PyQt_Dashboard/
Restart=always
User=dan
Group=users

[Install]
WantedBy=multi-user.target
```


```
sudo systemctl daemon-reexec
sudo systemctl daemon-reload
sudo systemctl enable python-dashboard.service
sudo systemctl start python-dashboard.service
```