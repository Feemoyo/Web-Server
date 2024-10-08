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
	static std::string		_status_msg;
	static size_t			_content_length;

	void					_extract_content(void);

private:

	std::map<std::string, std::string>	statusCodes;
	
public:

						ToolKit(void);
	virtual 			~ToolKit(void);

	std::string			get_content(void) const;
	std::string			get_file_name(void) const;
	std::string			get_file_path(void) const;
	std::string			get_status_code(void) const;
	std::string			get_content_type(void) const;
	size_t				get_content_length(void) const;

	void				status_code_mapper(void);
	void				status_code_distributor(std::string status_code);
	void				remove_white_spaces(std::string &str);

	void				set_file(std::string path, std::string name);
	void				set_file(std::string path_and_name);
	void				set_content(std::string new_content);
	void				set_content_type(std::string type);
	void				set_content_length(size_t length);

};

std::ostream	&operator<<(std::ostream &lhs, const t_location &rhs);
std::ostream	&operator<<(std::ostream &lhs, const t_server &rhs);

#endif
