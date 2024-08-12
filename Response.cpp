/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:05:03 by rferrero          #+#    #+#             */
/*   Updated: 2024/08/12 15:01:21 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "CGI.hpp"

/*
** ----------------------------- STATIC FUNCTIONS -----------------------------
*/

static std::string _display_time(void)
{
	std::time_t		currenttime;
	struct tm		*timeinfo;
	char			buffer[80];

	std::time(&currenttime);
	timeinfo = std::gmtime(&currenttime);

	std::strftime(buffer, 80, "%a, %d, %b, %Y %H:%M:%S GMT", timeinfo);
	return (std::string(buffer));
}

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

void	Response::_run_CGI(void)
{

	t_cgi	res_cgi;
	
	res_cgi.method = "create";
	res_cgi.cgi = "/usr/bin/python3";
	res_cgi.data_base = "./www/test/csv/tree.csv";
	res_cgi.script_file = (this->_response.server.root + this->_response.path + this->_response.name);
	res_cgi.data = "{\"name\":\"Fernando\",\"age\":\"25\"}";

	CGI			*cgi = new CGI(res_cgi);

	cgi->run_cgi();

	this->set_content(cgi->get_cgi_ret());
	delete cgi;
	return ;
}

void	Response::run_response(void)
{
	status_code_distributor("200");
	std::cout << "Antes" << this->_response.server.root << std::endl;
	std::cout << "Antes" << this->_response.path << std::endl;
	std::cout << "Antes" << this->_response.name << std::endl;
	_change_paths_for_redirections();
	std::cout << "Depois" << this->_response.server.root << std::endl;
	std::cout << "Depois" << this->_response.path << std::endl;
	std::cout << "Depois" << this->_response.name << std::endl;

	if (_check_for_cgi() == true)
	{
		_run_CGI();
		if (this->get_content() == "404\n")
		{
			status_code_distributor("404");
			this->_response.path = "/errors/";
			this->_response.name = "404.html";
			_file_validation();
		}
	}
	else
	{
		_check_allowed_methods();
		if (this->_status_code == "405")
		{
			this->_response.path = "/errors/";
			this->_response.name = (std::string)this->_status_code + ".html";
			_file_validation();
		}
		else if (this->_response.method == "GET")
		{
			if (this->_response.name.empty())
				_directory_validation();
			else
				_file_validation();
		}
		else if (this->_response.method == "DELETE")
		{
			if (this->_response.name.empty())
				status_code_distributor("400");
			else
			{
				std::string full_path = (this->_response.server.root + this->_response.path + this->_response.name);
				if (!remove(full_path.c_str()))
					status_code_distributor("202");	
				else
					status_code_distributor("404");
			}
			this->_response.path = "/errors/";
			this->_response.name = (std::string)this->_status_code + ".html";
			_file_validation();
		}
		else if (this->_response.method == "POST")
		{
			if (this->_response.name.empty())
				status_code_distributor("403");
			else
				status_code_distributor("200");
			this->_response.path = "/errors/";
			this->_response.name = (std::string)this->_status_code + ".html";
			this->_file_validation();
		}
	}

	if (this->_status_code[0] == '4')
    {
		_check_redirect_4xx_errors();
    }
	else
		this->_response_maker();
	this->_send_response();
	return ;
}

void	Response::_change_paths_for_redirections(void)
{
	std::vector<t_redirect>::iterator	i;

	for (i = this->_response.server.redirects.begin(); i != this->_response.server.redirects.end(); i++)
	{
		if (this->_response.path == i->path)
			this->_response.path = i->redir;
	}
	return ;
}

bool	Response::_check_for_cgi(void)
{
	if (this->_response.name.length() > 3 && this->_response.name.rfind(".py") == this->_response.name.length() - 3)
		return (true);
	return (false);
}

void	Response::_file_validation(void)
{
	_check_file_location();
	_check_max_body_size();
	if (this->_status_code != "200")
		_check_errors_location_file();
	else
		set_file((this->_response.server.root + this->_response.path + this->_response.name));
	return ;
}

void	Response::_directory_validation(void)
{
	_check_directory_location();
	_check_directory_autoindex();
	if (this->_status_code == "302")
		set_file((this->_response.server.root + this->_response.path + this->_response.server.locations[this->_response.path].default_file));
	else if (this->_status_code != "200")
		_check_errors_location_file();
	else
		_set_dir_content();
	return ;
}

void	Response::_check_allowed_methods(void)
{
	std::map<std::string, t_location>::iterator	it = this->_response.server.locations.find(this->_response.path);
	if (it == this->_response.server.locations.end())
		return ;
	else
	{
		std::vector<std::string>	&allowed_methods = it->second.methods;
		if (std::find(allowed_methods.begin(), allowed_methods.end(), this->_response.method) == allowed_methods.end())
			status_code_distributor("405");
	}
	return ;
}

void	Response::_check_directory_location(void)
{
	if (this->_status_code == "405")
		return ;
	else if (this->_response.server.locations.find(this->_response.path) == this->_response.server.locations.end())
		status_code_distributor("404");
	return ;
}

