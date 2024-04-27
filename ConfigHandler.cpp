/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 22:10:03 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/27 00:18:31 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigHandler.hpp"

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
		if (ref == '{')
			opened_brackets++;
		else if (ref == '}')
			closed_brackets++;
	}
	return (opened_brackets == closed_brackets);
}

bool	ConfigHandler::_is_server_block_ok(std::string &content)
{
	size_t	found = content.find("server");
	if (found == std::string::npos)
		return (false);
	found = content.find("listen");
	if (found == std::string::npos)
		return (false);
	return (true);
}

bool	ConfigHandler::_is_valid(std::string &content)
{
	if (_is_brackets_ok(content) == true && _is_server_block_ok(content) == true)
		return (true);
	return (false);
}
