/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:20:59 by rferrero          #+#    #+#             */
/*   Updated: 2024/07/30 17:21:19 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mime.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Mime::Mime()
{
	this->_set_image_types();
	this->_set_html_type();
	return ;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Mime::~Mime()
{
	return ;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
void	Mime::_add_mime(std::string extension, std::string mime_type)
{
	this->_mime[extension] = mime_type;
	return ;

}

void	Mime::_set_image_types()
{
	this->_add_mime("avif", "image/avif");
	this->_add_mime("avifs", "image/avif");
	this->_add_mime("bmp", "image/bmp");
	this->_add_mime("cgm", "image/cgm");
	this->_add_mime("g3", "image/g3fax");
	this->_add_mime("gif", "image/gif");
	this->_add_mime("heic", "image/heic");
	this->_add_mime("heif", "image/heic");
	this->_add_mime("ief", "image/ief");
	this->_add_mime("jpe", "image/jpeg");
	this->_add_mime("jpeg", "image/jpeg");
	this->_add_mime("jpg", "image/jpeg");
	this->_add_mime("jfif", "image/jpeg");
	this->_add_mime("jfif-tbnl", "image/jpeg");
	this->_add_mime("jif", "image/jpeg");
	this->_add_mime("pjpg", "image/pjpeg");
	this->_add_mime("png", "image/png");
	this->_add_mime("btif", "image/prs.btif");
	this->_add_mime("svg", "image/svg+xml");
	this->_add_mime("svgz", "image/svg+xml");
	this->_add_mime("tif", "image/tiff");
	this->_add_mime("tiff", "image/tiff");
	this->_add_mime("psd", "image/vnd.adobe.photoshop");
	this->_add_mime("djv", "image/vnd.djvu");
	this->_add_mime("djvu", "image/vnd.djvu");
	this->_add_mime("dwg", "image/vnd.dwg");
	this->_add_mime("dxf", "image/vnd.dxf");
	this->_add_mime("fbs", "image/vnd.fastbidsheet");
	this->_add_mime("fpx", "image/vnd.fpx");
	this->_add_mime("fst", "image/vnd.fst");
	this->_add_mime("mmr", "image/vnd.fujixerox.edmics-mmr");
	this->_add_mime("rlc", "image/vnd.fujixerox.edmics-rlc");
	this->_add_mime("mdi", "image/vnd.ms-modi");
	this->_add_mime("npx", "image/vnd.net-fpx");
	this->_add_mime("wbmp", "image/vnd.wap.wbmp");
	this->_add_mime("xif", "image/vnd.xiff");
	this->_add_mime("webp", "image/webp");
	this->_add_mime("dng", "image/x-adobe-dng");
	this->_add_mime("cr2", "image/x-canon-cr2");
	this->_add_mime("crw", "image/x-canon-crw");
	this->_add_mime("ras", "image/x-cmu-raster");
	this->_add_mime("cmx", "image/x-cmx");
	this->_add_mime("erf", "image/x-epson-erf");
	this->_add_mime("fh", "image/x-freehand");
	this->_add_mime("fh4", "image/x-freehand");
	this->_add_mime("fh5", "image/x-freehand");
	this->_add_mime("fh7", "image/x-freehand");
	this->_add_mime("fhc", "image/x-freehand");
	this->_add_mime("raf", "image/x-fuji-raf");
	this->_add_mime("icns", "image/x-icns");
	this->_add_mime("ico", "image/x-icon");
	//TIPS: dcr tambem pode ser usado para application/x-director
	this->_add_mime("dcr", "image/x-kodak-dcr");
	this->_add_mime("k25", "image/x-kodak-k25");
	this->_add_mime("kdc", "image/x-kodak-kdc");
	this->_add_mime("mrw", "image/x-minolta-mrw");
	this->_add_mime("nef", "image/x-nikon-nef");
	this->_add_mime("orf", "image/x-olympus-orf");
	this->_add_mime("raw", "image/x-panasonic-raw");
	this->_add_mime("rw2", "image/x-panasonic-raw");
	this->_add_mime("rwl", "image/x-panasonic-raw");
	this->_add_mime("pcx", "image/x-pcx");
	this->_add_mime("pef", "image/x-pentax-pef");
	this->_add_mime("ptx", "image/x-pentax-pef");
	this->_add_mime("pct", "image/x-pict");
	this->_add_mime("pic", "image/x-pict");
	this->_add_mime("pnm", "image/x-portable-anymap");
	this->_add_mime("pbm", "image/x-portable-bitmap");
	this->_add_mime("pgm", "image/x-portable-graymap");
	this->_add_mime("ppm", "image/x-portable-pixmap");
	this->_add_mime("rgb", "image/x-rgb");
	this->_add_mime("x3f", "image/x-sigma-x3f");
	this->_add_mime("arw", "image/x-sony-arw");
	this->_add_mime("sr2", "image/x-sony-sr2");
	this->_add_mime("srf", "image/x-sony-srf");
	this->_add_mime("xbm", "image/x-xbitmap");
	this->_add_mime("xpm", "image/x-xpixmap");
	this->_add_mime("xwd", "image/x-xwindowdump");
	return ;
}

void	Mime::_set_html_type()
{
	this->_add_mime("html", "text/html");
	this->_add_mime("csv", "text/csv");
	return ;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	&Mime::get_mime(std::string extension)
{
	return (this->_mime[extension]);
}
