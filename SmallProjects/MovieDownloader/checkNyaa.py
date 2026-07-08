from colorama import init,Fore,Style
import time

from config import json_dir
from HelperFunctions import *

init(autoreset=True)
    
def poll_website(show):
    feed = get_nyaa_feed(show)
    
    hash = parse_feed(feed)
    
    if hash == None:
        old_date, new_date = check_time(show)
        print_time_remaining(old_date, new_date)
        return
           
    print(Fore.GREEN  + "Found episode: " + show["alias"] + Fore.WHITE)


shows_data = load_json(json_dir)

if shows_data == None:
    time.sleep(5)
    exit()

for show in shows_data:
    if(show["enabled"]):                 
        poll_website(show)


time.sleep(5)