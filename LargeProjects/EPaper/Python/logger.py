import datetime


def log(text):
    file = open("log.txt", "a")    
    time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S - ')
    file.write(time + text + "\n")
    file.close()