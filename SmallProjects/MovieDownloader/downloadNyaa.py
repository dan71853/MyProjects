import qbittorrentapi
import requests
from bs4 import BeautifulSoup
import json
import subprocess


def login_to_qbittorrent():
    global qbt_client
    qbt_client = qbittorrentapi.Client(host="localhost", port=8080)

    try:
        qbt_client.auth_log_in()
    except qbittorrentapi.LoginFailed as e:
        print("Qbittorrent time out error: Aborting")  
        exit()


def load_shows_from_json():
    try:
        with open('shows.json', 'r') as json_file:
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
        url = f"https://nyaa.si/?f=0&c=0_0&q={show_name}+{episode_number:04d}&s=seeders&o=desc"
    else:
        url = f"https://nyaa.si/?f=0&c=0_0&q={show_name}+s{season_number:02d}e{episode_number:02d}&s=seeders&o=desc"
    response = requests.get(url)
    return response

def extract_info(response):
    soup = BeautifulSoup(response.text, 'html.parser')
    tr_elements = soup.find_all('tr', class_=['default', 'danger','success'])
    
    extracted_data = []
    
    for tr in tr_elements:
        
        td_elements = tr.find_all('td')
  
        if len(td_elements) >= 3:
            title_1 = td_elements[0].find('img').get('alt')
            
            elements = td_elements[1].find_all('a')
            for element in elements:
                title_2 = element.get('title')
                if len(title_2) > 15:
                    break
            
            magnet = td_elements[2].find('a', href=lambda href: href and href.startswith('magnet:')).get('href')
            
            data = {
                'title_1': title_1,
                'title_2': title_2,
                'magnet': magnet
            }
            
            extracted_data.append(data)
    
    return extracted_data


def process_data(json_data):
    data = json.loads(json_data)
    if not data:
        print("No data found.")
        return -1
    
    search_keywords = ["[EMBER]", "[ToonsHub]","[Xtrem]","[DKB]","[SubsPlease]"]
    
    for index, item in enumerate(data):
        title_2 = item["title_2"]
        if any(keyword in title_2 for keyword in search_keywords):
            magnet = item["magnet"]
            print(f"Title 2: {title_2}")
            print(f"Magnet: {magnet}")
            return index
    
    return -1

def download_magnet(magnet,show_name):
    qbt_client.torrents.add(magnet,is_sequential_download=True,save_path="E:\\Downloads\\Movies\\"+show_name,category=show_name)
        
def print_data(json_data, index,show_name):
    data = json.loads(json_data)
    if not data:
        print("No data found.")
        return 0
    
    if index < 0 or index >= len(data):
        print("Invalid index.")
        return 0
    
    item = data[index]
    title_2 = item["title_2"]
    magnet = item["magnet"]
    
    print(f"Title 2: {title_2}")
    print(f"Magnet: {magnet}")
    download_magnet(magnet,show_name)
    return 1


def increment_episode_number(show_name):
    try:
        with open("shows.json", 'r') as json_file:
            data = json.load(json_file)
        
        # Find the show by matching the show_name
        for show in data:
            if show['show_name'] == show_name:
                show['episode_number'] += 1
                break  # Break out of the loop once the show is found
        
        # Write the updated data back to the file
        with open("shows.json", 'w') as json_file:
            json.dump(data, json_file, indent=4)
        
        print("Episode number of shows.json incremented successfully.")
        return True
    except FileNotFoundError:
        print("File shows.json' not found.")
        return False
    except json.JSONDecodeError:
        print("Error decoding JSON file shows.json.")
        return False
    except KeyError:
        print("Show shows.json not found in the JSON data.")
        return False


qbt_client = None
login_to_qbittorrent()


shows_data = load_shows_from_json()
bypass = False

if shows_data:
    # Process the loaded data
    for show in shows_data:
        if("enabled" in show or bypass):
            if(show["enabled"] or bypass):     
                show_name = show['show_name']
                season_number = show['season_number']
                episode_number = show['episode_number']
                print(show_name)
                print(episode_number)
                response = poll_website(show_name, season_number, episode_number)
                data = extract_info(response)
                json_data = json.dumps(data, indent=4)
                index = process_data(json_data)
                if print_data(json_data, index,show_name):
                    increment_episode_number(show_name)
                    
else:
    print("No file found")


