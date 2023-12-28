import qbittorrentapi

# Instantiate a Client using the appropriate WebUI configuration

qbt_client = qbittorrentapi.Client(host="localhost", port=8080)

try:
    qbt_client.auth_log_in()
except qbittorrentapi.LoginFailed as e:
    print(e)

# Display qBittorrent info
print(f"qBittorrent: {qbt_client.app.version}")
print(f"qBittorrent Web API: {qbt_client.app.web_api_version}")
for k, v in qbt_client.app.build_info.items():
    print(f"{k}: {v}")

# Retrieve and show all torrents
for torrent in qbt_client.torrents_info():
    print(f"{torrent.hash[-6:]}: {torrent.name} ({torrent.state})")

# Pause all torrents
qbt_client.torrents.resume.all()

# Log out from the qBittorrent client
qbt_client.auth_log_out()
