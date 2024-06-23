/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:05:03 by rferrero          #+#    #+#             */
/*   Updated: 2024/06/23 00:10:38 by fmoreira         ###   ########.fr       */
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
	//TODO: mudar para boleanos e mudar o status code dinamicamente
	_check_directory_location();
	_check_allowed_methods();
	_check_file_location();
	if (this->_status_code != "200")
		_check_errors_location_file();
	set_file((this->_response.server.root + this->_response.path), this->_response.filename);
	// std::cout << (this->_response.server.root + this->_response.path) << this->_response.filename << "\n";
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
	else
	{
		std::map<std::string, t_location>::iterator i = this->_response.server.locations.find(this->_response.path);
		if (i != this->_response.server.locations.end())
		{
			if (find(i->second.methods.begin(), i->second.methods.end(), this->_response.method) == i->second.methods.end())
				status_code_distributor("405");
		}	
	}
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
	//TODO: resolver os status code
	else
		status_code_distributor("200");
	file.close();
	return ;
}

void	Response::_check_errors_location_file(void)
{
	this->_response.path = "/errors/";
	this->_response.filename = (this->_status_code + ".html");
	_check_file_location();
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
