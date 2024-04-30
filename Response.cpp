/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:59 by fmoreira          #+#    #+#             */
/*   Updated: 2024/04/30 12:50:36 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void)
{
	this->_response = "";
	this->_file_path = "./www/index.html";
	return ;
}

Response::Response(int client_socket)
{
	this->_client_socket = client_socket;
	this->_response = "";
	this->_file_path = "./www/index.html";
	return ;
}

Response::~Response(void)
{
	return ;
}

void	Response::set_socket(int socket)
{
	this->_client_socket = socket;
	return ;
}

void	Response::_make_response(void)
{
	std::string	file_content;
	std::ostringstream	handler;

	file_content = this->read_file();
	handler << file_content.size();
	
	this->_header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	this->_header += handler.str();
	this->_header += " \n\n";
	this->_response = this->_header;
	this->_response += file_content;

	return ;
}

void	Response::send_response(void)
{
	this->_make_response();
	write(this->_client_socket, this->_response.c_str(), this->_response.size());
}

std::string	Response::read_file(void) const
{
	std::ifstream	file;
	std::string		line;
	std::string		file_content;

	file.open(this->_file_path.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error opening file" << std::endl;
		return ("");
	}
	while (std::getline(file, line))
	{
		file_content += line;
	}
	file.close();
	return (file_content);
}