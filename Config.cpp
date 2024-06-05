/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/06/05 14:44:53 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
** ----------------------------- STATIC FUNCTIONS -----------------------------
*/

static bool	_is_space(char c)
{
	return (std::isspace(static_cast<unsigned char>(c)));
}

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

static void	_remove_white_spaces(std::string &str)
{
	str.erase(std::remove_if(str.begin(), str.end(), _is_space), str.end());
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
		std::cerr << "Error .conf: Brackets are not balanced" << std::endl;
		return (false);
	}
	return (true);
}

static int	_string_to_int(std::string str)
{
	std::stringstream	ss(str);
	int					ref;

	ss >> ref;
	return (ref);
}

static bool	_is_directory_visible(std::string str)
{
	if (str == "on")
		return (true);
	return (false);
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
	set_file("./", argv);
	_remove_comments(this->_content);
	_remove_white_spaces(this->_content);
	if (!_verify_brackets(this->_content))
		return ;
	_config_total_servers();
	_config_servers();
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

void	Config::_config_total_servers(void)
{
	size_t	ref = 0;

	while ((ref = this->_content.find("server{", ref)) != std::string::npos)
	{
		this->_total_servers.push_back(ref);
		ref += strlen("server{");
	}
	return ;
}

void	Config::_config_servers(void)
{
	if (this->_total_servers.empty())
	{
		std::cerr << "Fail to find server configuration" << std::endl;
		return ;
	}
	for (size_t i = 0; i < this->_total_servers.size(); i++)
	{
		t_server	server;

		server.port = _string_to_int(find_and_split(this->_total_servers[i], "listen", "server_name"));
		server.server_name = find_and_split(this->_total_servers[i], "server_name", "root");
		server.root = ("./" + find_and_split(this->_total_servers[i], "root", "directory"));
		server.directory = _is_directory_visible(find_and_split(this->_total_servers[i], "directory", "client_max_body_size"));
		server.max_body_size = _string_to_int(find_and_split(this->_total_servers[i], "client_max_body_size", "location"));

		_config_locations(server, this->_total_servers[i]);

		this->_servers.push_back(server);
	}
	return ;
}

void	Config::_config_locations(t_server &server, size_t start)
{
	size_t		end = this->_content.find("{", start);
	size_t		end_server = (this->_content.find("}}", start));
	std::string	ref;

	while (end < end_server)
	{
		t_location	locat;

		start = this->_content.find("location", start) + strlen("location");
		end = this->_content.find("{", start);

		locat.path = this->_content.substr(start, end - start);
		ref = find_and_split(end, "default", "directory");
		locat.default_file = ref;
		locat.directory = _is_directory_visible(find_and_split(end, "directory", "allowed_methods"));

		start = this->_content.find("allowed_methods", start) + strlen("allowed_methods");
		end = this->_content.find("}", start);
		ref = this->_content.substr(start, end - start);

		_config_methods(ref, locat);

		server.locations.insert(std::pair<std::string, t_location>(locat.path, locat));
	}
	return ;
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
	return (this->_total_servers.size());
}
