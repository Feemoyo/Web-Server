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
std::string	ToolKit::_content_type = "";
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

void	ToolKit::status_code_handler(std::string status_code, std::string html_path)
{
	std::map<std::string, std::string> statusCodes;

	statusCodes.insert(std::make_pair("100", "Continue"));
	statusCodes.insert(std::make_pair("101", "Switching Protocols"));
	statusCodes.insert(std::make_pair("102", "Processing"));
	statusCodes.insert(std::make_pair("200", "OK"));
	statusCodes.insert(std::make_pair("201","Created"));
	statusCodes.insert(std::make_pair("202", "Accepted"));
	statusCodes.insert(std::make_pair("203", "Non-Authoritative Information"));
	statusCodes.insert(std::make_pair("204",  "No Content"));
	statusCodes.insert(std::make_pair("205", "Reset Content"));
	statusCodes.insert(std::make_pair("206", "Partial Content"));
	statusCodes.insert(std::make_pair("207", "Multi-Status"));
	statusCodes.insert(std::make_pair("208", "Already Reported"));
	statusCodes.insert(std::make_pair("226", "IM Used"));
	statusCodes.insert(std::make_pair("300", "Multiple Choices"));
	statusCodes.insert(std::make_pair("301", "Moved Permanently"));
	statusCodes.insert(std::make_pair("302", "Found"));
	statusCodes.insert(std::make_pair("303", "See Other"));
	statusCodes.insert(std::make_pair("304", "Not Modified"));
	statusCodes.insert(std::make_pair("305", "Use Proxy"));
	statusCodes.insert(std::make_pair("307", "Temporary Redirect"));
	statusCodes.insert(std::make_pair("308", "Permanent Redirect"));
	statusCodes.insert(std::make_pair("400", "Bad Request"));
	statusCodes.insert(std::make_pair("401", "Unauthorized"));
	statusCodes.insert(std::make_pair("402", "Payment Required"));
	statusCodes.insert(std::make_pair("403", "Forbidden"));
	statusCodes.insert(std::make_pair("404", "Not Found"));
	statusCodes.insert(std::make_pair("405", "Method Not Allowed"));
	statusCodes.insert(std::make_pair("406", "Not Acceptable"));
	statusCodes.insert(std::make_pair("407", "Proxy Authentication Required"));
	statusCodes.insert(std::make_pair("408", "Request Timeout"));
	statusCodes.insert(std::make_pair("409", "Conflict"));
	statusCodes.insert(std::make_pair("410", "Gone"));
	statusCodes.insert(std::make_pair("411", "Length Required"));
	statusCodes.insert(std::make_pair("412", "Precondition Failed"));
	statusCodes.insert(std::make_pair("413", "Payload Too Large"));
	statusCodes.insert(std::make_pair("414", "URI Too Long"));
	statusCodes.insert(std::make_pair("415", "Unsupported Media Type"));
	statusCodes.insert(std::make_pair("416", "Range Not Satisfiable"));
	statusCodes.insert(std::make_pair("417", "Expectation Failed"));
	statusCodes.insert(std::make_pair("418", "I'm a teapot"));
	statusCodes.insert(std::make_pair("421", "Misdirected Request"));
	statusCodes.insert(std::make_pair("422", "Unprocessable Entity"));
	statusCodes.insert(std::make_pair("423", "Locked"));
	statusCodes.insert(std::make_pair("424", "Failed Dependency"));
	statusCodes.insert(std::make_pair("425", "Too Early"));
	statusCodes.insert(std::make_pair("426", "Upgrade Required"));
	statusCodes.insert(std::make_pair("428", "Precondition Required"));
	statusCodes.insert(std::make_pair("429", "Too Many Requests"));
	statusCodes.insert(std::make_pair("431", "Request Header Fields Too Large"));
	statusCodes.insert(std::make_pair("451", "Unavailable For Legal Reasons"));
	statusCodes.insert(std::make_pair("500", "Internal Server Error"));
	statusCodes.insert(std::make_pair("501", "Not Implemented"));
	statusCodes.insert(std::make_pair("502", "Bad Gateway"));
	statusCodes.insert(std::make_pair("503", "Service Unavailable"));
	statusCodes.insert(std::make_pair("504", "Gateway Timeout"));
	statusCodes.insert(std::make_pair("505", "HTTP Version Not Supported"));
	statusCodes.insert(std::make_pair("506", "Variant Also Negotiates"));
	statusCodes.insert(std::make_pair("507", "Insufficient Storage"));
	statusCodes.insert(std::make_pair("508", "Loop Detected"));
	statusCodes.insert(std::make_pair("510", "Not Extended"));
	statusCodes.insert(std::make_pair("511", "Network Authentication Required"));

	if (statusCodes.find(status_code) == statusCodes.end())
		status_code = "404";

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

std::string	&ToolKit::get_content_type(void)
{
	return (this->_content_type);
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

void	ToolKit::set_content_type(std::string type)
{
	this->_content_type = type;
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
