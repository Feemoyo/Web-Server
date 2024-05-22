#pragma once

#ifndef TOOLKIT_HPP
# define TOOLKIT_HPP

# include "Utils.hpp"

class	ToolKit
{
protected:
	std::string				_content;
	std::string				_file_path;
	std::string				_file_name;

	static std::string		_status_code;
	static std::string		_content_type;
	static	std::string		_status_msg;

	void				_extract_content(void);
	
public:

						ToolKit(void);
	virtual 			~ToolKit(void);

	std::string			&get_content(void);
	std::string			&get_file_name(void);
	std::string			&get_file_path(void);
	std::string			&get_status_code(void);
	std::string			&get_content_type(void);

	void				status_code_handler(std::string status_code, std::string html_path);
	void				set_file(std::string path, std::string name);
	void				set_file(std::string path_and_name);
	void				set_status_code(std::string code);
	void				set_content_type(std::string type);

};

std::ostream	&operator<<(std::ostream &lhs, const t_location &rhs);
std::ostream	&operator<<(std::ostream &lhs, const t_server &rhs);

#endif
