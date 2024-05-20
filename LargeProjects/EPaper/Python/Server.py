import json
from http.server import BaseHTTPRequestHandler, HTTPServer
import urllib

class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        parsed_path = urllib.parse.urlparse(self.path)
        if parsed_path.path == '/shows':
            self.handle_shows()
        elif parsed_path.path == '/status':
            self.handle_status()
        else:
            self.handle_not_found()
    
    def handle_shows(self):
        try:
            with open('../../../SmallProjects/MovieDownloader/shows.json', 'r') as file:
                shows = json.load(file)
            
            enabled_shows = []
            for show in shows:
                if show['enabled']:
                    if show['seasonNumber']==0:
                        show_info = f"https://nyaa.si/?page=rss&q={show['showName']}+{show['episodeNumber']:04d}+1080&s=seeders&o=desc"
                    else:
                        show_info = f"https://nyaa.si/?page=rss&q={show['showName']}+s{show['seasonNumber']:02d}e{show['episodeNumber']:02d}&s=seeders&o=desc"
                    enabled_shows.append(show_info.replace(" ", "+"))
            
            
            response_content = "\n".join(enabled_shows)
            
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(response_content.encode('utf-8'))
        except Exception as e:
            self.send_response(500)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            error_message = f"Error: {str(e)}"
            self.wfile.write(error_message.encode('utf-8'))

    def handle_status(self):
        response_content = "Server is running"
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(response_content.encode('utf-8'))

    def handle_not_found(self):
        self.send_response(404)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(b'404: Not Found')

def run(server_class=HTTPServer, handler_class=RequestHandler, port=8000):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
