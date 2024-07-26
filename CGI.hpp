#pragma once

#ifndef CGI_HPP
# define CGI_HPP

# include "Utils.hpp"

class	CGI
{
private:
	std::string							_script_path;
	std::map<std::string, std::string>	_env;

			CGI(void);
	char	**_conver_map_to_array(std::map<std::string, std::string> &env);

public:
			CGI(std::string &method, std::string &script, std::string &html, std::string &data);
			~CGI(void);

	void	run_cgi(void);

};

#endif