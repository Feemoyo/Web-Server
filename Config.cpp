/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/09 14:08:17 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

static int	string_to_int(std::string str)
{
	std::istringstream	iss(str);
	int					ref;

	iss >> ref;
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
:_total_servers(0)
{
	_copy_content(content);
	_remove_comments();
	_remove_white_spaces();
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

void	Config::_server_block(void)
{
	size_t		start = 0;
	size_t		end = 0;
	t_server	server;
	std::string	ref;

	start = this->_content.find("server{");
	if (start == std::string::npos)
	{
		std::cerr << "Failed to find server configuration" << std::endl;
		return ; 
	}
	start = this->_content.find("listen") + strlen("listen");
	end = this->_content.find("server_name");
	ref = this->_content.substr(start, end - start);
	server.port = string_to_int(ref);

	start = this->_content.find("server_name") + strlen("server_name");
	end = this->_content.find("root");
	server.server_name = this->_content.substr(start, end - start);

	start = this->_content.find("root") + strlen("root");
	end = this->_content.find("client_max_body_size");
	server.root = this->_content.substr(start, end - start);

	start = this->_content.find("client_max_body_size") + strlen("error_page");
	end = this->_content.find("error_page");
	server.max_body_size = this->_content.substr(start, end - start);

	return ;
}

std::string	&Config::get_content(void)
{
	return (this->_content);
}
