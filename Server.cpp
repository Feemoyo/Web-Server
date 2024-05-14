/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:36:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/14 02:11:55 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(void)
{
	return ;
}

Server::Server(t_server &server)
{
	this->_server = server;
	this->_init_socket();
	this->_init_bind();
	this->_init_listen();

	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server(void)
{
	close(this->_server.server_socket);
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// void	Server::start(void)
// {
// 	this->_init_listen();
// 	std::cout << "Server running on port " << this->_server.port << "..." << std::endl;
	// this->_init_fds();

	// // TODO: Implementar uma classe para gerenciar os clientes (FD_SET)
	// while (true)
	// {
	// 	std::cout << "\nWaiting for connections..." << std::endl;
	// 	int	poll_result = poll(this->_server.fds, this->_server.active_clients + 1, -1);
	// 	std::cout << "Clients result: " << this->_server.active_clients << std::endl;

	// 	if (poll_result < 0)
	// 	{
	// 		std::cerr << "Poll fail" << std::endl;
	// 		break ;
	// 	}
		
	// 	if (this->_server.fds.revents & POLLIN)
	// 	{
	// 		if (_accept_request() != 0)
	// 		{
	// 			std::cerr << "Accept fail" << std::endl;
	// 			break ;
	// 		}
	// 		std::cout << "New connection accepted" << std::endl;
	// 	}
		
	// 	// for(int i = 1; i <= this->_server.active_clients; ++i)
	// 	// {
	// 	// 	if (this->_server.fds.revents & POLLIN)
	// 	// 	{
	// 	// 		_process_request(this->_server.fds.fd);
	// 	// 		close(this->_server.fds.fd);
	// 	// 		this->_server.fds = this->_server.fds;
	// 	// 	}
	// 	// }
	// }
// 	return ;
// }


// void	Server::_process_request(int client_socket)
// {
// 	char	buffer[1024] = {0};
// 	read(client_socket, buffer, sizeof(buffer));
	
// 	this->_request.set_buffer(buffer);
// 	std::string path = this->_request.get_path();
// 	this->_response.set_socket(client_socket);
// 	this->_response.set_file("." + path);
// 	this->_response.send_response();
// 	return ;
// }

void	Server::_init_socket(void)
{
	this->_server.server_socket = socket(AF_INET, SOCK_STREAM, 0);
	this->_server.server_addr.sin_family = AF_INET;
	this->_server.server_addr.sin_addr.s_addr = INADDR_ANY;
	this->_server.server_addr.sin_port = htons(this->_server.port);
	if (this->_server.server_socket < 0)
	{
		std::cerr << "Socket creation failed on server port:" << this->_server.port << std::endl;
        exit(EXIT_FAILURE);
	}
	std::cout << "Server socket on port " << this->_server.port << std::endl;
	return ;
}

void	Server::_init_bind(void)
{
	if (bind(this->_server.server_socket, reinterpret_cast<sockaddr *>(&this->_server.server_addr), sizeof(this->_server.server_addr)) < 0)
	{
		std::cerr << "Bind failed on server port: " << this->_server.port << std::endl;
		close(this->_server.server_socket);
       	exit(EXIT_FAILURE);
	}
	std::cout << "Binded on port " << this->_server.port << std::endl;
	return ;
}

void	Server::_init_listen(void)
{
	if (listen(this->_server.server_socket, MAX_CLIENTS) < 0)
	{
		std::cerr << "Listen failed on port: " << this->_server.port << std::endl;
		close(this->_server.server_socket);
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening on port " << this->_server.port << std::endl;
	return ;
}

void	Server::accept_connection(struct pollfd &fds)
{
	if (fds.revents & POLLIN)
	{
		int	client_socket = accept(this->_server.server_socket, NULL, NULL);
		if (client_socket < 0)
		{
			std::cerr << "Accept fail on port: " << this->_server.port << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cout << "Connection accepted on port: " << this->_server.port << std::endl;
		close(client_socket);
	}
	return ;
}

int	&Server::get_server_port(void)
{
	return (this->_server.port);
}

int	&Server::get_server_socket(void)
{
	return (this->_server.server_socket);
}
