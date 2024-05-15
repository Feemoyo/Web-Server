/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:59 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/15 16:34:02 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response(void)
{
	this->_response = "";
	this->set_file("./www/index.html");
	return ;
}

Response::Response(int client_fd, std::string root)
{
	this->_client_fd = client_fd;
	this->_response = "";
	this->set_file("./www/index.html");
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response(void)
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Response::_make_response(void)
{
	std::string			file_content;
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
	write(this->_client_fd, this->_response.c_str(), this->_response.size());
}

/*
** --------------------------------- SETTERS ---------------------------------
*/

void	Response::set_fd(int fd)
{
	this->_client_fd = fd;
	return ;
}
