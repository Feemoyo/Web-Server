/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/08/08 21:35:47 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
** ----------------------------- STATIC FUNCTIONS -----------------------------
*/

static void	_remove_comments(std::string &str)
{
	size_t	ref;

	ref = str.find('#');
	while (ref != std::string::npos)
	{
		size_t	end;

		end = str.find('\n', ref);
		str.erase(ref, end - ref);
		ref = str.find('#');
	}
	return ;
}

static int	_count_occurrences(const std::string &str, const char c)
{
	int i = 0;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
	    if (*it == c)
			i++;
	}
	return i;
}

static bool	_verify_brackets(std::string &str)
{
	if (_count_occurrences(str, '{') != _count_occurrences(str, '}'))
	{
		std::cerr << "Error .conf: Brackets are not balanced" << "\n";;
		return (false);
	}
	return (true);
}

static bool	_is_directory_visible(std::string str)
{
	return (str.compare("on"));
}

static void	_change_server_name(const std::string &name)
{
	std::ifstream	host_file("/etc/hosts");
	std::string		line;
	std::string		new_hosts;
	bool			name_exists = false;

	if (!host_file.is_open())
	{
		std::cerr << "Unable to change server: " << name << "\n";
		return ;
	}
	while (std::getline(host_file, line))
	{
		new_hosts += line + "\n";
		if (line.find("127.0.0.1") != std::string::npos)
		{
			if (line.find(name) != std::string::npos)
				name_exists = true;
		}
	}
	host_file.close();
	if (!name_exists)
	{
		new_hosts += "127.0.0.1";
		new_hosts += '\t';
		new_hosts += name;
		new_hosts += '\n';
	}
	std::ofstream	host_file_out("/etc/hosts");
	if (!host_file_out.is_open())
	{
		std::cerr << "Unable to write new server name" << "\n";
		return ;
	}
	host_file_out << new_hosts;
	host_file_out.close();
	return ;
}

std::string	_find_and_split(const std::string &content, size_t start, std::string first_lim, std::string second_lim)
{
	size_t		ref_end;

	start = content.find(first_lim, start) + first_lim.size();
	ref_end = content.find(second_lim, start);
	return (content.substr(start, ref_end - start));
}

