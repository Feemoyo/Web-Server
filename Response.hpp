#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ToolKit.hpp"

class	Response : public ToolKit
{
private:
	t_response	_response;

				Response(void);

	void		_file_validation(void);
	void		_directory_validation(void);
	void		_check_directory_location(void);
	void		_check_allowed_methods(void);
	void		_check_directory_autoindex(void);
	void		_check_errors_location_file(void);
<<<<<<< HEAD
	void		_check_file_location(void);
	void		_set_dir_content(void);
	std::string	_get_dir_files(void);
=======
	void		_set_dir_file(std::string &full_dir_path);
>>>>>>> 4f669df38635351ec010d40c431bc52ea65aea74

	void		_make_response(void);
	void		_send_response(void);

public:
				Response(int client_fd, t_server &server, std::string path_and_name, std::string method);
				~Response(void);

	void		run_response(void);

};

#endif
