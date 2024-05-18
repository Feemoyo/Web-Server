/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolKit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:01:03 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/18 00:17:47 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ToolKit.hpp"

std::string	ToolKit::_status_code = "";
std::string	ToolKit::_status_msg = "";

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ToolKit::ToolKit(void)
{
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ToolKit::~ToolKit(void)
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


void	ToolKit::_extract_content(void)
{
	std::ifstream	file((this->_file_path + this->_file_name).c_str());
	std::string		line;

	std::getline(file, line);
	this->_content = line + '\n';
	while (std::getline(file, line))
		this->_content += line + '\n';
	file.close();
	return ;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	&ToolKit::get_content(void)
{
	return (this->_content);
}

std::string	&ToolKit::get_file_name(void)
{
	return (this->_file_name);
}

std::string	&ToolKit::get_file_path(void)
{
	return (this->_file_path);
}

std::string	&ToolKit::get_status_code(void)
{
	return (this->_status_code);
}

/*
** --------------------------------- SETTERS ---------------------------------
*/

void	ToolKit::set_file(std::string path, std::string name)
{
	this->_file_path = path;
	this->_file_name = name;
	_extract_content();
	return ;
}

void	ToolKit::set_file(std::string path_and_name)
{	 
	this->_file_path = path_and_name.substr(0, path_and_name.find_last_of('/') + 1);
	this->_file_name = path_and_name.substr(path_and_name.find_last_of('/') + 1);
	_extract_content();
	return ;
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