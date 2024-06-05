/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:58:36 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/14 04:58:29 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Servers.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Servers::Servers(void)
{
	return ;
}

Servers::Servers(Config *&config)
{
	for (size_t i = 0; i < config->get_total_servers(); i++)
		this->_servers.push_back(config->get_server(i));
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Servers::~Servers(void)
{
	close_servers();	
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Servers::_init_socket(void)
{
	for (size_t i = 0; i < this->_servers.size(); i ++)
	{
		this->_servers[i].server_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_servers[i].server_socket < 0)
		{
			std::cerr << "Socket creation failed on server port:" << this->_servers[i].port << std::endl;
			close(this->_servers[i].server_socket);
			continue ;
		}
		this->_servers[i].server_addr.sin_family = AF_INET;
		this->_servers[i].server_addr.sin_addr.s_addr = INADDR_ANY;
		this->_servers[i].server_addr.sin_port = htons(this->_servers[i].port);
	}
	return ;
}

bool	Servers::_init_bind(void)
{
	for (size_t i = 0; i < this->_servers.size(); i ++)
	{
		if (bind(this->_servers[i].server_socket, reinterpret_cast<sockaddr *>(&this->_servers[i].server_addr), sizeof(this->_servers[i].server_addr)) < 0)
		{
			std::cerr << "Bind failed on server port: " << this->_servers[i].port << std::endl;
			close_servers();
			return (false);
		}
	}
	return (true);
}

void	Servers::_init_listen(void)
{
	for (size_t i = 0; i < this->_servers.size(); i ++)
	{
		if (listen(this->_servers[i].server_socket, MAX_CLIENTS) < 0)
		{
			std::cerr << "Listen failed on port: " << this->_servers[i].port << std::endl;
			close_servers();
			break ;
		}
	}
	return ;
}

void	Servers::_init_fds(void)
{
	for(size_t i = 0; i < this->_servers.size(); i++)
	{
		struct pollfd	fds;
	
		fds.fd = this->_servers[i].server_socket;
		fds.events = POLLIN;
		fds.revents = 0;
		this->_fds.push_back(fds);
	}
	return ;
}

void	Servers::run_servers(void)
{
	_init_socket();
	if (_init_bind() == false)
		return;
	_init_listen();
	_init_fds();
	while (true)
	{
		std::cout << "Webserv running..." << std::endl;
		if (poll(this->_fds.data(), this->_fds.size(), -1) < 0)
		{
			std::cerr << "Poll fail" << std::endl;
			close_servers();
			break ;
		}
		for (size_t i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents & POLLIN)
				_accept_connection(i);
		}
	}
	return ;
}

void	Servers::_accept_connection (size_t index)
{
	struct sockaddr_in	client_addr;
	socklen_t			client_len = sizeof(client_addr);
	int					client_fd = accept(this->_fds[index].fd, (struct sockaddr*)&client_addr, &client_len);

	if (client_fd < 0)
	{
		std::cerr << "Accept fail on port: " << this->_servers[index].port << std::endl;
		close_servers();
		close(client_fd);
		return ;
	}
	_process_client(index, client_fd);
	_process_response(index, client_fd, "GET");
	close(client_fd);
	return ;
}

void	Servers::_process_client(size_t index, int &client_fd)
{
	char	buffer[1024];
	ssize_t	bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);

	if (bytes_read < 0)
	{
		std::cerr << "Reading from client fail on port: " << this->_servers[index].port << std::endl;
		return ;
	}
	else if (bytes_read == 0)
	{
		std::cerr << "Client closed connection on port: " << this->_servers[index].port << std::endl;
		return ;
	}
	this->_client.set_buffer(buffer);
	return ;
}

void	Servers::_process_response(size_t index, int &client_fd, std::string method)
{
	Response	resp(client_fd, this->_servers[index], this->_client.get_path(), method);
	
	resp.run_response();
	return ;
}

void	Servers::close_servers(void)
{
	for (size_t i = 0; i < this->_servers.size(); i++)
		close(this->_servers[i].server_socket);
	this->_servers.clear();
	this->_fds.clear();
	return ;
}