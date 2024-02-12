import time 
import feedparser
import json
from colorama import init,Fore

init(convert=True)

search_keywords = ["[EMBER]", "[ToonsHub]","[Xtrem]","[DKB]","[SubsPlease]","[Yameii]"]

def load_shows_from_json():
    try:
        with open('shows copy.json', 'r') as json_file:
            data = json.load(json_file)
            return data
    except FileNotFoundError:
        print("File shows.json not found.")
        return None
    except json.JSONDecodeError:
        print("Error decoding JSON file shows.json.")
        return None
    
def poll_website(show_name, season_number, episode_number):
    if season_number==0:
        url = f"https://nyaa.si/?page=rss&q={show_name}+{episode_number:04d}&s=seeders&o=desc"
    else:
        url = f"https://nyaa.si/?page=rss&q={show_name}+s{season_number:02d}e{episode_number:02d}&s=seeders&o=desc"
    url = url.replace(" ", "+")
    feed = feedparser.parse(url)

    print(url)
    if feed.bozo == 0:
        print("Feed parsing successful")
        if len(feed.entries)>0:
            print("Ep found")
            for entry in feed.entries:
                print(entry.title)
                if any(keyword in entry.title for keyword in search_keywords):
                    magnet = entry.nyaa_infohash
                    date = entry.published
                    return True, magnet,date
    else:
        print("Error parsing feed:", feed.bozo_exception)
    return False,"empty" # No show found
   



def process_data(json_data):
    data = json.loads(json_data)
    if not data:
        print("No episode found.")
        return -1
    
    search_keywords = ["[EMBER]", "[ToonsHub]","[Xtrem]","[DKB]","[SubsPlease]","[Yameii]"]
    
    for index, item in enumerate(data):
        title_2 = item["title_2"]
        # print(title_2)
        if any(keyword in title_2 for keyword in search_keywords):
            print(Fore.GREEN  + "Found episode: " , )
            print(Fore.GREEN + title_2)
            print(Fore.WHITE)
            return index
    
    
    print("Episode found but not from trusted uploader.")
    return -1



shows_data = load_shows_from_json()
bypass = False
if shows_data:
    # Process the loaded data
    for show in shows_data:
        if("enabled" in show or bypass):
            if(show["enabled"] or bypass):     
        
                show_name = show['showName']
                season_number = show['seasonNumber']
                episode_number = show['episodeNumber']
                print(f"\n\nLooking for show: {show_name}")
                print(f"Season: {season_number}, episode: {episode_number}")
                response = poll_website(show_name, season_number, episode_number)
                if(response[0]):
                    print(response[1:])
                else:
                    print("No show found, last upload was " + show['uploadTime'])
                exit()
                data = extract_info(response)
                json_data = json.dumps(data, indent=4)
                index = process_data(json_data)
            
else:
    print("No file found")

time.sleep(5)


