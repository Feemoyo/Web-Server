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
	std::string content_type = this->_map_finder("Request", ".", " ");
	std::string accept = this->_buffer_map["Accept"];
	std::string aux = content_type.substr(1);
	aux = this->_mime.get_mime_image(aux);
	this->set_content_type(aux);
  return ;
}

bool	Client::set_buffer(std::vector<char> buffer, bool &payload) 
{
	std::string 		str(buffer.begin(), buffer.end());
	std::istringstream	stream(str);
	std::string 		line;
	std::string 		aux;

	if (!payload)
	{
		std::getline(stream, line);
		this->_buffer_map["Request"] = line;
	}
	while (std::getline(stream, line))
	{
		if (_buffer_map["Request"].find("POST") != std::string::npos && (line == "\r" || payload))
		{
			line.erase(std::remove_if(line.begin(), line.end(), static_cast<int(*)(int)>(&std::isspace)), line.end());
			this->_buffer_map["Payload"] += line.substr(0, line.find('\0'));
			payload = true;
		}
		else
		{
			std::size_t first_space = line.find(':');
			if (first_space != std::string::npos)
			{
				std::string key = line.substr(0, first_space);
				std::string value = line.substr(first_space + 2);
				this->_buffer_map[key] = value;
			}
		}
	}

	//TODO: transformar em função
	std::stringstream	toInt(this->_buffer_map["Content-Length"]);
	size_t 				content_length;
	toInt >> content_length;
	//

	// std::cout << "Content-Length: " << content_length << std::endl;
	// std::cout << "Payload: " << this->_buffer_map["Payload"].size() << std::endl;
	//TODO: explorar o motivo do payload ficar maior que o content-length

	if (content_length == (this->_buffer_map["Payload"].size()))
		payload = false;
	return (payload);
}

std::string	Client::get_path(void)
{
	std::string path = this->_map_finder("Request", "/", " ");
	if (path == "/")
	{
		this->set_content_type("text/html");
		return ("index.html");
	}
	else if (std::strchr(path.c_str(), '.') == NULL)
	{
		this->set_content_type("text/html");
		return (path + "index.html");
	}
	else
		this->format_content_type();
	return (path);
}

std::string	Client::get_method(void)
{
	return (this->_map_finder("Request", "", " "));
}

void	Client::clear_buffer(void)
{
	this->_buffer_map.clear();
	return ;
}

void	Client::print_map(void)
{
	std::map<std::string, std::string>::iterator it;

	for (it = this->_buffer_map.begin(); it != this->_buffer_map.end(); ++it)
		std::cout << it->first << ": " << it->second << "\n";;
	return ;
}