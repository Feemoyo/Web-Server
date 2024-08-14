#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ToolKit.hpp"

class	Response : public ToolKit
{
private:
	t_response	_response;

				Response(void);

	void		_change_paths_for_redirections(void);
	void		_check_allowed_methods(void);
	void		_check_directory_location(void);

	bool		_check_for_cgi(void);
	void		_run_CGI(void);

	void		_handle_get_request(void);
	void		_handle_delete_request(void);
	void		_handle_post_request(void);

	void		_file_validation(void);
	void		_check_file_location(void);
	void		_check_max_body_size(void);

	void		_directory_validation(void);
	void		_check_directory_autoindex(void);
	std::string	_get_dir_files(void);
	void		_set_dir_content(void);

	void		_set_error_response(const std::string &status_code);
	void		_check_errors_location_file(void);
	void		_check_redirect_4xx_errors(void);

	void		_finalize_response(void);
	void		_response_maker(void);
	void		_send_response(void);

public:
				Response(int client_fd, t_server &server, std::string path_and_name, std::string method, std::string payload);
				~Response(void);

	void		run_response(void);

};

#endif
