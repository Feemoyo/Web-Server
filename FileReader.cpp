/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileReader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 16:36:14 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/23 17:05:14 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileReader.hpp"

FileReader::FileReader(void)
{
	return ;
}

FileReader::FileReader(std::string filename)
:_filename(filename)
{
	_classification();
	return ;
}

void	FileReader::_classification(void)
{
	char	*tmp = this->_filename.c_str();
	char	*ref;

	ref = strchr(tmp, '.');
	
	return ;
}
