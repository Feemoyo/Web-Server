/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:59 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/01 20:28:51 by fmoreira         ###   ########.fr       */
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
	
	//TODO: o Content-Type tem que ser dinamico e pode ser encontrado no request
	this->_header = "HTTP/1.1 200 OK\nContent-Type: */*\nContent-Length: ";
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
	std::cout << "File path: " << this->_file_path << std::endl;
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

void	Response::set_file_path(std::string path)
{
	if (path == "/")
	{
		this->_file_path = "./www/index.html";
		return ;
	}
	this->_file_path = "." + path;
	return ;
}
