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


def getLatestEpisodes(num:int=5):
    returnString = ""
    response = getJsonResponse()
    for i in range(num):
        singleEpisode = response[i]
        uploadedTime = singleEpisode["uploadedFriendly"]
        if "hour" in uploadedTime or "minute" in uploadedTime:
            uploader = singleEpisode["uploader"]["ChannelName"]
            title = singleEpisode["title"]
            string = "%s\n%s\n%s\n\n"%(uploader, title, uploadedTime)
            returnString += string
    return returnString

def getLiveHermits():
    returnString = ""
    response = requests.get("https://hermitcraft.com/api/hermit").json()
    for hermits in response:
        if hermits["Streaming"]:
            returnString += hermits["DisplayName"] + " - https://www.twitch.tv/" + hermits["TwitchName"]
    return returnString

def getAllStatus():
    return getLiveHermits() + "\n\n" + getLatestEpisodes()

# For testing
# while(1):
#     os.system('clear')
#     print(getAllStatus())
#     sleep(REFRESH_TIME_M*60)

    
