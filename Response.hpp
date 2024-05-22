#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ToolKit.hpp"

class	Response : public ToolKit
{
private:
	int			_client_fd;
	t_server	_server;
	std::string	_path;
	std::string	_filename;
	std::string	_method;

	std::string	_header;
	std::string	_response;

				Response(void);

	void		_check_directory_location(void);
	void		_check_file_location(void);
	void		_check_file_empty(void);
	void		_check_allowed_methods(void);
	void		_check_errors_location_file(void);

	void		_make_response(void);
	void		_send_response(void);

public:
				Response(void);
				Response(int client_fd, t_server &server, std::string path_and_name, std::string method);
				~Response(void);

	void		run_response(void);

};

#endif
