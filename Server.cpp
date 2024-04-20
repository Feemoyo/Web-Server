/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:36:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/19 23:35:28 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

Server::Server(void)
{
	return ;
}

Server::Server(int n_port)
:port(n_port)
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
	{
		std::cerr << "Socket creation failed" << std::endl;
        exit(1);
	}

	sockaddr_in	server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	if (bind(server_socket, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) < 0)
	{
		std::cerr << "Bind failed" << std::endl;
		close(server_socket);
       	exit(1);
	}

	return ;
}

Server::~Server(void)
{
	close(server_socket);
	return ;
}

void	Server::start(void)
{
	if (listen(server_socket, 3) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		close(server_socket);
		exit(1);
	}
	
	std::cout << "Server running on port " << port << "..." << std::endl;

	struct pollfd	fds[MAX_CLIENTS + 1];
	fds[0].fd = server_socket;
	fds[0].events = POLLIN;

	int	active_clients = 0;

	while (true)
	{
		int	poll_result = poll(fds, active_clients + 1, -1);
		if (poll_result < 0)
		{
			std::cerr << "Poll fail" << std::endl;
			break ;
		}
		if (fds[0].revents & POLLIN)
		{
			sockaddr_in	client_addr;
			socklen_t	addr_size = sizeof(client_addr);
			int			client_socket = accept(server_socket, reinterpret_cast<sockaddr *>(&client_addr), &addr_size);
		
			if (client_socket < 0)
			{
				std::cerr << "Accepting connection fail" << std::endl;
				continue ;
			}
			fds[++active_clients].fd = client_socket;
			fds[active_clients].events = POLLIN;

			std::cout << "New connection accepted" << std::endl;
		}
		for(int i = 1; i <= active_clients; ++i)
		{
			if (fds[i].revents & POLLIN)
			{
				process_request(fds[i].fd);
				close(fds[i].fd);
				fds[i] = fds[active_clients--];
			}
		}
	}
	return ;
}

// void	Server::stop(void)
// {
// 	close(server_socket);
// 	exit(0);
// }

void	Server::process_request(int client_socket)
{
	char	buffer[1024] = {0};
	read(client_socket, buffer, sizeof(buffer));;

	std::string	response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 84\n\n<html><head><title>WebServ</title></head><body><h1>Hello, World!!</h1></body></html>";
	write(client_socket, response.c_str(), response.size());
}
