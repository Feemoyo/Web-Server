/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:05:03 by rferrero          #+#    #+#             */
/*   Updated: 2024/07/12 18:37:15 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
 ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response(void)
{
	return ;
}

//TODO: pegar o max_body_size para validar com o content_length do payload
Response::Response(int client_fd, t_server &server, std::string path_and_name, std::string method)
{
	size_t	start_file = path_and_name.find_last_of("/") + 1;
	this->status_code_mapper();
	this->_response.client = client_fd;
	this->_response.server = server;
	this->_response.method = method;
	this->_response.path = path_and_name.substr(0, start_file);
	this->_response.filename = path_and_name.substr(start_file);
	return ;
}

/*
 -------------------------------- DESTRUCTOR --------------------------------
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
	this->_status_code = "200";
	_check_directory_location();
	_check_allowed_methods();
	_check_file_location();
	_check_max_body_size();
	if (this->_status_code != "200")
		_check_errors_location_file();
	else
		set_file((this->_response.server.root + this->_response.path), this->_response.filename);
	_make_response();
	_send_response();
	return ;
}

void	Response::_check_directory_location(void)
{
	if (this->_response.server.locations.find(this->_response.path) == this->_response.server.locations.end())
		status_code_distributor("404");
	return ;
}

void	Response::_check_allowed_methods(void)
{
	if (this->_status_code == "404")
		return ;
	else if (find(this->_response.server.locations.find(this->_response.path)->second.methods.begin(), \
				this->_response.server.locations.find(this->_response.path)->second.methods.end(), \
				this->_response.method) == this->_response.server.locations.find(this->_response.path)->second.methods.end())
		status_code_distributor("405");
	return ;
}

void	Response::_check_file_location(void)
{
	std::string		full_path = (this->_response.server.root + this->_response.path + this->_response.filename);
	std::ifstream	file(full_path.c_str());
	struct stat		info;

	if (stat(full_path.c_str(), &info) != 0 || !S_ISREG(info.st_mode))
		status_code_distributor("404");
	else if (!file.is_open() || (file.peek() == std::ifstream::traits_type::eof()))
		status_code_distributor("302");
	file.close();
	return ;
}

void	Response::_check_errors_location_file(void)
{
	std::map<std::string, t_location>::iterator	it;

	it = this->_response.server.locations.find("/errors/");
	if (it == this->_response.server.locations.end())
	{
		std::string	new_content = ("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<title>" + (std::string)this->_status_code + " " + (std::string)this->_status_msg + "</title>\n\t\t<link rel=\"icon\" href=\"/imgs/icon.svg\" type=\"image/png\">\n\t</head>\n\t<body>\n\t\t<h1 style=\"background-color: black; text-align: center; color: white;\"></h1>\n\t\t<p style=\"font-size: 8;\">" + (std::string)this->_status_code + " " + (std::string)this->_status_msg + "</p>\n\t</body>\n</html>\n");
		this->set_content(new_content);
	}
	else
	{
		this->_response.path = "/errors/";
		this->_response.filename = ((std::string)this->_status_code + ".html");
		this->set_file((this->_response.server.root + this->_response.path), this->_response.filename);
	}
	return ;
}

void	Response::_check_max_body_size(void)
{
	if (this->get_content_length() > static_cast<size_t>(this->_response.server.max_body_size))
		status_code_distributor("413");
	return ;
}

void	Response::_make_response(void)
{
	std::string			file_content;
	std::ostringstream	handler;

	file_content = this->get_content();
	handler << file_content.size();

	this->_response.header = "HTTP/1.1 ";
	this->_response.header += this->_status_code + " ";
	this->_response.header += this->_status_msg;
	this->_response.header += "\nContent-Type:";
	this->_response.header += this->get_content_type();
	this->_response.header += "\nContent-Length: ";
	this->_response.header += handler.str();
	this->_response.header += " \n\n";
	this->_response.body = this->_response.header;
	this->_response.body += file_content;
	return ;
}

void	Response::_send_response(void)
{
	write(this->_response.client, this->_response.body.c_str(), this->_response.body.size());
	return ;
}
