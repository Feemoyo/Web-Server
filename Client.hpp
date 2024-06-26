#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ToolKit.hpp"
# include "Mime.hpp"

class	Client : public ToolKit
{
private:
	std::map<std::string, std::string>	_buffer_map;
	Mime								_mime;
	std::string							_map_finder(std::string key, std::string value1, std::string value2);
	

public:
					Client(void);
					~Client(void);

	void			set_buffer(std::vector<char> buffer);
	void			format_content_type(void);
	
	std::string		get_path(void);
	std::string		get_method(void);
	void			print_map(void);

};

#endif
