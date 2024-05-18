/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:59 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/17 23:28:40 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response(void)
{
	return ;
}

Response::Response(int client_fd, t_server &server, std::string path_and_name, std::string method)
:_client_fd(client_fd), _server(server), _method(method)
{
	size_t	start_file = path_and_name.find_last_of("/") + 1;
	
	this->_path = path_and_name.substr(0, start_file);
	this->_filename = path_and_name.substr(start_file);
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

void	Response::run_response(void)
{
	_check_directory_location();
	_check_file_location();
	_check_file_empty();
	_check_allowed_methods();

	set_file((this->_server.root + this->_path), this->_filename);

	_make_response();
	_send_response();
	return ;
}

void	Response::_check_directory_location(void)
{
	if (this->_server.locations.find(this->_path) == this->_server.locations.end())
	{
		this->_path = "/errors/";
		this->_filename = this->_server.locations.find("/errors/")->second.default_file;
		this->_status_code = "404";
		this->_status_msg = "Not Found";
	}
	return ;
}

void	Response::_check_file_location(void)
{
	std::string	full_path = (this->_server.root + this->_path + this->_filename);
	struct stat	info;

	if (stat(full_path.c_str(), &info) != 0 || !S_ISREG(info.st_mode))
	{
		this->_path = "/errors/";
		this->_filename = this->_server.locations.find("/errors/")->second.default_file;
		this->_status_code = "404";
		this->_status_msg = "Not Found";
	}
	return ;
}

void	Response::_check_file_empty(void)
{
	std::string	full_path = (this->_server.root + this->_path + this->_filename);
	std::ifstream	file(full_path.c_str());

	if (!file.is_open() || (file.peek() == std::ifstream::traits_type::eof()))
	{
		this->_path = "/errors/";
		this->_filename = "204.html";
		this->_status_code = "302";
		this->_status_msg = "Found";
	}
	else
	{
		this->_status_code = "200";
		this->_status_msg = "OK";
	}
	file.close();
	return ;
}

void	Response::_check_allowed_methods(void)
{
	if (find(this->_server.locations.find(this->_path)->second.methods.begin(), this->_server.locations.find(this->_path)->second.methods.end(), this->_method) == this->_server.locations.find(this->_path)->second.methods.end())
	{
		this->_path = "/errors/";
		this->_filename = "405.html";
		this->_status_code = "405";
		this->_status_msg = "Method Not Allowed";
	}
	return ;
}

void	Response::_make_response(void)
{
	std::string			file_content;
	std::ostringstream	handler;

	file_content = this->get_content();
	handler << file_content.size();
	
	this->_header = "HTTP/1.1 ";
	this->_header += this->_status_code + " ";
	this->_header += this->_status_msg;
	this->_header += "\nContent-Type: */*\nContent-Length: ";
	this->_header += handler.str();
	this->_header += " \n\n";
	this->_response = this->_header;
	this->_response += file_content;

	return ;
}

void	Response::_send_response(void)
{
	write(this->_client_fd, this->_response.c_str(), this->_response.size());
	return ;
}
