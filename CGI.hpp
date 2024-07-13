#pragma once

#ifndef CGI_HPP
# define CGI_HPP

class	CGI
{
private:
	std::string	_scripts_arg;

public:
			CGI(void);
			~CGI(void);

	void	run_cgi(void);

};

#endif