/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:32:11 by fmoreira          #+#    #+#             */
/*   Updated: 2024/08/13 21:26:32 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ----------------------------- STATIC FUNCTIONS -----------------------------
*/

size_t	str_to_size_t(std::string str)
{
	size_t		size;

	std::istringstream(str) >> size;
	return (size);
}

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

bool	Client::set_buffer(std::vector<char> buffer, bool &payload) 
{
	std::string 		str(buffer.begin(), buffer.end());
	std::istringstream	stream(str);
	std::string			line;

	if (!payload)
	{
		this->_request_header(stream);
		payload = true;
	}
	if (this->_buffer_map["Request"].find("POST") != std::string::npos)
	{
		if (this->_map_finder("Content-Type", "", ";") == ("multipart/form-data"))
		{
			while (std::getline(stream, line, '\0'))
			{
				this->_buffer_map["Payload"] += line.substr(0, line.find('\0'));
			}
		}
		else
		{	
			while (std::getline(stream, line))
			{
				this->_buffer_map["Payload"] += line.substr(0, line.find('\0'));
			}
		}
	}
	else
		payload = false;
	return (payload);
}

void	Client::_request_header(std::istringstream &stream)
{
	std::string line;

	std::getline(stream, line);
	this->_buffer_map["Request"] = line;
	this->_buffer_map["Query"] = this->_map_finder("Request", "?", "HTTP");

	if (this->_buffer_map["Query"].size() > 1)
	{
		line.clear();
		line = this->_buffer_map["Query"].substr(1);
		this->_buffer_map["Query"] = line;
		line.clear();
	}

	while (std::getline(stream, line))
	{
		if (line == "\r")
			return ;
		std::size_t first_space = line.find(':');
		if (first_space != std::string::npos)
		{
			std::string key = line.substr(0, first_space);
			std::string value = line.substr(first_space + 2);
			this->_buffer_map[key] = value;
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Client::get_method(void)
{
	std::string map_method = this->_map_finder("Request", "", " ");

	for (size_t i = 0; i < map_method.size(); i++)
		map_method[i] = std::toupper(map_method[i]);
	
	return (map_method);
}

std::string	Client::get_path(void)
{
	std::string path = this->_map_finder("Request", "/", " ");
	std::string query = this->_buffer_map["Query"];

	if (path.find("?") != std::string::npos)
		path = path.substr(0, path.find("?"));

	if (std::strchr(path.c_str(), '.') == NULL)
	{
		this->set_content_type("text/html");
		if (path.find_last_of("/") == path.size() - 1)
			return (path);
		else
			return (path + "/");
	}
	else
		this->format_content_type();
	return (path);
}

std::string	Client::get_map_content(std::string ref)
{
	return (this->_buffer_map[ref]);
}

/*
** --------------------------------- SETTERS ---------------------------------
*/

void	Client::format_content_type(void)
{
	std::string content_type = this->_map_finder("Request", ".", " ");
	std::string aux = content_type.substr(1);
	aux = this->_mime.get_mime(aux);
	this->set_content_type(aux);
	return ;
}

void		Client::set_body_size(void)
{
	if (!this->_buffer_map["Payload"].empty() )
		this->set_content_length(this->_buffer_map["Payload"].size());
	return ;
}

/*
** --------------------------------- UTILITIES ---------------------------------
*/

std::string	Client::_map_finder(std::string key, std::string value1, std::string value2)
{
	std::size_t auxFindGET1 = this->_buffer_map[key].find(value1, 0);
	std::size_t auxFindGET2 = this->_buffer_map[key].find(value2, auxFindGET1);

	if(auxFindGET1 == std::string::npos || auxFindGET2 == std::string::npos)
		return ("");

	if (auxFindGET2 == auxFindGET1)
		return (this->_buffer_map[key].substr(auxFindGET1));

	return(this->_buffer_map[key].substr(auxFindGET1, auxFindGET2 - auxFindGET1));
}

void	Client::clear_buffer(void)
{
	this->_buffer_map.clear();
	return ;
}

void	Client::clear_body_size(void)
{
	this->set_content_length(0);
	return ;
}

/*
** ------------------- TO DELETE AT END OF THE PROJECT ----------------------
*/

void	Client::print_map(void)
{
	std::map<std::string, std::string>::iterator it;

	for (it = this->_buffer_map.begin(); it != this->_buffer_map.end(); ++it)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
	return ;
}
