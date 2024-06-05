/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:05:03 by rferrero          #+#    #+#             */
/*   Updated: 2024/06/05 15:05:54 by rferrero         ###   ########.fr       */
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

Response::Response(int client_fd, t_server &server, std::string path_and_name, std::string method)
{
	size_t	start_file = path_and_name.find_last_of("/") + 1;

	this->status_code_mapper();
	this->response.client = client_fd;
	this->response.server = server;
	this->response.method = method;
	this->response.path = path_and_name.substr(0, start_file);
	this->response.filename = path_and_name.substr(start_file);
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
	_check_directory_location();
	_check_allowed_methods();
	_check_file_location();
	if (this->_status_code != "200")
	{
		std::string		save_code = this->_status_code;
		std::string		save_msg = this->_status_msg;
		while (this->_status_code != "200")
			_check_errors_location_file();
		this->_status_code = save_code;
		this->_status_msg = save_msg;
	}
	set_file((this->response.server.root + this->response.path), this->response.filename);
	_make_response();
	_send_response();
	return ;
}

void	Response::_check_directory_location(void)
{
	if (this->response.server.locations.find(this->response.path) == this->response.server.locations.end())
		status_code_distributor("404");
	return ;
}

void	Response::_check_allowed_methods(void)
{
	if (this->_status_code == "404")
		return ;
	else if (find(this->response.server.locations.find(this->response.path)->second.methods.begin(), this->response.server.locations.find(this->response.path)->second.methods.end(), this->response.method) == this->response.server.locations.find(this->response.path)->second.methods.end())
		status_code_distributor("405");
	return ;
}

void	Response::_check_file_location(void)
{
	std::string		full_path = (this->response.server.root + this->response.path + this->response.filename);
	std::ifstream	file(full_path.c_str());
	struct stat		info;

	if (stat(full_path.c_str(), &info) != 0 || !S_ISREG(info.st_mode))
		status_code_distributor("404");
	else if (!file.is_open() || (file.peek() == std::ifstream::traits_type::eof()))
		status_code_distributor("302");
	else
		status_code_distributor("200");
	file.close();
	return ;
}

void	Response::_check_errors_location_file(void)
{
	this->response.path = "/errors/";
	this->response.filename = (this->_status_code + ".html");
	_check_file_location();
	return ;
}

void	Response::_make_response(void)
{
	std::string			file_content;
	std::ostringstream	handler;

	file_content = this->get_content();
	handler << file_content.size();

	this->response.header = "HTTP/1.1 ";
	this->response.header += this->_status_code + " ";
	this->response.header += this->_status_msg;
	this->response.header += "\nContent-Type:";
	this->response.header += this->get_content_type();
	this->response.header += "\nContent-Length: ";
	this->response.header += handler.str();
	this->response.header += " \n\n";
	this->response.body = this->response.header;
	this->response.body += file_content;

	return ;
}

void	Response::_send_response(void)
{
	write(this->response.client, this->response.body.c_str(), this->response.body.size());
	return ;
}
