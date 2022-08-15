import requests
import time
import shutil
import os

newFolderPath='C:/Users/sommerdani/Documents/Git/TestProjects/random/TimeLapseESPCamera/'+"output" +"/"
if not os.path.isdir(newFolderPath):
    os.mkdir(newFolderPath)

imageNum=0

while(imageNum <1000):

    t = time.gmtime(time.time()+46800)
    fileName = str(t.tm_mday)+"_"+str(t.tm_mon)+"_"+str(t.tm_year)+" "+str(t.tm_hour)+"_"+str(t.tm_min)
    url = "http://192.168.1.220/capture?_cb=1636312546302"+fileName
    image = requests.get(url, stream=True)
    with open(newFolderPath+fileName+'.jpg', 'wb') as out_file:
        shutil.copyfileobj(image.raw, out_file)

    print(".")
    imageNum +=1
    time.sleep(10*60)