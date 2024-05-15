#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ToolKit.hpp"

class	Response : public ToolKit
{
	private:
		int			_client_socket;
		std::string	_header;
		std::string	_response;

		void		_make_response(void);

	public:
					Response(void);
					Response(int client_socket);
					~Response(void);

		void		set_socket(int socket);
		void		send_response(void);

};

#endif
