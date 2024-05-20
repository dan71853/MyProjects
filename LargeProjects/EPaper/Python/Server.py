import json
from http.server import BaseHTTPRequestHandler, HTTPServer

class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
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

def run(server_class=HTTPServer, handler_class=RequestHandler, port=8000):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
