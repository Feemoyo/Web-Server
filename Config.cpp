/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/15 16:19:05 by rferrero         ###   ########.fr       */
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

//TODO: Melhorar o encerramento em caso de erro
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

		_find_config_port(server, this->_total_servers[i]);
		_find_config_server_name(server, this->_total_servers[i]);
		_find_config_root(server, this->_total_servers[i]);
		_find_config_max_body_size(server, this->_total_servers[i]);
		_find_config_errors_location(server, this->_total_servers[i]);
		_find_config_default_index_location(server, this->_total_servers[i]);
		_find_other_locations(server, this->_total_servers[i]);
		this->_servers.push_back(server);
	}
	return ;
}

void	Config::_find_config_port(t_server &server, size_t start)
{
	size_t		end;
	std::string	ref;

	start = this->_content.find("listen", start) + strlen("listen");
	end = this->_content.find("server_name", start);
	ref = this->_content.substr(start, end - start);
	server.port = _string_to_int(ref);
	return ;
}

void	Config::_find_config_server_name(t_server &server, size_t start)
{
	size_t		end;

	start = this->_content.find("server_name", start) + strlen("server_name");
	end = this->_content.find("root", start);
	server.server_name = this->_content.substr(start, end - start);
	return ;
}

void	Config::_find_config_root(t_server &server, size_t start)
{
	size_t		end;

	start = this->_content.find("root", start) + strlen("root");
	end = this->_content.find("client_max_body_size", start);
	server.root = this->_content.substr(start, end - start);
	return ;
}

void	Config::_find_config_max_body_size(t_server &server, size_t start)
{
	size_t		end;
	std::string	ref;

	start = this->_content.find("client_max_body_size", start) + strlen("client_max_body_size");
	end = this->_content.find("location/", start);
	ref = this->_content.substr(start, end - start);
	server.max_body_size = _string_to_int(ref);
	return ;
}

void	Config::_find_config_errors_location(t_server &server, size_t start)
{
	size_t		end;
	std::string	ref;

	start = this->_content.find("location/errors/{default", start) + strlen("location/errors/{default");
	end = this->_content.find("allowed_methods", start);
	ref = this->_content.substr(start, end - start);
	t_location	errors;

	errors.path = ref;
	errors.default_file = ref;
	start = this->_content.find(ref + "allowed_methods", start) + strlen(ref.c_str()) + strlen("allowed_methods");
	end = this->_content.find("}", start);
	ref = this->_content.substr(start, end - start);
	_find_methods(ref, errors);
	server.locations.insert(std::pair<std::string, t_location>(errors.path, errors));
	return ;
}

void	Config::_find_config_default_index_location(t_server &server, size_t start)
{
	size_t		end;
	std::string	ref;

	start = this->_content.find("location", start) + strlen("location");
	end = this->_content.find("{defaultindex.html", start);
	ref = this->_content.substr(start, end - start);
	t_location	index;

	index.path = ref;
	index.default_file = "index.html";
	start = this->_content.find("index.htmlallowed_methods", start) + strlen("index.htmlallowed_methods");
	end = this->_content.find("}", start);
	ref = this->_content.substr(start, end - start);
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

			locat.path = ref;
			start = this->_content.find("default", end) + strlen("default");
			end = this->_content.find("allowed_methods", start);
			ref = this->_content.substr(start, end - start);
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

/*
** -------------------------------- OVERLOADS ---------------------------------
*/

std::ostream	&operator<<(std::ostream &lhs, const t_location &rhs)
{
	lhs << "Location " << rhs.path << std::endl;
	lhs << "Default file: " << rhs.default_file << std::endl;
	for (size_t i = 0; i < rhs.methods.size(); i++)
		lhs << rhs.methods[i] << " ";
	return (lhs);
}

std::ostream	&operator<<(std::ostream &lhs, const t_server &rhs)
{
	lhs << "Server " << rhs.server_name << std::endl;
	lhs << "Port: " << rhs.port << std::endl;
	lhs << "Root: " << rhs.root << std::endl;
	lhs << "Max client body size: " << rhs.max_body_size << std::endl;
	for (std::map<std::string, t_location>::const_iterator it = rhs.locations.begin(); it != rhs.locations.end(); it++)
		lhs << (*it).first << " " << (*it).second << std::endl;
	return (lhs);
}
