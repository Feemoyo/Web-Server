/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:36:14 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/23 18:38:00 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileReader.hpp"

FileReader::FileReader(void)
{
	return ;
}

FileReader::FileReader(std::string filename)
:_filename(filename), _type(-1)
{
	_classification();
	return ;
}

FileReader::~FileReader(void)
{
	return ;
}

const char	*FileReader::_get_extension(std::string filename)
{
	const char	*dot;

	dot = std::strrchr(filename.c_str(), '.');
	if (!dot || dot == filename)
		return (NULL);
	return (dot + 1);
}

void	FileReader::_classification(void)
{
	char	extension[32];

	std::cout <<	std::strcpy(extension, _get_extension(this->_filename)) << std::endl;;
	// if (std::strcmp(extension, "HTML") || std::strcmp(extension, "html"))
	// 	this->_type = 0;
	// else if(std::strcmp(extension, "PNG") || std::strcmp(extension, "png"))
	// 	this->_type = 1;
	// else if (std::strcmp(extension, "SVG") || std::strcmp(extension, "svg"))
	// 	this->_type = 2;
	return ;
}

const int	&FileReader::get_type(void)
{
	return (this->_type);
}