bool	_check_string_in_string(const std::string &locations, std::string begin, std::string final, size_t start)
{
	start = locations.find(begin, start);
	size_t	end = locations.find(final, start);

	if (start == std::string::npos || end == std::string::npos || start > end)
		return (false);
	return (true);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Config(void)
{
	return ;
}

Config::Config(char *argv)
{
	this->_good_to_run = false;
	set_file("./", argv);
	_remove_comments(this->_content);
	this->remove_white_spaces(this->_content);
	if (_verify_brackets(this->_content) && _config_total_servers() && _config_servers())
		this->_good_to_run = true;
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Config::~Config(void)
{
	this->_servers.clear();
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

bool	Config::_config_total_servers(void)
{
	size_t	start = 0;
	size_t	end;

	while (start != std::string::npos)
	{
		start = this->_content.find("server{", start);
		end = this->_content.find("};", start);
		if (end != std::string::npos)
		{
			std::string ref;

			ref = this->_content.substr(start, end - start);
			this->_server_string.push_back(ref);
		}
		start = end;
	}
	if (this->_server_string.empty())
	{
		std::cerr << "Fail to find server configuration" << "\n";;
		return (false);
	}
	return (true);
}

bool	Config::_config_servers(void)
{
	for (size_t i = 0; i < this->_server_string.size(); i++)
	{
		t_server	server;

		if (_is_there_a_valid_port(this->_server_string[i], server.port) == false)
			return (false);
		_set_server_name(this->_server_string[i], server.server_name);
		_set_server_root(this->_server_string[i], server.root);
		_set_dir_visibility(this->_server_string[i], server.directory);
		_set_max_body_size(this->_server_string[i], server.max_body_size);

		if (!_config_locations(this->_server_string[i], server))
			return (false);
		this->_servers.push_back(server);
	}
	return (true);
}

bool	Config::_is_there_a_valid_port(std::string &serv, int &port)
{
	if (_check_string_in_string(serv, "listen", ";", 0) == false)
	{
		std::cerr << "Fail to find server port" << "\n";;
		return (false);
	}
	port = this->string_to_int(_find_and_split(serv, 0, "listen", ";"));
	if ((port != 80 && port < 1024) || port > 49151)
	{
		std::cerr << "Invalid server port" << "\n";;
		return (false);
	}
	return (true);
}

void	Config::_set_server_name(std::string &serv, std::string &name)
{

	if (_check_string_in_string(serv, "server_name", ";", 0) == true)
	{
		name = _find_and_split(serv, 0, "server_name", ";");
		_change_server_name(name);
	}
	else
		serv = "localhost";

	return ;
}

void	Config::_set_server_root(std::string &serv, std::string &root)
{
	if (_check_string_in_string(serv, "root", ";", 0) == true)
		root = ("./" + _find_and_split(serv, 0, "root", ";"));
	else
		root = "./";

	return ;
}

void	Config::_set_dir_visibility(std::string &serv, bool &dir)
{

	if (_check_string_in_string(serv, "directory", ";", 0) == false)
		dir = false;
	else
	{
		size_t	ref = serv.find("directory");
		size_t	end_1 = serv.find(";client_max_body_size", ref);
		if (end_1 != std::string::npos)
			dir = _is_directory_visible(_find_and_split(serv, ref, "directory", ";"));
		else
		{
			size_t	end_2 = serv.find(";location", ref);
			if (end_2 != std::string::npos)
				dir = _is_directory_visible(_find_and_split(serv, ref, "directory", ";"));
			else
			{
				size_t	end_3 = serv.find("};");
				if (end_3 != std::string::npos)
					dir = _is_directory_visible(_find_and_split(serv, ref, "directory", ";"));
			}
		}
	}
	return ;
}

void	Config::_set_max_body_size(std::string &serv, int &max_body_size)
{

	if (_check_string_in_string(serv, "client_max_body_size", ";", 0) == false)
		max_body_size = 200;
	else
	{
		size_t	ref = serv.find("client_max_body_size");
		size_t	end_2 = serv.find(";location", ref);
		if (end_2 != std::string::npos)
			max_body_size = this->string_to_int(_find_and_split(serv, ref, "client_max_body_size", ";"));
		else
		{
			size_t	end_3 = serv.find("};");
			if (end_3 != std::string::npos)
				max_body_size = this->string_to_int(_find_and_split(serv, ref, "client_max_body_size", ";"));
		}
	}
	if (max_body_size > 200000)
	{
		std::cerr << "Max body size of the server is too big" << "\n";
		std::cerr << "Changed max body size to 200000" << "\n";
		max_body_size = 200000;
	}
	return ;
}

bool	Config::_config_locations(std::string &serv, t_server &server)
{
	size_t		start = serv.find("location");

	if (start == std::string::npos)
		return (true);
	else
	{
		size_t		end_server = serv.find("}};", start);
		std::string	locations = serv.substr(start, end_server - start);
		size_t		end = locations.find(";}", start);

		start = 0;
		while (end != std::string::npos)
		{
			t_location	locat;

			start = locations.find("location", start);
			end = locations.find("{", start);
			if (start + strlen("location") >= end)
			{
				std::cerr << "Location need a path on server " << server.server_name << "\n";
				return (false);
			}
			locat.path = _find_and_split(locations, start, "location", "{");

			if (_check_for_redirection(locations, start) == true)
			{
				t_redirect	redirect;

				redirect.path = locat.path;
				redirect.redir = _find_and_split(locations, start, "return", ";");
				server.redirects.push_back(redirect);
				start = locations.find("}", end);
				end = locations.find("{", start);
				continue ;
			}

			if (_check_for_default_file(locations, start) == true)
				locat.default_file = _find_and_split(locations, start, "default", ";");
			else
				locat.default_file.clear();

			size_t	end2;

			end2 = locations.find(";", end);
			start = locations.find("directory", end);
			if (start == std::string::npos || end2 == std::string::npos || start - end2 > 2)
				locat.directory = false;
			else
				locat.directory = _is_directory_visible(_find_and_split(locations, start, "directory", ";"));

			start = locations.find("allowed_methods", end);
			if (start == std::string::npos)
				locat.methods.push_back("GET");
			else
			{
				start = start + strlen("allowed_methods");
				end = locations.find(";", start);
				std::string	ref = locations.substr(start, end - start);
				_config_methods(ref, locat);
			}
			start = locations.find("}", end);
			end = locations.find("{", start);
			server.locations.insert(std::pair<std::string, t_location>(locat.path, locat));
		}

	}
	return (true);
}

bool	Config::_check_for_default_file(std::string &locations, size_t ref)
{
	size_t	start = locations.find("default", ref);
	size_t	end = locations.find(";", ref);

	if (start == std::string::npos || end == std::string::npos || start > end)
		return (false);
	return (true);
}

bool	Config::_check_for_redirection(std::string &locations, size_t ref)
{
	size_t	start = locations.find("return", ref);
	size_t	end = locations.find(";", ref);

	if (start == std::string::npos || end == std::string::npos || start > end)
		return (false);
	return (true);
}

void	Config::_config_methods(std::string ref, t_location &location)
{
	size_t		start;

	start = ref.find("GET");
	if (start != std::string::npos)
		location.methods.push_back("GET");
	start = ref.find("POST");
	if (start != std::string::npos)
		location.methods.push_back("POST");
	start = ref.find("DELETE");
	if (start != std::string::npos)
		location.methods.push_back("DELETE");
	return ;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

t_server	&Config::get_server(int index)
{
	return (this->_servers[index]);
}

size_t	Config::get_total_servers(void)
{
	return (this->_server_string.size());
}

bool	&Config::is_good_to_run(void)
{
	return (this->_good_to_run);
}