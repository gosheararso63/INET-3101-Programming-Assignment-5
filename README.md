# Module 5 - Client Server in C

## Problem Statement
Client sends filename, server returns file contents (max 255 chars). Handles errors.

## Describe the Solution
Server listens on port 8080, receives filename, validates it, reads file, and sends content or error.
Client connects, sends filename, prints response.
Server supports graceful shutdown using Ctrl+C.

## Pros and Cons

Pros:
- Simple
- Handles errors
- Uses sockets
- Graceful shutdown

Cons:
- One client at a time
- Max 255 chars
- Localhost only
