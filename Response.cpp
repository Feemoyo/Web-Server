/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:59 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/23 15:34:29 by fmoreira         ###   ########.fr       */
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
:_client_fd(client_fd), _server(server), _method(method)
{ 
	this->status_code_mapper();
	size_t	start_file = path_and_name.find_last_of("/") + 1;
	
	this->_path = path_and_name.substr(0, start_file);
	this->_filename = path_and_name.substr(start_file);
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

void    Response::_make_response(void)
{
    std::string            file_content;
    std::ostringstream    handler;

    file_content = this->get_content();
    handler << file_content.size();

    //TODO: verificar se este bloco de codigo esta otimizado
    this->_header = "HTTP/1.1 ";
    this->_header += this->_status_code + " ";
    this->_header += this->_status_msg;
    this->_header += "\nContent-Type:";
    this->_header += this->get_content_type();
    this->_header += "\nContent-Length: ";
    this->_header += handler.str();
    this->_header += " \n\n";
    this->_response = this->_header;
    this->_response += file_content;

    return ;
}

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
	set_file((this->_server.root + this->_path), this->_filename);
	_make_response();
	_send_response();
	return ;
}

void	Response::_check_directory_location(void)
{
	if (this->_server.locations.find(this->_path) == this->_server.locations.end())
	{
		std::cout << "check_directory_location: 404" << std::endl;
		status_code_distributor("404");
	}
	return ;
}

void	Response::_check_allowed_methods(void)
{
	if (find(this->_server.locations.find(this->_path)->second.methods.begin(), this->_server.locations.find(this->_path)->second.methods.end(), this->_method) == this->_server.locations.find(this->_path)->second.methods.end())
	{
		status_code_distributor("405");
	}
	return ;
}

void	Response::_check_file_location(void)
{
	std::string		full_path = (this->_server.root + this->_path + this->_filename);
	std::ifstream	file(full_path.c_str());
	struct stat		info;

	if (stat(full_path.c_str(), &info) != 0 || !S_ISREG(info.st_mode))
	{
		//TODO: o teste de abrir um arquivo vazio esta caindo nesta condição e retornando 404 no lugar de 
		std::cout << "check_file_location: 404" << std::endl;
		status_code_distributor("404");
	}
	else if (!file.is_open() || (file.peek() == std::ifstream::traits_type::eof()))
	{
		status_code_distributor("302");
	}
	else
	{
		status_code_distributor("200");
	}
	file.close();
	return ;
}

void	Response::_check_errors_location_file(void)
{
	this->_path = "/errors/";
	this->_filename = (this->_status_code + ".html");
	_check_file_location();
	return ;
}

void	Response::_send_response(void)
{
	write(this->_client_fd, this->_response.c_str(), this->_response.size());
	return ;
}
