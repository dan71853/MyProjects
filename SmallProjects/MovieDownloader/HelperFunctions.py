import json
import qbittorrentapi
import feedparser
import time
from colorama import init,Fore,Style


from config import search_keywords
# Load the json file with the shows data
# Return an array of dictionaries for all shows
def load_json(dir:str):
    try:
        with open(dir, 'r') as json_file:
            data = json.load(json_file)
            return data
    except FileNotFoundError:
        print("File shows.json not found.")
        return None
    except json.JSONDecodeError:
        print("Error decoding JSON file shows.json.")
        return None

# Login to the qBittorrent client
# Enable Bypass authentication for clients on localhost on qBittorrent
def qbittorrent_login():
    qbt_client = qbittorrentapi.Client(host="localhost", port=8080)

    try:
        qbt_client.auth_log_in()
        return qbt_client
    except qbittorrentapi.LoginFailed as e:
        print("Qbittorrent time out error: Aborting")  
        exit()


def get_nyaa_feed(show):
    show_name = show['showName']
    show_alias = show['alias']
    season_number = show['seasonNumber']
    episode_number = show['episodeNumber']
    print(f"\n\nLooking for show: {show_alias}")
    print(f"Season: {season_number}, episode: {episode_number}")

    url = f"https://nyaa.si/?page=rss&q={show_name}+s{season_number:02d}e{episode_number:02d}&s=seeders&o=desc"
    url = url.replace(" ", "+")
    feed = feedparser.parse(url)
    return feed

def parse_feed(feed):
    if feed.bozo != 0:
        print("Error parsing feed:", feed.bozo_exception)
        return None
    
    if len(feed.entries)<=0:
        print("No episode found: " )
        return None

        #Loop over all entries for a show
    for entry in feed.entries:
        # print(entry.title)
        #Ignore any english dub entries
        if "[English Dub]" in entry.title: 
            continue

        if any(keyword in entry.title for keyword in search_keywords):
            return entry.nyaa_infohash
    print("No trusted uploader found")
        




def check_time(show):
    if(show['uploadTime'] == -1):
        print("No time data")
        return None
    old_date = show['uploadTime'] + 607800 #Add 1 week
    new_date = time.time()

    return old_date, new_date

def print_time_remaining(old_date, new_date):
    if old_date > new_date:
        delta = old_date - new_date

        # Calculate remaining days and hours
        remaining_days = delta // (24*60*60)
        remaining_hours = (delta%(24*60*60)) // (60*60)
        remaining_minutes = (delta%(60*60)) // 60

        if(remaining_days<=1):
            print(Fore.LIGHTRED_EX + f"Expected: {remaining_days} days, {remaining_hours} hours, {remaining_minutes} minutes"+Fore.WHITE)
        else:
            print(f"Expected: {remaining_days} days, {remaining_hours} hours")
    else:
        print(Fore.RED + "Show should be out" + Fore.WHITE)
                  