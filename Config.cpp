/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:46:21 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/06 16:39:02 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

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

	// this->_servers.push_back(this->_server_block())

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

std::string	&Config::get_content(void)
{
	return (this->_content);
}
