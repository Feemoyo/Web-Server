/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:36:14 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/26 10:55:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileHandler.hpp"

FileHandler::FileHandler(void)
{
	return ;
}

FileHandler::FileHandler(std::string filename)
:_filename(filename), _type(INVALID)
{
	_classification();
	if (this->_type != INVALID)
		_extract_content();
	return ;
}

FileHandler::~FileHandler(void)
{
	return ;
}

const char	*FileHandler::_find_extension(std::string &filename)
{
	const char	*dot;

	dot = std::strrchr(filename.c_str(), '.');
	if (!dot || dot == filename)
		return (NULL);
	return (dot + 1);
}

void	FileHandler::_extract_content(void)
{
	std::ifstream	file(this->_filename.c_str());
	std::string		line;

	if (!file.is_open())
	{
		std::cerr << "File cannot be opened" << std::endl;
		return ;
	}
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << "File is empty" << std::endl;
		file.close();
		return ;
	}
	while (std::getline(file, line))
		this->_file_content += line + '\n';
	file.close();
	return ;
}


void	FileHandler::_classification(void)
{
	char	extension[32];
	
	if (_find_extension(this->_filename) == NULL)
		return ;
	std::strcpy(extension, _find_extension(this->_filename));
	if (std::strcmp(extension, "CONF") || std::strcmp(extension, "conf"))
		this->_type = CONF;
	else if (std::strcmp(extension, "HTML") || std::strcmp(extension, "html"))
		this->_type = HTML;
	else if(std::strcmp(extension, "PNG") || std::strcmp(extension, "png"))
		this->_type = PNG;
	else if (std::strcmp(extension, "SVG") || std::strcmp(extension, "svg"))
		this->_type = SVG;
	return ;
}

const t_type	&FileHandler::get_type(void)
{
	return (this->_type);
}

std::string	&FileHandler::get_content(void)
{
	return (this->_file_content);
}
