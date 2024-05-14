/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 23:58:36 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/14 02:15:27 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerHandler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ServerHandler::ServerHandler(void)
{
	return ;
}

ServerHandler::ServerHandler(std::vector<Server> &servers)
{
	this->_servers = servers;
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ServerHandler::~ServerHandler(void)
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	ServerHandler::run_servers(void)
{
	std::vector<struct pollfd>	fds(this->_servers.size());

	for(size_t i = 0; i < this->_servers.size(); i++)
	{
		fds[i].fd = this->_servers[i].get_server_socket();
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}

	while (true)
	{
		if (poll(fds.data(), fds.size(), -1) < 0)
		{
			std::cerr << "Poll fail" << std::endl;
			break ;
		}
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{

				std::cout << (fds[i].revents & POLLIN) << std::endl;
				this->_servers[i].accept_connection(fds[i]);
			}
		}
	}
	return ;
}
