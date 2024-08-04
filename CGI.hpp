#pragma once

#ifndef CGI_HPP
# define CGI_HPP

# include "Utils.hpp"

class	CGI
{
private:
	std::map<std::string, std::string>	_env;
	std::string							_cgi_ret;

			CGI(void);

	char	**_conver_map_to_array(std::map<std::string, std::string> &env);

public:
				CGI(t_cgi &cgi);
				~CGI(void);

	void		run_cgi(void);

	std::string	get_cgi_ret(void);

};

#endif