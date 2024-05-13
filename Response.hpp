#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Utils.hpp"

class	Response
{
private:
	int			_client_socket;
	std::string	_header;
	std::string	_response;
	std::string	_file_path;

	std::string	_read_file(void) const;
	void		_make_response(void);

public:
				Response(void);
				Response(int client_socket);
				~Response(void);

	void		send_response(void);

	void		set_socket(int socket);
	void		set_file_path(std::string path);

};

#endif
