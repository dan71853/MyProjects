import requests
from time import gmtime,strftime, sleep
import os


REFRESH_TIME_M = 15

def generateTimeString():
    return strftime("%a, %d %b %Y %H:%M:%S GMT",gmtime())


def generateRequest():
    return "https://hermitcraft.com/api/videos?type=Latest&start="+generateTimeString()


def getJsonResponse():
    response = requests.get(generateRequest())
    return response.json()


def printLatestEpisodes(num:int=5):
    response = getJsonResponse()
    for i in range(num):
        singleEpisode = response[i]
        uploadedTime = singleEpisode["uploadedFriendly"]
        if "hour" in uploadedTime or "minute" in uploadedTime:
            uploader = singleEpisode["uploader"]["ChannelName"]
            title = singleEpisode["title"]
            string = "%s\n%s\n%s\n"%(uploader, title, uploadedTime)
            print(string)

def printLiveHermits():
    response = requests.get("https://hermitcraft.com/api/hermit").json()
    for hermits in response:
        if hermits["Streaming"]:
            print(hermits["DisplayName"] + " - https://www.twitch.tv/" + hermits["TwitchName"])



while(1):
    os.system('clear')
    printLiveHermits()
    print("\n\n")
    printLatestEpisodes()
    sleep(REFRESH_TIME_M*60)

    
