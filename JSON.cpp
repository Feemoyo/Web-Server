/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JSON.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:16:45 by rferrero          #+#    #+#             */
/*   Updated: 2024/08/03 16:49:44 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JSON.hpp"

/*
** ----------------------------- STATIC FUNCTIONS -----------------------------
*/

static bool	_directory_exists(const std::string &path)
{
	struct stat	info;

	if (stat(path.c_str(), &info) != 0)
	{
		std::cerr << "Cannot access the path: " << path << "\n";
		return (false);
	}
	else if (info.st_mode & S_IFDIR)
		return (true);
	return (false);
}

static bool	_create_dir(const std::string &path)
{
	if (mkdir(path.c_str(), 0755) == 0)
		return (true);
	return (false);
}

static bool	_file_exists(const std::string &path, const std::string &file)
{
	struct stat	info;

	if (stat(((path + "/" + file).c_str()), &info) == 0 && S_ISREG(info.st_mode))
		return (true);
	return (false);
}

static bool	_create_file(const std::string &path, const std::string &file)
{
	std::ofstream	new_file((path + "/" + file).c_str());

	if (new_file)
	{
		new_file.close();
		return (true);
	}
	return (false);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

JSON::JSON(void)
{
	return ;
}

JSON::JSON(std::string payload, std::string path, std::string file)
:_payload(payload), _path(path)
{
	for (std::size_t i = 0; i < file.size(); i++)
	{
		if (file[i] == '/')
			file[i] = '_';
	}
	this->_file = file + ".json";
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

JSON::~JSON(void)
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	JSON::json_delete(int comment)
{
	std::string			file_path = this->_path + "/" + this->_file;
	std::ifstream		infile(file_path.c_str());
	
	_check_dir_and_file();
	if (!infile.is_open())
	{
		std::cerr << "Fail to open: " << file_path <<  " to DELETE" << std::endl;
		return ;
	}
	infile.seekg(0, std::ios::end);
	if (infile.tellg() == 0)
	{
		infile.close();
		return ;
	}
	infile.seekg(0, std::ios::beg);

	std::stringstream	buffer;
	std::string			line;
	int					current_line = 0;

	while (std::getline(infile, line))
	{
		if (current_line != comment)
			buffer << line << "\n";
		current_line++;
	}
	infile.close();

	std::ofstream	outfile(file_path.c_str());

	if (!outfile.is_open())
	{
		std::cerr << "Fail to open: " << file_path <<  " to finish DELETE" << std::endl;
		return ;
	}
	outfile << buffer.str();
	outfile.close();
	return ;
}

void	JSON::json_post(void)
{
	std::string			file_path = this->_path + "/" + this->_file;
	std::ifstream		infile(file_path.c_str());
	bool				is_empty = true;

	_check_dir_and_file();
	if (!infile.is_open())
	{
		std::cerr << "Fail to open: " << file_path <<  " to POST" << std::endl;
		return ;
	}
	std::stringstream	buffer;
	std::string			line;

	infile.seekg(0, std::ios::end);
	if (infile.tellg() != 0)
	{
		is_empty = false;
		infile.seekg(0, std::ios::beg);
		buffer << infile.rdbuf();
	}
	infile.close();

	this->_payload_parser();
	std::ofstream	outfile(file_path.c_str());

	if (!outfile.is_open())
	{
		std::cerr << "Fail to open: " << file_path <<  " to finish POST" << std::endl;
		return ;
	}
	if (is_empty)
		outfile << "[\n{\"" << this->_payload << "\"}\n]";
	else
	{
		std::string	content = buffer.str();
		size_t		end = content.find("]");

		if (end != std::string::npos)
		{
			if (end > 0 && content[end - 1] == '[')
				content.insert(end, "\n{\"" + this->_payload + "\"}");
			else
				content.insert(end - 1, ",\n{\"" + this->_payload + "\"}");
		}
		outfile << content;
	}
	outfile.close();
	return ;
}

void	JSON::_payload_parser(void)
{
	this->_replace_ampersand();
	this->_replace_equal();
	return ;
}

void	JSON::_replace_ampersand(void)
{
	for(size_t i = this->_payload.find("&"); i != std::string::npos; i = this->_payload.find("&", i))
	{
		this->_payload.replace(i, 1, "\",\"");
	}
	return ;
}

void	JSON::_replace_equal(void)
{
	for(size_t i = this->_payload.find("="); i != std::string::npos; i = this->_payload.find("=", i))
	{
		this->_payload.replace(i, 1, "\": \"");
	}
	return ;
}

bool	JSON::_check_dir_and_file(void)
{
	if (!_directory_exists(this->_path))
	{
		if (!_create_dir(this->_path))
		{
			std::cerr << "Fail to create directory on path: " << this->_path << "\n";
			return (false);
		}
	}
	if (!_file_exists(this->_path, this->_file))
	{
		if (!_create_file(this->_path, this->_file))
		{
			std::cerr << "Fail to create file: " << this->_path + this->_file << "\n";
			return (false);
		}
	}
	return (true);
}
