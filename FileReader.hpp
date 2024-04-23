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
	VSG
}	t_type;

class	FileReader
{
private:
	std::string	_filename;
	t_type		_type;

	int			_classification(void);

public:
				FileReader(void);
				FileReader(std::string filename);
				~FileReader(void);
};

#endif
