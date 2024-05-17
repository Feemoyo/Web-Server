/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:59 by fmoreira          #+#    #+#             */
/*   Updated: 2024/05/17 15:50:17 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

/*
 ------------------------------- CONSTRUCTOR --------------------------------
*/

Response::Response(void)
{
    return ;
}

Response::Response(int client_fd, t_server server, std::string path_and_name)
{
    this->_client_fd = client_fd;
    this->_header = "";
    this->_response = "";
    this->_root = ("/" + server.root);

    this->set_file(path_and_name);

    // std::cout << "PATH: " << this->check_path() << std::endl;

    // std::cout << "RESPONSE PATH ANTES: " << path_and_name << std::endl;

    // if (this->_status_code >= "400" && this->_status_code < "500")
    //     this->set_file(server.root + (server.locations.find("/errors/"))->second.path + this->_status_code + ".html");

    // std::cout << "RESPONSE PATH DEPOIS: " << (server.root + (server.locations.find("/errors/"))->second.path + this->_status_code + ".html") << std::endl;

    return ;
}

/*
 -------------------------------- DESTRUCTOR --------------------------------
*/

Response::~Response(void)
{
    return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void    Response::_make_response(void)
{
    std::string            file_content;
    std::ostringstream    handler;

    file_content = this->get_content();
    handler << file_content.size();

    //TODO: o Content-Type tem que ser dinamico e pode ser encontrado no request
    this->_header = "HTTP/1.1 ";
    this->_header += this->_status_code;
    this->_header += "\nContent-Type:*/*\nContent-Length: ";
    this->_header += handler.str();
    this->_header += " \n\n";
    this->_response = this->_header;
    this->_response += file_content;

    return ;
}

void    Response::send_response(void)
{
    this->_make_response();
    write(this->_client_fd, this->_response.c_str(), this->_response.size());
    return ;
}