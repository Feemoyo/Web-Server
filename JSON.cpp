/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JSON.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 12:16:45 by rferrero          #+#    #+#             */
/*   Updated: 2024/07/30 17:09:29 by rferrero         ###   ########.fr       */
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
:_payload(payload), _path(path), _file(file)
{
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

void	JSON::_json_writer(void)
{
	std::string			file_path = this->_path + "/" + this->_file;
	std::ofstream		outfile(file_path.c_str(), std::ios::in | std::ios::out | std::ios::ate);

	set_file(file_path);
	if (this->_content.empty())
	{
		outfile.seekp(0, std::ios::beg);
		outfile << "[\n\t{\n\t\t\"" << this->_payload << "\"\n\t}\n]";
	}
	else
	{
		size_t	found = this->_content.rfind("]");
		if (found != std::string::npos)
		{
			if (found > 0 && this->_content[found - 1] == '[')
			{
				outfile.seekp(found);
				outfile << "\n\t{\n\t\t\"" << this->_payload << "\"\n\t}\n]";
			}
			else
			{
				outfile.seekp(found - 1);
				outfile << ",\n\t{\n\t\t\"" << this->_payload << "\"\n\t}\n]";
			}
		}
	}
	outfile.close();
	return ;
}

void	JSON::run(void)
{
	if (!_directory_exists(this->_path))
	{
		if (!_create_dir(this->_path))
		{
			std::cerr << "Fail to create directory on path: " << this->_path << "\n";
			return ;
		}
	}
	if (!_file_exists(this->_path, this->_file))
	{
		if (!_create_file(this->_path, this->_file))
		{
			std::cerr << "Fail to create file: " << this->_path + this->_file << "\n";
			return ;
		}
	}
	this->_json_writer();
	return ;
}