void	Response::_check_directory_autoindex(void)
{
	if (this->_status_code == "404" || this->_status_code == "405")
		return ;

	std::map<std::string, t_location>::iterator	it = this->_response.server.locations.begin();
	for (; it != this->_response.server.locations.end(); it++)
	{
		if (it->first == this->_response.path)
		{
			if (it->second.directory == false)
			{
				if (it->second.default_file.empty())
					status_code_distributor("404");
				else
				{
					status_code_distributor("302");
					this->_response.name = it->second.default_file;
				}
				return ;
			}
			else
				return ;
		}
	}
	return ;
}

void	Response::_check_errors_location_file(void)
{
	std::map<std::string, t_location>::iterator	it;

	it = this->_response.server.locations.find("/errors/");
	if (it == this->_response.server.locations.end())
	{
		std::string	new_content = ("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<title>" + (std::string)this->_status_code + " " + (std::string)this->_status_msg + "</title>\n\t\t</head>\n\t<body>\n\t\t<h1 style=\"background-color: black; text-align: center; color: white;\"></h1>\n\t\t<p style=\"font-size: 8;\">" + (std::string)this->_status_code + " " + (std::string)this->_status_msg + "</p>\n\t</body>\n</html>\n");
		this->set_content(new_content);
	}
	else
	{
		this->_response.path = "/errors/";
		this->_response.name.clear();
		this->_response.name = ((std::string)this->_status_code + ".html");
		this->set_file((this->_response.server.root + this->_response.path), this->_response.name);
	}
	
	return ;
}

void	Response::_check_file_location(void)
{
	std::string		full_path = (this->_response.server.root + this->_response.path + this->_response.name);
	std::ifstream	file(full_path.c_str());
	struct stat		info;

	std::cout << full_path << std::endl;

	if (this->_status_code == "404" || this->_status_code == "405")
		return ;
	if (stat(full_path.c_str(), &info) != 0 || !S_ISREG(info.st_mode))
		status_code_distributor("404");
	else if (!file.is_open())
		status_code_distributor("302");
	file.close();
	this->_check_directory_location();
	return ;
}

void	Response::_check_max_body_size(void)
{
	if (this->_status_code == "404" || this->_status_code == "405")
		return ;
	if (this->get_content_length() > static_cast<size_t>(this->_response.server.max_body_size))
		status_code_distributor("413");
	return ;
}

void	Response::_check_redirect_4xx_errors(void)
{
	std::map<std::string, t_location>::iterator	it;

	it = this->_response.server.locations.find("/errors/");
	if (it == this->_response.server.locations.end())
	{
		this->_response_maker();
		return ;
	}
	
	std::stringstream	port;
	port << this->_response.server.port;
	
	this->_response.header = "HTTP/1.1 ";
	this->_response.header += "301 Moved Permanently ";
	this->_response.header += "\nLocation: ";
	this->_response.header += "http://" + this->_response.server.server_name + ":" + port.str() + "/errors/" + this->_status_code + ".html";
	this->_response.header += "\nContent-Type: ";
	this->_response.header += this->get_content_type();
	this->_response.header += "\nContent-Length: ";
	this->_response.header += "31";
	this->_response.header += "\nDate: ";
	this->_response.header += _display_time();
	this->_response.header += "\n\n";
	
	this->_response.body = this->_response.header;
	this->_response.body += "<body>Moved Permanently</body>";
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
		if (entry->d_type == DT_REG || entry->d_type == DT_DIR)
		{
			std::string	filename = entry->d_name;

			files_list += "<a href=" + this->_response.path + filename + ">" + filename + "</a><br/>\n";
		}
	}
	closedir(dir);
	return (files_list);
}

void	Response::_set_dir_content(void)
{
	std::string	full_html = ("<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<meta charset=\"UTF-8\">\n\t\t<title>Files in Directory</title>\n\t\t</head>\n\t<body>\n\t\t");
	std::string	files = _get_dir_files();
	full_html += files;
	full_html += ("\n\t</body>\n</html>\n");
	this->set_content(full_html);
	return ;
}

void	Response::_response_maker(void)
{
	std::ostringstream	handler;

	std::string			file_content;
	file_content = this->get_content();

	handler << file_content.size();

	this->_response.header = "HTTP/1.1 ";
	this->_response.header += this->_status_code + " ";
	this->_response.header += this->_status_msg;
	this->_response.header += "\nContent-Type:";
	this->_response.header += this->get_content_type();
	this->_response.header += "\nContent-Length: ";
	this->_response.header += handler.str();
	this->_response.header += "\nDate: ";
	this->_response.header += _display_time();
	this->_response.header += "\n\n";
	
	this->_response.body = this->_response.header;
	this->_response.body += file_content;
	return ;
}

void	Response::_send_response(void)
{
	write(this->_response.client, this->_response.body.c_str(), this->_response.body.size());
	return ;
}
