#pragma once

#ifndef FILEREADER_H
# define FILEREADER_H

# include <iostream>
# include <fstream>
# include <cstring>

typedef	enum	e_type
{
	HTML,
	PNG,
	SVG
}	t_type;

class	FileReader
{
private:
	std::string		_filename;
	int				_type;

	void			_classification(void);
	const char		*_get_extension(std::string filename);

public:
					FileReader(void);
					FileReader(std::string filename);
					~FileReader(void);

	const int	&get_type(void);

};

#endif
