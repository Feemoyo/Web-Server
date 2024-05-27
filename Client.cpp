/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:32:11 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/14 19:32:39 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(void)
{
	this->_buffer_map.clear();
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client(void)
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string	Client::_map_finder(std::string key, std::string value1, std::string value2)
{
	std::size_t auxFindGET1 = this->_buffer_map[key].find(value1, 0);
	std::size_t auxFindGET2 = this->_buffer_map[key].find(value2, auxFindGET1);

	return(this->_buffer_map[key].substr(auxFindGET1, auxFindGET2 - auxFindGET1));
}

void	Client::format_content_type(void)
{
	//GOHORSE
	std::string content_type = this->_map_finder("Request", ".", " ");
	//std::cout << "content_type: " << content_type << std::endl;
	std::string accept = this->_buffer_map["Accept"];
	//std::cout << "controle: " << accept << std::endl;
	// std::string aux = accept.substr(accept.find(content_type.substr(1)), accept.find(content_type) + content_type.size());
	std::string aux = content_type.substr(1);
	//std::cout << "aux: " << aux << std::endl;
	aux = this->_mime.get_mime_image(aux);
	this->set_content_type(aux);
  return ;
}

void	Client::set_buffer(char *buffer)
{
	std::istringstream	stream(buffer);
	std::string 		line;

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

void	Client::print_map(void)
{
	std::map<std::string, std::string>::iterator it;

	for (it = this->_buffer_map.begin(); it != this->_buffer_map.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	return ;
}

std::string	Client::get_path(void)
{
	std::string path = this->_map_finder("Request", "/", " ");
	//TODO: este /index.html deve ser algo padrao ou setado pelo .conf? GET /srthsryjsryj.html
	if (path == "/")
	{
		this->set_content_type("text/html");
		return ("/index.html");
	}
	//GOHORSE
	else
		this->format_content_type();
	return (path);
}