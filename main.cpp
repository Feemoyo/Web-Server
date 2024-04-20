// #include <iostream>
// #include <string>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <sys/epoll.h>

// const int PORT = 8080;
// const int MAX_EVENTS = 10;
// const int BUFFER_SIZE = 1024;

// int main() {
//     int server_fd;
// 	int new_socket;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     char buffer[BUFFER_SIZE] = {0};

//     // TIP: se o Content-Length for maior que o necessario o browser ficara em loop esperando o ultimo byte
//     std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 84\n\n<html><head><title>WebServ</title></head><body><h1>Hello, World!!</h1></body></html>";

//     // Create epoll instance
//     int epoll_fd = epoll_create(10);
//     if (epoll_fd == -1) {
//         std::cerr << "Failed to create epoll instance" << std::endl;    
//         return -1;
//     }

//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         std::cerr << "Socket creation failed" << std::endl;
//         return -1;
//     }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     // Bind the socket to the port
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
//         std::cerr << "Bind failed" << std::endl;
//         return -1;
//     }

//     // Listen for incoming connections
//     if (listen(server_fd, 3) < 0) {
//         std::cerr << "Listen failed" << std::endl;
//         return -1;
//     }

//     // Add server socket to epoll
//     struct epoll_event event;
//     event.events = EPOLLIN;
//     event.data.fd = server_fd;
//     if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
//         std::cerr << "Failed to add server socket to epoll" << std::endl;
//         return -1;
//     }

//     struct epoll_event events[MAX_EVENTS];
//     while (epoll_wait(epoll_fd, events, MAX_EVENTS, -1) > 0)
//     {   
//         for (int i = 0; i < MAX_EVENTS; ++i) {
//             if (events[i].data.fd == server_fd) {
//                 // Accept incoming connection
//                 if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
//                     std::cerr << "Accept failed" << std::endl;
//                     continue;
//                 }

//                 // Add new connection to epoll
//                 event.events = EPOLLIN;
//                 event.data.fd = new_socket;
//                 if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
//                     std::cerr << "Failed to add new socket to epoll" << std::endl;
//                     close(new_socket);
//                     continue;
//                 }
//             } else {
//                 // Handle incoming data
//                 int client_socket = events[i].data.fd;
//                 ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
//                 if (bytes_received <= 0) {
//                     // Connection closed or error
//                     epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, NULL);
//                     std::cout << "Connection closed" << std::endl;
//                 } else {
//                     // Send response
//                     send(client_socket, response.c_str(), response.length(), 0);
//                     std::cout << "Response sent" << std::endl;
//                 }
//                 close(client_socket);
//             }
//         }
//     }

//     // Close the server socket
// 	std::cout << "Socket closed" << std::endl;
//     close(server_fd);
//     return 0;
// }

#include "Server.h"

int main(void)
{
	Server	*server = new Server(8080);
	server->start();

	delete server;
	
	return (0);
}