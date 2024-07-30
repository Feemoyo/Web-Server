/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Client.cpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: rferrero <rferrero@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/05/13 14:32:11 by fmoreira		  #+#	#+#			 */
/*   Updated: 2024/05/14 19:32:39 by rferrero		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Client.hpp"

/*
** ----------------------------- STATIC FUNCTIONS -----------------------------
*/

static int	_from_hex(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (0);
}

static std::string	_url_decode(const std::string &str)
{
	std::string	result;
	char		hex[3] = {0, 0, 0};

	result.reserve(str.length());
	for (std::size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '%')
		{
			if (i + 2 < str.length())
			{
				hex[0] = str[i + 1];
				hex[1] = str[i + 2];
				result += static_cast<char>(_from_hex(hex[0]) * 16 + _from_hex(hex[1]));
				i += 2;
			}
		}
		else if (str[i] == '+')
			result += ' ';
		else if (str[i] == '\n' || str[i] == '\r')
			i += 1;
		else
			result += str[i];
	}
	return (result);
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
	std::string 		line;
	std::string 		aux;

	if (!payload)
	{
		std::getline(stream, line);
		this->_buffer_map["Request"] = line;
		if (this->_buffer_map["Request"].find("POST") != std::string::npos)
			this->_buffer_map["Payload"] = "";
	}
	while (std::getline(stream, line))
	{
		if (line == "\r" || payload)
		{
			this->remove_white_spaces(line);
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
		if (!this->_buffer_map["Payload"].empty() && this->_buffer_map["Request"].find("POST") != std::string::npos && (this->str_to_size_t(this->_buffer_map["Content-Length"]) == (this->_buffer_map["Payload"].size())))
		{
			payload = true;
			break ;
		}
	}

	if ((this->_buffer_map["Payload"].empty() && this->_buffer_map["Request"].find("POST") == std::string::npos) || this->str_to_size_t(this->_buffer_map["Content-Length"]) == (this->_buffer_map["Payload"].size()))
		payload = false;

	return (payload);
}

void	Client::run_json(void)
{
	// std::string output_name;

	// output_name += this->_map_finder("Request", "/", ".");
	this->_buffer_map["Payload"] = _url_decode(this->_buffer_map["Payload"]);
	JSON	*json = new JSON(this->_buffer_map["Payload"], "./www/temp", "new.json");

	json->run();

	delete json;
	return ;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Client::get_method(void)
{
	return (this->_map_finder("Request", "", " "));
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
	if (!this->_buffer_map["Payload"].empty())
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
		std::cout << it->first << ": " << it->second << "\n";
	return ;
}
