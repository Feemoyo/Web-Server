#pragma once

#ifndef FileHandler_H
# define FileHandler_H

# include "Utils.hpp"

class	FileHandler
{
private:
	std::string		_filename;
	t_type			_type;
	std::string		_file_content;

	void			_classification(void);

	const char		*_find_extension(std::string &filename);
	void			_extract_content(void);

public:
					FileHandler(void);
					FileHandler(std::string filename);
					~FileHandler(void);

	const t_type	&get_type(void);
	std::string		&get_content(void);

};

#endif
