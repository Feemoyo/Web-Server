#pragma once

#ifndef MIME_HPP
# define MIME_HPP

# include "Utils.hpp"

class Mime
{
private:
	std::map<std::string, std::string> _mime_image;

	void			_add_mime_image(std::string extension, std::string mime_type);
	void			_set_image_types();

public:
					Mime();
					~Mime();
	std::string		&get_mime_image(std::string extension);

};

#endif