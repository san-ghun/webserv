### Overview

1. Initialization: The server starts by initializing necessary components such as configuration settings and logging.
2. Socket Setup: The server creates a listening socket to accept incoming connections.
3. Polling for Events: The server uses the poll() system call to wait for events on the listening socket and any connected sockets.
4. Handling Connections: When a new connection is detected, the server accepts the connection and adds the new socket to the polling list.
5. Request Handling: The server reads incoming HTTP requests from connected sockets.
6. Response Generation: The server generates HTTP responses based on the requests, serving static files or handling other types of requests as necessary.
7. Sending Responses: The server sends the generated HTTP responses back to the clients.
8. Cleanup: The server cleans up closed connections and prepares to handle new incoming requests.

### Detailed Flow

1. Initialization
   • Load configuration settings from a file or environment variables using Config.
   • Initialize the logger using Logger to enable logging throughout the server.
   • Create an instance of HTTPServer.
2. Socket Setup
   • The HTTPServer creates a listening socket using Socket.
   • Bind the socket to a specified port and set it to listen for incoming connections.
3. Polling for Events
   • The HTTPServer creates an instance of Poller.
   • Add the listening socket to the Poller to monitor for incoming connection events.
4. Handling Connections
   • When the Poller detects an event on the listening socket, the HTTPServer accepts the new connection using Socket.
   • Add the new client socket to the Poller to monitor for incoming data.
5. Request Handling
   • When the Poller detects data available on a client socket, the HTTPServer reads the data.
   • Parse the incoming data as an HTTP request using HTTPRequest.
6. Response Generation
   • The HTTPServer uses RequestHandler to process the request.
   • If the request is for a static file, RequestHandler delegates to StaticFileHandler.
   • StaticFileHandler reads the requested file from the static/ directory and prepares an HTTP response using HTTPResponse.
7. Sending Responses
   • The HTTPServer sends the HTTP response back to the client using the client socket.
   • Continue monitoring the socket for further requests or connection closure.
8. Cleanup
   • When a client closes the connection, the HTTPServer removes the client socket from the Poller and closes the socket.
   • Periodically, the server performs cleanup tasks such as freeing unused resources and logging server status.
