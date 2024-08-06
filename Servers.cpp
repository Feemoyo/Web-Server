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

std::string	_to_string(int num)
{
	std::stringstream ss;

    ss << num;
    
	std::string str = ss.str();
	return (str);
}

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
			std::cerr << "Socket creation failed on server port:" << this->_servers[i].port << "\n";;
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
			std::cerr << "Bind failed on server port: " << this->_servers[i].port << "\n";;
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
			std::cerr << "Listen failed on port: " << this->_servers[i].port << "\n";;
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
		std::cout << "Webserv running..." << "\n";;
		int	poll_count = poll(this->_fds.data(), this->_fds.size(), -1);
		if (poll_count < 0)
		{
			std::cerr << "Poll fail" << "\n";;
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
		std::cerr << "Accept fail on port: " << this->_servers[index].port << "\n";;
		close_servers();
		return ;
	}
	int flag = fcntl(client_fd, F_GETFL, 0);
	if (flag < 0 || fcntl(client_fd, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		std::cerr << "Fcntl fail on port: " << this->_servers[index].port << "\n";;
		close(client_fd);
		return ;
	}
	_process_client(index, client_fd);
	_process_response(index, client_fd, this->_client.get_method());
	close(client_fd);
	return ;
}

void	Servers::_process_client(size_t index, int &client_fd)
{
	std::vector<char>	buffer(8192, 0);
	ssize_t				bytes_read;
	bool				payload = false;
	int					retry_count = 0;
	const int			max_retries = 5;


	this->_client.clear_buffer();
	this->_client.clear_body_size();
	while(true)
	{
		buffer.assign(buffer.size(), 0);
		bytes_read = recv(client_fd, &buffer[0], buffer.size() - 1, 0);
		if (bytes_read < 0)
		{
			retry_count++;
			if (retry_count >= max_retries)
			{
				std::cerr << "Reading from client fail on port: " << this->_servers[index].port << std::endl;
				break ;
			}
			usleep(100 * 1000);
			continue ;
		}
		else if (bytes_read == 0)
		{
			std::cerr << "Client closed connection on port: " << this->_servers[index].port << std::endl;
			break ;
		}
		if (!this->_client.set_buffer(buffer, payload))
			break ;
	}
	if (this->_client.get_method() == "POST")
	{
		this->_client.set_content(this->_client.get_map_content("Payload"));
		if (this->_client.get_content_length() > (long unsigned int)this->_servers[index].max_body_size)
		{
			std::cerr << "Payload too large on port: " << this->_servers[index].port << std::endl;
			return ;
		}
	}
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