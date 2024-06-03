import requests
import time
import feedparser
from datetime import datetime

show_name = "Mashle+ember"
season_number= 2
episode_number= 1

url = f"https://nyaa.si/?f=0&c=0_0&q={show_name}+s{season_number:02d}e{episode_number:02d}&s=seeders&o=desc"

startTime = time.time()


url = f"https://nyaa.si/?page=rss&q={show_name}+s{season_number:02d}e{episode_number:02d}&s=seeders&o=desc"
# response = requests.get(url)

feed = feedparser.parse(url)

print(feed.entries[0].title)
print(feed.entries[0].nyaa_infohash)
print(feed.entries[0].published)
given_date = datetime.strptime(feed.entries[0].published, "%a, %d %b %Y %H:%M:%S %z")
# Get the current datetime
current_date = datetime.now(given_date.tzinfo)
print(feed.entries[0].published)



exit()
if feed.bozo == 0:
    print("Feed parsing successful")
    # Print feed title and entries
    print("Title:", feed.feed.title)
    print("Entries:")
    for entry in feed.entries:
        print(entry.title)
        print(entry.link)
        print(entry.published)
        print("\n")
else:
    print("Error parsing feed:", feed.bozo_exception)

# print(response.text)
# print(time.time() - startTime)
 
