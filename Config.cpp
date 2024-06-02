/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/06/02 15:32:01 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
** ----------------------------- STATIC FUNCTIONS -----------------------------
*/

static int	_string_to_int(std::string str)
{
	std::stringstream	ss(str);
	int					ref;

	ss >> ref;
	return (ref);
}

static bool	_is_space(char c)
{
	return (std::isspace(static_cast<unsigned char>(c)));
}

static void	_find_methods(std::string ref, t_location &location)
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
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Config::Config(void)
{
	return ;
}

Config::Config(char *argv)
{
	set_file("./", argv);
	_remove_comments();
	_remove_white_spaces();
	if (!_verify_brackets())
		return ;
	_find_total_servers();
	_server_block();
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

void	Config::_remove_comments(void)
{
	size_t	ref;

	ref = this->_content.find('#');
	while (ref != std::string::npos)
	{
		size_t	end;

		end = this->_content.find('\n', ref);
		this->_content.erase(ref, end - ref);
		ref = this->_content.find('#');
	}
	return ;
}

void	Config::_remove_white_spaces(void)
{
	this->_content.erase(std::remove_if(this->_content.begin(), this->_content.end(), _is_space), this->_content.end());
	return ;
}

bool	Config::_verify_brackets(void)
{
	if (this->_count_occurrences(this->_content, '{') != this->_count_occurrences(this->_content, '}'))
	{
		std::cerr << "Error .conf: Brackets are not balanced" << std::endl;
		return (false);
	}
	return (true);
}

int	Config::_count_occurrences(const std::string &str, const char c)
{
	int i = 0;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
	    if (*it == c)
			i++;
	}
	return i;
}

void	Config::_find_total_servers(void)
{
	size_t	ref = 0;

	while ((ref = this->_content.find("server{", ref)) != std::string::npos)
	{
		this->_total_servers.push_back(ref);
		ref += strlen("server{");
	}
	return ;
}

void	Config::_server_block(void)
{
	if (this->_total_servers.empty())
	{
		std::cerr << "Fail to find server configuration" << std::endl;
		return ;
	}
	for (size_t i = 0; i < this->_total_servers.size(); i++)
	{
		t_server	server;

		server.port = _string_to_int(_find_in_config_file(this->_total_servers[i], "listen", "server_name"));
		server.server_name = _find_in_config_file(this->_total_servers[i], "server_name", "root");
		server.root = ("./" + _find_in_config_file(this->_total_servers[i], "root", "client_max_body_size"));
		server.max_body_size = _string_to_int(_find_in_config_file(this->_total_servers[i], "client_max_body_size", "location/"));

		_find_config_errors_location(server, this->_total_servers[i]);
		_find_config_default_index_location(server, this->_total_servers[i]);
		_find_other_locations(server, this->_total_servers[i]);

		this->_servers.push_back(server);
	}
	return ;
}

std::string	Config::_find_in_config_file(size_t start, std::string ref_start, std::string ref_end)
{
	size_t		end;

	start = this->_content.find(ref_start, start) + strlen(ref_start.c_str());
	end = this->_content.find(ref_end, start);
	return (this->_content.substr(start, end - start));
}

void	Config::_find_config_errors_location(t_server &server, size_t start)
{
	std::string	ref;

	ref = _find_in_config_file(start, "location/errors/{default", "allowed_methods");
	t_location	errors;

	errors.path = "/errors/";
	errors.default_file = ref;
	ref = _find_in_config_file(start, (ref + "allowed_methods"), "}");
	_find_methods(ref, errors);
	server.locations.insert(std::pair<std::string, t_location>(errors.path, errors));
	return ;
}

void	Config::_find_config_default_index_location(t_server &server, size_t start)
{
	std::string	ref;

	ref = _find_in_config_file(start, "location", "{defaultindex.html");
	t_location	index;

	index.path = (ref);
	index.default_file = "index.html";
	ref = _find_in_config_file(start, "index.htmlallowed_methods", "}");
	_find_methods(ref, index);
	server.locations.insert(std::pair<std::string, t_location>(index.path, index));
	return ;
}

void	Config::_find_other_locations(t_server &server, size_t start)
{
	size_t		end = this->_content.find("{", start);
	size_t		end_server = (this->_content.find("}}", start));
	std::string	ref;

	while (end < end_server)
	{
		start = this->_content.find("location", start) + strlen("location");
		end = this->_content.find("{", start);
		ref = this->_content.substr(start, end - start);
		if (ref != "/errors/" && ref != "/")
		{
			t_location	locat;

			locat.path = (ref);
			ref = _find_in_config_file(end, "default", "allowed_methods");
			locat.default_file = ref;
			start = this->_content.find("allowed_methods", start) + strlen("allowed_methods");
			end = this->_content.find("}", start);
			ref = this->_content.substr(start, end - start);
			_find_methods(ref, locat);
			server.locations.insert(std::pair<std::string, t_location>(locat.path, locat));
		}
	}
	return ;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::vector<t_server>	&Config::get_servers(void)
{
	return (this->_servers);
}

t_server	&Config::get_server(int index)
{
	return (this->_servers[index]);
}

size_t	Config::get_total_servers(void)
{
	return (this->_total_servers.size());
}

std::string	&Config::get_content(void)
{
	return (this->_content);
}

