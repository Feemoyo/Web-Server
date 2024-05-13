/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:59 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/13 15:34:21 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ToolKit.hpp"
#include "Response.hpp"

Response::Response(void)
{
	this->_response = "";
	this->set_file("./www/index.html");
	return ;
}

Response::Response(int client_socket)
{
	this->_client_socket = client_socket;
	this->_response = "";
	this->set_file("./www/index.html");
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

	file_content = this->get_content();
	handler << file_content.size();
	
	//TODO: o Content-Type tem que ser dinamico e pode ser encontrado no request
	this->_header = "HTTP/1.1 ";
	this->_header += this->_status_code;
	this->_header += "\nContent-Type: */*\nContent-Length: ";
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

