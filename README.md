# INET-3101-Programming-Assignment-5
C client-server program using sockets to request files, handle errors, and support graceful shutdown.

# INET 3101 - Programming Assignment 5

## Problem Statement
This project implements a client-server application in C using socket programming. The server accepts a filename from the client and returns the file contents as a text response. The file must not exceed 255 characters. The program must also handle error conditions such as invalid filenames, file not found, and files exceeding the allowed size. As a bonus feature, the server supports graceful shutdown using signal handling.

---

## Describe the Solution
The solution consists of two programs: a server and a client.

The server creates a TCP socket, binds to port 8080, and listens for incoming client connections. When a client connects, the server receives a filename, validates it, and attempts to open the file. If the file exists and its contents are within the 255-character limit, the server reads and sends the content back to the client. If not, it sends an appropriate error message.

The client creates a socket and connects to the server using localhost. It sends a filename as a request and waits for a response. Once the response is received, it is printed to the terminal.

The server also includes signal handling using SIGINT so that pressing Ctrl+C will shut down the server gracefully without leaving resources open.

---

## Pros and Cons of the Solution

### Pros
- Simple and easy to understand implementation of client-server communication  
- Demonstrates core networking concepts using sockets in C  
- Handles common error cases (invalid filename, file not found, file too large)  
- Includes graceful shutdown using signal handling  
- Clear separation between client and server functionality  

### Cons
- Handles only one client at a time (no concurrency)  
- Limited to files with a maximum of 255 characters  
- Client is hardcoded to connect to localhost  
- Minimal security (no encryption or authentication)  
