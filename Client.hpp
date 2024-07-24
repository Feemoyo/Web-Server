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
	std::string							_url_decode(const std::string &str);
	int									_from_hex(char c);
	// std::string							_setOutputFile(std::string fileName);

public:
					Client(void);
					~Client(void);

	bool			set_buffer(std::vector<char> buffer, bool &payload);
	void			set_body_size(void);
	void			format_content_type(void);
	
	std::string		get_path(void);
	std::string		get_method(void);
	void			clear_buffer(void);
	void			clear_body_size(void);
	void			decode_payload(void);
	void			save_output(void);
	void			print_map(void);
};

#endif
