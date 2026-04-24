#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <fstream>
#include <sstream>

#define PORT 8080
#define BUFFER_SIZE 4096

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);

    std::string request(buffer);

    // Parse request line
    std::istringstream request_stream(request);
    std::string method, path, version;
    request_stream >> method >> path >> version;

    std::cout << "Request: " << method << " " << path << std::endl;

    std::string response;

    // Default file
    if (path == "/") {
        path = "/index.html";
    }

    std::string file_path = "." + path;

    std::ifstream file(file_path);

    if (file.good()) {
        std::stringstream content;
        content << file.rdbuf();
        std::string body = content.str();

        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
        response += "\r\n";
        response += body;
    } else {
        std::string body = "<h1>404 Not Found</h1>";

        response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
        response += "\r\n";
        response += body;
    }

    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server running on http://localhost:" << PORT << std::endl;

    while (true) {
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        std::thread t(handle_client, client_socket);
        t.detach();
    }

    close(server_fd);
    return 0;
}