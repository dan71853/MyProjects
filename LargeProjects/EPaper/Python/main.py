from Server import runServer
from ImageConvert import convertFiles
from ImageServer import startImageServer

import threading

if __name__ == '__main__':

    convertFiles("./NewImages")
    
    http_thread = threading.Thread(target=runServer)
    image_thread = threading.Thread(target=startImageServer)

    # Start both threads
    http_thread.start()
    image_thread.start()

    # Wait for both threads to finish
    http_thread.join()
    image_thread.join()

