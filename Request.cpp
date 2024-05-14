/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:32:11 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/14 19:32:39 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Request::Request(void)
{
	this->_buffer_map.clear();

	return ;
}

Request::Request(char *buffer)
{
	this->set_buffer(buffer);
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Request::~Request(void)
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Request::set_buffer(char *buffer)
{
	std::istringstream stream(buffer);
	std::string line;

	this->_buffer_map.clear();
	std::getline(stream, line);
	this->_buffer_map["Request"] = line;
	while (std::getline(stream, line))
	{
		std::size_t first_space = line.find(':');
		if (first_space != std::string::npos)
		{
			std::string key = line.substr(0, first_space);
			std::string value = line.substr(first_space + 2);

			this->_buffer_map[key] = value;
		}
	}

	return ;
}

void	Request::print_map(void)
{
	std::map<std::string, std::string>::iterator it;
	for (it = this->_buffer_map.begin(); it != this->_buffer_map.end(); ++it)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}

	return ;
}

std::string	Request::get_path(void)
{
	std::size_t auxFindGET1 = this->_buffer_map["Request"].find("/", 0);
	std::size_t auxFindGET2 = this->_buffer_map["Request"].find(" ", auxFindGET1);
	std::string path = this->_buffer_map["Request"].substr(auxFindGET1, auxFindGET2 - auxFindGET1);

	if (!path.compare("/"))
		path = "/www/index.html";
	return (path);
}