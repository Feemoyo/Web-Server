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

std::string	Client::_url_decode(const std::string &str)
{
	std::string result;
	char hex[3] = {0, 0, 0};

	result.reserve(str.length());
	for (std::size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '%')
		{
			if (i + 2 < str.length())
			{
				hex[0] = str[i + 1];
				hex[1] = str[i + 2];
				result += static_cast<char>(this->_from_hex(hex[0]) * 16 + this->_from_hex(hex[1]));
				i += 2;
			}
		}
		else if (str[i] == '+')
			result += ' ';
		else
			result += str[i];
	}
	return (result);
}

int	Client::_from_hex(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (0);
}

void	Client::format_content_type(void)
{
	std::string content_type = this->_map_finder("Request", ".", " ");
	std::string aux = content_type.substr(1);
	aux = this->_mime.get_mime(aux);
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
		if (this->str_to_size_t(this->_buffer_map["Content-Length"]) == (this->_buffer_map["Payload"].size()))
			break ;
	}
	// std::cout << "Payload: " << this->_buffer_map["Payload"] << "\n";
	if (this->_buffer_map["Payload"].empty() || this->str_to_size_t(this->_buffer_map["Content-Length"]) == (this->_buffer_map["Payload"].size()))
		payload = false;
	return (payload);
}

void		Client::set_body_size(void)
{
	if (!this->_buffer_map["Payload"].empty())
		this->set_content_length(this->_buffer_map["Payload"].size());
	return ;
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

std::string	Client::get_method(void)
{
	return (this->_map_finder("Request", "", " "));
}

void	Client::clear_buffer(void)
{
	this->_buffer_map.clear();
	return ;
}

void	Client::clear_body_size(void)
{
	this->set_content_length(0);
}

void	Client::decode_payload(void)
{
	// std::cout << "decode: " << this->_buffer_map["Payload"] << "\n";
	this->_buffer_map["Payload"] = this->_url_decode(this->_buffer_map["Payload"]);
	return ;
}

void	Client::save_output(void)
{
	std::ofstream		output;
	std::istringstream	ss(this->_buffer_map["Payload"]);
	std::string 		line;
	std::string			aux;
	// size_t				runner = 0;
	char				ch = '&';

	output.open("./www/temp/test/form/output.json");
	if (!output.is_open())
	{
		std::cerr << "Error: could not open output file\n";
		return ;
	}

	if (output.is_open())
	{
		output << "{\n";

		//COPY: set_buffer()
		std::map<std::string, std::string> json_map;
		while (std::getline(ss, line, ch))
		{
			std::size_t first_space = line.find('=');
			if (first_space != std::string::npos)
			{
				//TODO: decode payload
				this->_url_decode(line);
				std::string key = line.substr(0, first_space);
				std::string value = line.substr(first_space + 1);
				json_map[key] = value;
			}
		}



		//COPY: print_map()
		std::map<std::string, std::string>::iterator it;

		for (it = json_map.begin(); it != json_map.end(); ++it)
		{
			if (it == --json_map.end())
				output << "\t\"" <<it->first << "\": \"" << it->second << "\"\n";
			else
				output << "\t\"" <<it->first << "\": \"" << it->second << "\",\n";
		}

		output << "}\n";
	}
	
	output.close();
	return ;
}

void	Client::print_map(void)
{
	std::map<std::string, std::string>::iterator it;

	for (it = this->_buffer_map.begin(); it != this->_buffer_map.end(); ++it)
		std::cout << it->first << ": " << it->second << "\n";
	return ;
}