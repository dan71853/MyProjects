import feedparser
import json
import time
from colorama import init,Fore
from datetime import datetime,timezone
from dateutil.relativedelta import relativedelta

init(convert=True)

jsonDir = 'shows.json'


# search_keywords = ["[EMBER]", "[ToonsHub]","[Xtrem]","[DKB]","[SubsPlease]","[LostYears]"]
search_keywords = ["[EMBER]", "[Xtrem]","[DKB]","[SubsPlease]","[LostYears]"] #"[ToonsHub]"

date_format = "%a, %d %b %Y %H:%M:%S %z"

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
        url = f"https://nyaa.si/?page=rss&q={show_name}+{episode_number:04d}+1080&s=seeders&o=desc"
    else:
        url = f"https://nyaa.si/?page=rss&q={show_name}+s{season_number:02d}e{episode_number:02d}&s=seeders&o=desc"
    url = url.replace(" ", "+")
    feed = feedparser.parse(url)

    if feed.bozo == 0:
        if len(feed.entries)>0:
            for entry in feed.entries:
                # print(entry.title)
                if any(keyword in entry.title for keyword in search_keywords):
                    print(Fore.GREEN  + "Found episode: " + show["showName"] + Fore.WHITE)                   
                    return 
            print("No trusted uploader found")
    else:
        print("Error parsing feed:", feed.bozo_exception)
    print("No episode found, last upload was ")# + show['uploadTime'])
    if(show['uploadTime'] == -1):
        print("No time data")
        return
    old_date = show['uploadTime'] + 607800 #Add 1 week
    new_date = time.time()
    
    


    if old_date > new_date:
        time_difference = old_date - new_date

        # Calculate remaining days and hours
        remaining_days = time_difference // (24*60*60)
        remaining_hours = (time_difference%(24*60*60)) // (60*60)
        remaining_minutes = (time_difference%(60*60)) // 60

        if(remaining_days<=1):
            print(Fore.LIGHTRED_EX + f"Expected: {remaining_days} days, {remaining_hours} hours, {remaining_minutes} minutes"+Fore.WHITE)
        else:
            print(f"Expected: {remaining_days} days, {remaining_hours} hours")
    else:
        print(Fore.RED + "Show should be out" + Fore.WHITE)
                  
                    



shows_data = load_shows_from_json()
bypass = False

if shows_data:
    # Process the loaded data
    # print(shows_data)
    for show in shows_data:
        if("enabled" in show or bypass):
            if(show["enabled"] or bypass):                 
               
                poll_website(show)
                
    time.sleep(5)
else:
    print("No file found")


