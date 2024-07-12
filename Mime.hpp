#pragma once

#ifndef MIME_HPP
# define MIME_HPP

# include "Utils.hpp"

class Mime
{
private:
	std::map<std::string, std::string> _mime;

	void			_add_mime(std::string extension, std::string mime_type);
	void			_set_image_types();
	void			_set_html_type();

public:
					Mime();
					~Mime();
	std::string		&get_mime(std::string extension);

};

#endif