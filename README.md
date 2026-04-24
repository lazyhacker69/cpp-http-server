# Multithreaded HTTP Server in C++

A lightweight HTTP server built using C++ and POSIX sockets on Linux.

## Features
- Handles HTTP GET requests
- Serves static files (HTML)
- Supports multiple clients using multithreading
- Custom portfolio frontend with Bootstrap and JavaScript

## Tech Stack
C++, POSIX Sockets, TCP/IP, Linux, HTML, CSS, JavaScript

## How to Run

```bash
g++ server.cpp -o server -pthread
./server
