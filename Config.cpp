/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/10 23:47:27 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::ostream	&operator<<(std::ostream &lhs, const t_location &rhs)
{
	lhs << rhs.path << std::endl;
	lhs << rhs.default_file << std::endl;
	for (size_t i = 0; i < rhs.methods.size(); i++)
		lhs << rhs.methods[i] << std::endl;
	return (lhs);
}

static int	string_to_int(std::string str)
{
	std::stringstream	ss(str);
	int					ref;

	ss >> ref;
	return (ref);
}

static bool	is_space(char c)
{
	return (std::isspace(static_cast<unsigned char>(c)));
}

Config::Config(void)
{
	return ;
}

Config::Config(std::string &content)
{
	_copy_content(content);
	_remove_comments();
	_remove_white_spaces();
	_find_total_servers();
	_server_block();

	return ;
}

Config::~Config(void)
{
	return ;
}

void	Config::_copy_content(std::string &content)
{
	this->_content = content;
	return ;
}

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
	this->_content.erase(std::remove_if(this->_content.begin(), this->_content.end(), is_space), this->_content.end());
	return ;
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
		this->_servers.push_back(server);
	}
	for (std::vector<t_server>::iterator j = this->_servers.begin(); j != this->_servers.end(); j++)
	{
		std::cout << j->port << std::endl;
		std::cout << j->server_name << std::endl;
		std::cout << j->root << std::endl;
		std::cout << j->max_body_size << std::endl;
		for (std::map<std::string, t_location>::iterator it = j->locations.begin(); it != j->locations.end(); it++)
		{
			std::cout << it->first << std::endl;
			std::cout << it->second << std::endl;
		}
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
	server.port = string_to_int(ref);
	return ;
}

void	Config::_find_config_server_name(t_server &server, size_t start)
{
	size_t		end;
	std::string	ref;

	start = this->_content.find("server_name", start) + strlen("server_name");
	end = this->_content.find("root", start);
	server.server_name = this->_content.substr(start, end - start);
	return ;
}

void	Config::_find_config_root(t_server &server, size_t start)
{
	size_t		end;
	std::string	ref;

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
	server.max_body_size = string_to_int(ref);
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

	errors.path = "/errors/";
	errors.default_file = ref;
	errors.methods.push_back("GET");
	server.locations.insert(std::pair<std::string, t_location>("errors", errors));
	return ;
}

std::string	&Config::get_content(void)
{
	return (this->_content);
}
