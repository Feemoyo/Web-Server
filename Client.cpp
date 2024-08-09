/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:32:11 by fmoreira          #+#    #+#             */
/*   Updated: 2024/08/09 19:34:40 by fmoreira         ###   ########.fr       */
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

// static int	_from_hex(char c)
// {
// 	if (c >= '0' && c <= '9')
// 		return (c - '0');
// 	if (c >= 'a' && c <= 'f')
// 		return (c - 'a' + 10);
// 	if (c >= 'A' && c <= 'F')
// 		return (c - 'A' + 10);
// 	return (0);
// }

// static std::string	_url_decode(const std::string &str)
// {
// 	std::string	result;
// 	char		hex[3] = {0, 0, 0};

// 	result.reserve(str.length());
// 	for (std::size_t i = 0; i < str.length(); ++i)
// 	{
// 		if (str[i] == '%')
// 		{
// 			if (i + 2 < str.length())
// 			{
// 				hex[0] = str[i + 1];
// 				hex[1] = str[i + 2];
// 				if (hex[0] == '2' && hex[1] == '2')
// 					result += "\\\"";
// 				else if (hex[0] == '5' && hex[1] == 'C')
// 					result += "\\\\";
// 				else if (hex[0] == '0' && hex[1] == 'A')
// 					result += "\\n";
// 				else if (hex[0] == '0' && hex[1] == 'D')
// 					result += "\\r";
// 				else
// 					result += static_cast<char>(_from_hex(hex[0]) * 16 + _from_hex(hex[1]));
// 				i += 2;
// 			}
// 		}
// 		else if (str[i] == '+')
// 			result += ' ';
// 		else if (str[i] == '\n' || str[i] == '\r')
// 			i += 1;
// 		else
// 			result += str[i];
// 	}
// 	return (result);
// }

static std::string _to_hex(unsigned char c)
{
	const char hex_chars[] = "0123456789ABCDEF";
	std::string hex_str;

	int first_digit = c / 16;
    int second_digit = c % 16;

    hex_str += hex_chars[first_digit];
    hex_str += hex_chars[second_digit];

	return (hex_str);
}

static std::string _url_encode(const std::string &str)
{
	std::string result;
	result.reserve(str.length());

	for (std::size_t i = 0; i < str.length(); ++i)
	{
		char c = str[i];
		if (isalnum(c))
			result += c;
		else if (c == ' ')
			result += '+';
		else if (c == '\\')
			result += "%5C";
		else if (c == '\"')
			result += "%22";
		else if (c == '\n')
			result += "%0A";
		else if (c == '\r')
			result += "%0D";
		else
		{
			result += "%";
			result += _to_hex(static_cast<unsigned char>(c));
		}
	}

	return result;
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
			while (std::getline(stream, line))
			{
				this->_buffer_map["Payload"] += _url_encode(line.substr(0, line.find('\0')) + "\n");
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
	if (path == "/")
	{
		this->set_content_type("text/html");
		return ("/index.html");
	}
	else if (std::strchr(path.c_str(), '.') == NULL)
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
