import qbittorrentapi
import feedparser
import json
from colorama import init,Fore

init(convert=True)

jsonDir = 'shows copy.json'

def login_to_qbittorrent():
    global qbt_client
    qbt_client = qbittorrentapi.Client(host="localhost", port=8080)

    try:
        qbt_client.auth_log_in()
    except qbittorrentapi.LoginFailed as e:
        print("Qbittorrent time out error: Aborting")  
        exit()

search_keywords = ["[EMBER]", "[ToonsHub]","[Xtrem]","[DKB]","[SubsPlease]","[Yameii]"]

def load_shows_from_json():
    try:
        with open(jsonDir, 'r') as json_file:
            data = json.load(json_file)
            return data
    except FileNotFoundError:
        print("File shows.json not found.")
        return None
    except json.JSONDecodeError:
        print("Error decoding JSON file shows.json.")
        return None
    
def poll_website(show):
    show_name = show['showName']
    season_number = show['seasonNumber']
    episode_number = show['episodeNumber']
    print(f"\n\nLooking for show: {show_name}")
    print(f"Season: {season_number}, episode: {episode_number}")
     
    if season_number==0:
        url = f"https://nyaa.si/?page=rss&q={show_name}+{episode_number:04d}&s=seeders&o=desc"
    else:
        url = f"https://nyaa.si/?page=rss&q={show_name}+s{season_number:02d}e{episode_number:02d}&s=seeders&o=desc"
    url = url.replace(" ", "+")
    feed = feedparser.parse(url)

    if feed.bozo == 0:
        if len(feed.entries)>0:
            for entry in feed.entries:
                print(entry.title)
                if any(keyword in entry.title for keyword in search_keywords):
                    print(Fore.GREEN  + "Found episode: " + show["showName"] + Fore.WHITE)
                    qbt_client.torrents.add(entry.nyaa_infohash,is_sequential_download=True,save_path="E:\\Downloads\\Movies\\"+show_name,category=show_name)                    
                    show['uploadTime'] = entry.published
                    show['episodeNumber'] +=1
                    return 
            print("No trusted uploader found")
    else:
        print("Error parsing feed:", feed.bozo_exception)
    print("No episode found, last upload was " + show['uploadTime'])

                  
                    
   


qbt_client = None
login_to_qbittorrent()


shows_data = load_shows_from_json()
bypass = False

if shows_data:
    # Process the loaded data
    # print(shows_data)
    for show in shows_data:
        if("enabled" in show or bypass):
            if(show["enabled"] or bypass):                 
               
                poll_website(show)
                
                

    with open(jsonDir, 'w') as json_file:
            json.dump(shows_data, json_file, indent=4)
                    
else:
    print("No file found")


