#include "ToolKit.hpp"

std::string	ToolKit::_status_code = "";

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

	if (!file.is_open())
	{
		std::cerr << "File cannot be opened" << std::endl;
		this->_content = "";
		this->_status_code = "404 Not Found";
		return ;
	}
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << "File is empty" << std::endl;
		this->_status_code = "404 Not Found";
		file.close();
		return ;
	}
	this->_status_code = "200 OK";
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

void	ToolKit::set_status_code(std::string code)
{
	this->_status_code = code;
	return ;
}
