/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 22:10:03 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/28 01:41:07 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigHandler.hpp"

static bool	_is_digit(char c)
{
	return (c >= 48 && c <= 57);
}

static bool	_is_alpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static bool	_is_alphanum(char c)
{
	return (_is_digit(c) || _is_alpha(c));
}

static bool	_is_white_space(char c)
{
	return (c == 32 || (c >= 8 && c <= 13));
}

static bool	_is_open_bracket(char c)
{
	return (c == 123);
}

static bool	_is_close_bracket(char c)
{
	return (c == 125);
}

ConfigHandler::ConfigHandler(void)
{
	return ;
}

ConfigHandler::~ConfigHandler(void)
{
	return ;
}

ConfigHandler::ConfigHandler(std::string &content)
{
	if (this->_is_valid(content) == true)
		std::cout << "VALID" << std::endl;
	else
		std::cout << "NOT VALID" << std::endl;
	return ;
}

bool	ConfigHandler::_is_brackets_ok(std::string &content)
{
	int	opened_brackets = 0;
	int	closed_brackets = 0;

	for (size_t i = 0; i < content.size(); i++)
	{
		char	ref = content[i];
		if (_is_open_bracket(ref))
			opened_brackets++;
		else if (_is_close_bracket(ref))
			closed_brackets++;
	}
	return (opened_brackets == closed_brackets);
}

bool	ConfigHandler::_is_server_block_ok(std::string &content)
{
	//	check for server on config file
	size_t	found = content.find("server");
	if (found == std::string::npos || \
		(!_is_white_space(content[found + 6]) && !_is_open_bracket(content[found + 6])))
		return (false);
	
	//	check for listen and port number in config file
	//	listen
	found = content.find("listen");
	if (found == std::string::npos || !_is_white_space(content[found + 6]))
		return (false);
	found += 6;
	while (found < content.length() && _is_white_space(content[found]))
		found++;
	//	port number
	if (!_is_digit(content[found]))
		return (false);

	//	check for server_name and chosen name in config file
	//	server_name
	found = content.find("server_name");
	if (found == std::string::npos || !_is_white_space(content[found + 11]))
		return (false);
	found += 11;
	while (found < content.length() && _is_white_space(content[found]))
		found++;
	//	chosen name
	while (_is_alphanum(content[found]))
		found++;

	return (true);
}

bool	ConfigHandler::_is_valid(std::string &content)
{
	if (_is_brackets_ok(content) == true && _is_server_block_ok(content) == true)
		return (true);
	return (false);
}
