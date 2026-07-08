import json
import time
from colorama import init,Fore,Style
init(autoreset=True)

from HelperFunctions import *
from config import base_save_path,json_dir
 
    
def poll_website(show):
    feed = get_nyaa_feed(show)
    
    hash = parse_feed(feed)
    
    if hash == None:
        return
        
    print(Fore.GREEN  + "Found episode: " + show["alias"] + Fore.WHITE)
    qbt_client.torrents.add(hash,is_sequential_download=True,save_path="base_save_path"+show['alias'],category=show['alias'])                    
    show['uploadTime'] = timestamp_seconds = time.time()
    show['episodeNumber'] +=1    
                    
   


qbt_client = qbittorrent_login()


shows_data = load_json(json_dir)

if shows_data == None:
    time.sleep(5)
    exit()

for show in shows_data:
    if(show["enabled"]):                 
        poll_website(show)


with open(json_dir, 'w') as json_file:
        json.dump(shows_data, json_file, indent=4)

 

