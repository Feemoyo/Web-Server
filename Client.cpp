/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 14:32:11 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/14 19:32:39 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(void)
{
	this->_buffer_map.clear();

	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client(void)
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Client::request_process(int &client_fd)
{
	char	buffer[1024];
	ssize_t	bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
	
	(void)bytes_read;
	std::istringstream stream(buffer);
	std::string line;

	this->_buffer_map.clear();
	std::getline(stream, line);
	this->_buffer_map["Request"] = line;
	while (std::getline(stream, line))
	{
		std::size_t first_space = line.find(':');
		if (first_space != std::string::npos)
		{
			std::string key = line.substr(0, first_space);
			std::string value = line.substr(first_space + 2);

			this->_buffer_map[key] = value;
		}
	}
	return ;
}

void	Client::print_map(void)
{
	std::map<std::string, std::string>::iterator it;
	for (it = this->_buffer_map.begin(); it != this->_buffer_map.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	return ;
}

std::string	Client::get_path(std::map<std::string, t_location> locations)
{
	std::size_t auxFindGET1 = this->_buffer_map["Request"].find("/", 0);
	std::size_t auxFindGET2 = this->_buffer_map["Request"].find(" ", auxFindGET1);
	std::string path = this->_buffer_map["Request"].substr(auxFindGET1, auxFindGET2 - auxFindGET1);

	//verificar se path é uma localização valida
	// struct stat buffer;
    // if (stat(path.c_str(), &buffer) == 0) {
    //     if (!S_ISREG(buffer.st_mode) && (buffer.st_mode & S_IRUSR))
	// 	{
    //         this->set_status_code("403 Forbidden");
    //     }
    // }
	// else
	// {
	// 	this->set_status_code("404 Not Found");
	
	// }

	std::map<std::string, t_location>::iterator	it;
	for (it = locations.begin(); it != locations.end(), it++)
	{
		if (it != std::string::npos)
	}

	if (!path.compare("/"))
<<<<<<< HEAD
		path = "/index.html";
	return (path);
=======
		path = "/www/index.html";

	print_map();

	return ("." + path);
>>>>>>> 5f2e937bea34b791c0ed59030e27378718e9a643
}