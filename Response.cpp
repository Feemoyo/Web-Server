/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:05:03 by rferrero          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/07/12 17:34:26 by rferrero         ###   ########.fr       */
=======
/*   Updated: 2024/07/11 18:25:07 by rferrero         ###   ########.fr       */
>>>>>>> 4f669df38635351ec010d40c431bc52ea65aea74
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
<<<<<<< HEAD
#include <fstream>
=======
>>>>>>> 4f669df38635351ec010d40c431bc52ea65aea74
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
	size_t	start = path_and_name.find_last_of("/") + 1;

	this->status_code_mapper();
	this->_response.client = client_fd;
	this->_response.server = server;
	this->_response.method = method;
	this->_response.path = path_and_name.substr(0, start);
	this->_response.name = path_and_name.substr(start);
	if (this->_response.name.find(".") == std::string::npos)
	{
		this->_response.is_file = false;
		std::cout << "NOT A FILE" << "\n";
	}
	else
	{
		this->_response.is_file = true;
		std::cout << "FILE" << "\n";
	}
	this->_status_code = "200";

	std::cout << "INPUT: " << path_and_name << "\n";

	std::cout << "Path: " << this->_response.path << "\n";
	std::cout << "Name: " << this->_response.name << "\n";

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
<<<<<<< HEAD
	status_code_distributor("200");
	if (this->_response.filename.size() == 0)
		_directory_validation();
	else
		_file_validation();
	_make_response();
	_send_response();
	return ;
}

void	Response::_file_validation(void)
{
	_check_file_location();
	_check_allowed_methods();
	if (this->_status_code != "200")
		_check_errors_location_file();
	else
		set_file((this->_response.server.root + this->_response.path), this->_response.filename);
	return ;
}

void	Response::_directory_validation(void)
{
	_check_directory_location();
	_check_allowed_methods();
	_check_directory_autoindex();
	if (this->_status_code != "200")
		_check_errors_location_file();
	else
		_set_dir_content();
=======
	_check_directory_location();
	_check_allowed_methods();
	if (this->_response.is_file == true)
		_check_file_location();
	if (this->_status_code != "200")
		_check_errors_location_file();
	else
	{
		if (this->_response.is_file == true)
			set_file((this->_response.server.root + this->_response.path), this->_response.name);
	}
	_make_response();
	_send_response();
>>>>>>> 4f669df38635351ec010d40c431bc52ea65aea74
	return ;
}

void	Response::_check_directory_location(void)
{
	if (this->_response.is_file == true)
	{
		if (this->_response.server.locations.find(this->_response.path) == this->_response.server.locations.end())
			status_code_distributor("404");
	}
		
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
void	Response::_check_directory_autoindex(void)
{
	if (this->_status_code == "404")
		return ;
	else if (this->_response.server.locations.find(this->_response.path)->second.directory != true)
		status_code_distributor("403");
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

void	Response::_check_file_location(void)
{
	std::string		full_path = (this->_response.server.root + this->_response.path + this->_response.name);
	std::ifstream	file(full_path.c_str());
	struct stat		info;

	if (stat(full_path.c_str(), &info) != 0 || !S_ISREG(info.st_mode))
		status_code_distributor("404");
	else if (!file.is_open() || (file.peek() == std::ifstream::traits_type::eof()))
		status_code_distributor("302");
	file.close();
	return ;
}

std::string	Response::_get_dir_files(void)
{
	std::string	files_list;
	DIR			*dir = opendir((this->_response.server.root + this->_response.path).c_str());

	if (dir == NULL)
	{
		std::cerr << "Directory " << (this->_response.server.root + this->_response.path) << " opening fail" << "\n";
		return ("");
	}
	struct dirent	*entry;
	while ((entry = readdir(dir)) != NULL)
	{
<<<<<<< HEAD
		if (entry->d_type == DT_REG || entry->d_type == DT_DIR)
		{
			std::string	filename = entry->d_name;
			std::string	filepath = (this->_response.server.root + this->_response.path) + "/" + filename;

			files_list += "<a href=\"" + filepath + "\">" + filename + "</a><br/>\n";
		}
=======
		this->_response.path = "/errors/";
		this->_response.name = ((std::string)this->_status_code + ".html");
		this->set_file((this->_response.server.root + this->_response.path), this->_response.name);
>>>>>>> 4f669df38635351ec010d40c431bc52ea65aea74
	}
	closedir(dir);
	return (files_list);
}

void	Response::_set_dir_content(void)
{
	std::string	full_html = ("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<title>Files in Directory</title>\n\t\t<link rel=\"icon\" href=\"/imgs/icon.svg\" type=\"image/png\">\n\t</head>\n\t<body>\n\t\t");
	std::string	files = _get_dir_files();
	full_html += files;
	full_html += ("\n\t</body>\n</html>\n");
	this->set_content(full_html);
	return ;
}

void	Response::_set_dir_file(std::string &full_dir_path)
{
	std::string		files_list;
	struct dirent*	entry;
	DIR				*dir = opendir(full_dir_path.c_str());

	if (dir == NULL)
	{
		files_list = ("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<title>404 Not Found Dir</title>\n\t\t<link rel=\"icon\" href=\"/imgs/icon.svg\" type=\"image/png\">\n\t</head>\n\t<body>\n\t\t<h1 style=\"background-color: black; text-align: center; color: white;\"></h1>\n\t\t<p style=\"font-size: 8;\">404 Not Found Dir</p>\n\t</body>\n</html>\n");
		this->set_content(files_list);
		return ;
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_REG)
		{
			std::string	filename = entry->d_name;
			std::string	filepath = full_dir_path + "/" + filename;
			files_list += "<a href=\"" + filepath + "\">" + filename + "</a><br/>\n";
		}
	}
	closedir(dir);
	
	std::string	full_dir_html = "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<title>Directory Files</title>\n\t</head>\n\t<body>\n\t\t";
	full_dir_html += files_list;
	full_dir_html += "\n\t</body>\n</html>\n";
	this->set_content(full_dir_html);
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
