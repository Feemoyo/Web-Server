/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:36:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/22 18:01:16 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

Server::Server(void)
{
	return ;
}

Server::Server(int port)
:_port(port), _active_clients(0)
{
	this->_init_socket();
	this->_init_bind();
	return ;
}

Server::~Server(void)
{
	close(this->_server_socket);
	return ;
}

void	Server::start(void)
{
	this->_init_listen();
	std::cout << "Server running on port " << this->_port << "..." << std::endl;
	this->_init_fds();

	// TODO: Implementar uma classe para gerenciar os clientes
	while (true)
	{
		std::cout << "\nWaiting for connections..." << std::endl;
		int	poll_result = poll(this->_fds, this->_active_clients + 1, -1);
		std::cout << "Clients result: " << this->_active_clients << std::endl;

		if (poll_result < 0)
		{
			std::cerr << "Poll fail" << std::endl;
			break ;
		}
		
		if (this->_fds[0].revents & POLLIN)
		{
			if (_accept_request() != 0)
			{
				std::cerr << "Accept fail" << std::endl;
				break ;
			}
			std::cout << "New connection accepted" << std::endl;
		}
		 
		for(int i = 1; i <= this->_active_clients; ++i)
		{
			std::cout << "fds[0] " << this->_fds[i].revents << std::endl; 
			std::cout << "POLLIN " << POLLIN << std::endl;
			if (this->_fds[i].revents & POLLIN)
			{
				std::cout << "fds[i].fd " << this->_fds[i].fd << " " << i  << std::endl; 
				_process_request(this->_fds[i].fd);
				close(this->_fds[i].fd);
				this->_fds[i] = this->_fds[this->_active_clients--];
			}
		}
	}
	return ;
}

int	Server::_accept_request(void)
{
	sockaddr_in	client_addr;
	socklen_t	addr_size = sizeof(client_addr);
	int			client_socket = accept(this->_server_socket, reinterpret_cast<sockaddr *>(&client_addr), &addr_size);
	
	if (client_socket < 0)
	{
		std::cerr << "Accept fail" << std::endl;
		return (1);
	}
	this->_fds[++this->_active_clients].fd = client_socket;
	this->_fds[this->_active_clients].events = POLLIN;
	return (0);
}

void	Server::_process_request(int client_socket)
{
	char	buffer[1024] = {0};
	read(client_socket, buffer, sizeof(buffer));;

	std::string	response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 84\n\n<html><head><title>WebServ</title></head><body><h1>Hello, World!!</h1></body></html>";
	std::cout << client_socket << std::endl;
	write(client_socket, response.c_str(), response.size());
	std::cout << "Num Response: " << ++this->_NUM_REQUEST << std::endl;
	return ;
}

void	Server::_init_socket(void)
{
	this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	this->_server_addr.sin_family = AF_INET;
	this->_server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server_addr.sin_port = htons(this->_port);
	if (this->_server_socket < 0)
	{
		std::cerr << "Socket creation failed" << std::endl;
        exit(1);
	}
	
	return ;
}

void	Server::_init_bind(void)
{
	if (bind(this->_server_socket, reinterpret_cast<sockaddr *>(&this->_server_addr), sizeof(this->_server_addr)) < 0)
	{
		std::cerr << "Bind failed" << std::endl;
		close(this->_server_socket);
       	exit(1);
	}
	return ;
}

void	Server::_init_listen(void)
{
	if (listen(this->_server_socket, 3) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		close(this->_server_socket);
		exit(1);
	}
	return ;
}

void	Server::_init_fds(void)
{
	this->_fds[0].fd = this->_server_socket;
	this->_fds[0].events = POLLIN;
	for (int i = 1; i < _MAX_CLIENTS - 1; i++)
	{
		this->_fds[i].fd = 0;
		this->_fds[i].events = 0;
		this->_fds[i].revents = 0;
	}
	return ;
}

void	Server::stop(void)
{
	close(_server_socket);
	return ;
}
