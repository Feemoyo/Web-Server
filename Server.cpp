/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:36:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/29 20:56:02 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
			if (this->_fds[i].revents & POLLIN)
			{
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
		return (EXIT_FAILURE);
	}
	this->_fds[++this->_active_clients].fd = client_socket;
	this->_fds[this->_active_clients].events = POLLIN;
	return (EXIT_SUCCESS);
}

void	Server::_process_request(int client_socket)
{
	// TIPS: dentro do buffer a segunda substr é o path do request
	char	buffer[1024] = {0};
	// std::stringstream buffer;
	read(client_socket, buffer, sizeof(buffer));
	std::cout << "requests: " << buffer << std::endl;

	// TODO: Adicionar um tratamento para os multiplos requests ex: o proximo request "icon.svg"
	this->_response.set_socket(client_socket);
	this->_response.send_response();
	
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
        exit(EXIT_FAILURE);
	}
	
	return ;
}

void	Server::_init_bind(void)
{
	if (bind(this->_server_socket, reinterpret_cast<sockaddr *>(&this->_server_addr), sizeof(this->_server_addr)) < 0)
	{
		std::cerr << "Bind failed" << std::endl;
		close(this->_server_socket);
       	exit(EXIT_FAILURE);
	}
	return ;
}

void	Server::_init_listen(void)
{
	if (listen(this->_server_socket, 3) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		close(this->_server_socket);
		exit(EXIT_FAILURE);
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
