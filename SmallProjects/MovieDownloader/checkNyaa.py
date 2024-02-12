import feedparser
import json
from colorama import init,Fore
from datetime import datetime,timezone
from dateutil.relativedelta import relativedelta

init(convert=True)

jsonDir = 'shows.json'


search_keywords = ["[EMBER]", "[ToonsHub]","[Xtrem]","[DKB]","[SubsPlease]","[Yameii]"]
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
                    return 
            print("No trusted uploader found")
    else:
        print("Error parsing feed:", feed.bozo_exception)
    print("No episode found, last upload was " + show['uploadTime'])

    parsed_date = datetime.strptime(show['uploadTime'], date_format)
    # Add a week to the datetime object
    new_date = parsed_date + relativedelta(weeks=1)
    # Get the current date and time in UTC timezone and make it naive
    current_date = datetime.now(timezone.utc).replace(tzinfo=None)
    # Make new_date naive
    new_date = new_date.replace(tzinfo=None)   
    if current_date < new_date:
        # Calculate the difference
        time_difference = new_date - current_date
        
        # Calculate remaining days and hours
        remaining_days = time_difference.days
        remaining_hours = time_difference.seconds // 3600  # 3600 seconds in an hour

        print(f"Expected: {remaining_days} days, {remaining_hours} hours")
    else:
        print("The new date is not before the current date.")
                  
                    



shows_data = load_shows_from_json()
bypass = False

if shows_data:
    # Process the loaded data
    # print(shows_data)
    for show in shows_data:
        if("enabled" in show or bypass):
            if(show["enabled"] or bypass):                 
               
                poll_website(show)
                exit()
                
                    
else:
    print("No file found")


