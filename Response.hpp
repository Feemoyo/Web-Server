#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ToolKit.hpp"

class	Response : public ToolKit
{
private:
	int			_client_fd;
	std::string	_header;
	std::string	_response;
	std::string	_root;

	void		_make_response(void);

public:
				Response(void);
				Response(int client_fd);
				~Response(void);

	void		send_response(void);

	void		set_fd(int fd);

};

#endif
