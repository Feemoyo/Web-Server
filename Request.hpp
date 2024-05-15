#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "ToolKit.hpp"

class	Request : public ToolKit
{
private:
	std::map<std::string, std::string>	_buffer_map;


public:
										Request(void);
										Request(char *buffer);
										~Request(void);

	void								set_buffer(char *buffer);
	
	// temos espaço para um getter de todas infos de um request ex: Content-Type.
	std::string							get_path(void);
	void								print_map(void);

};

#endif
