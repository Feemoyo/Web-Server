/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:33:56 by fmoreira          #+#    #+#             */
/*   Updated: 2024/04/29 19:37:25 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_H
# define RESPONSE_H

# include <iostream>
# include <fstream>
# include <string>
# include <sys/socket.h>
# include <unistd.h>
# include <sstream>

// TODO: é necessario entender se precisamos primeiro ler todos os .html ou deixar conforme as request chegam
//
class Response
{
	private:
		int		_client_socket;
		std::string	_header;
		std::string	_response;
		std::string	_file_path;
		
		std::string	read_file(void) const;
		void		_make_response(void);
		
	public:
		Response(void);
		Response(int client_socket);
		~Response(void);

		void		set_socket(int socket);
		void		send_response(void);
};

#endif