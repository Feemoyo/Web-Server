#include "Mime.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Mime::Mime()
{
	this->_set_image_types();
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
void	Mime::_add_mime_image(std::string extension, std::string mime_type)
{
	this->_mime_image[extension] = mime_type;
}

void	Mime::_set_image_types()
{
	this->_add_mime_image("avif", "image/avif");
	this->_add_mime_image("avifs", "image/avif");
	this->_add_mime_image("bmp", "image/bmp");
	this->_add_mime_image("cgm", "image/cgm");
	this->_add_mime_image("g3", "image/g3fax");
	this->_add_mime_image("gif", "image/gif");
	this->_add_mime_image("heic", "image/heic");
	this->_add_mime_image("heif", "image/heic");
	this->_add_mime_image("ief", "image/ief");
	this->_add_mime_image("jpe", "image/jpeg");
	this->_add_mime_image("jpeg", "image/jpeg");
	this->_add_mime_image("jpg", "image/jpeg");
	this->_add_mime_image("jfif", "image/jpeg");
	this->_add_mime_image("jfif-tbnl", "image/jpeg");
	this->_add_mime_image("jif", "image/jpeg");
	this->_add_mime_image("pjpg", "image/pjpeg");
	this->_add_mime_image("png", "image/png");
	this->_add_mime_image("btif", "image/prs.btif");
	this->_add_mime_image("svg", "image/svg+xml");
	this->_add_mime_image("svgz", "image/svg+xml");
	this->_add_mime_image("tif", "image/tiff");
	this->_add_mime_image("tiff", "image/tiff");
	this->_add_mime_image("psd", "image/vnd.adobe.photoshop");
	this->_add_mime_image("djv", "image/vnd.djvu");
	this->_add_mime_image("djvu", "image/vnd.djvu");
	this->_add_mime_image("dwg", "image/vnd.dwg");
	this->_add_mime_image("dxf", "image/vnd.dxf");
	this->_add_mime_image("fbs", "image/vnd.fastbidsheet");
	this->_add_mime_image("fpx", "image/vnd.fpx");
	this->_add_mime_image("fst", "image/vnd.fst");
	this->_add_mime_image("mmr", "image/vnd.fujixerox.edmics-mmr");
	this->_add_mime_image("rlc", "image/vnd.fujixerox.edmics-rlc");
	this->_add_mime_image("mdi", "image/vnd.ms-modi");
	this->_add_mime_image("npx", "image/vnd.net-fpx");
	this->_add_mime_image("wbmp", "image/vnd.wap.wbmp");
	this->_add_mime_image("xif", "image/vnd.xiff");
	this->_add_mime_image("webp", "image/webp");
	this->_add_mime_image("dng", "image/x-adobe-dng");
	this->_add_mime_image("cr2", "image/x-canon-cr2");
	this->_add_mime_image("crw", "image/x-canon-crw");
	this->_add_mime_image("ras", "image/x-cmu-raster");
	this->_add_mime_image("cmx", "image/x-cmx");
	this->_add_mime_image("erf", "image/x-epson-erf");
	this->_add_mime_image("fh", "image/x-freehand");
	this->_add_mime_image("fh4", "image/x-freehand");
	this->_add_mime_image("fh5", "image/x-freehand");
	this->_add_mime_image("fh7", "image/x-freehand");
	this->_add_mime_image("fhc", "image/x-freehand");
	this->_add_mime_image("raf", "image/x-fuji-raf");
	this->_add_mime_image("icns", "image/x-icns");
	this->_add_mime_image("ico", "image/x-icon");
	//TIPS: dcr tambem pode ser usado para application/x-director
	this->_add_mime_image("dcr", "image/x-kodak-dcr");
	this->_add_mime_image("k25", "image/x-kodak-k25");
	this->_add_mime_image("kdc", "image/x-kodak-kdc");
	this->_add_mime_image("mrw", "image/x-minolta-mrw");
	this->_add_mime_image("nef", "image/x-nikon-nef");
	this->_add_mime_image("orf", "image/x-olympus-orf");
	this->_add_mime_image("raw", "image/x-panasonic-raw");
	this->_add_mime_image("rw2", "image/x-panasonic-raw");
	this->_add_mime_image("rwl", "image/x-panasonic-raw");
	this->_add_mime_image("pcx", "image/x-pcx");
	this->_add_mime_image("pef", "image/x-pentax-pef");
	this->_add_mime_image("ptx", "image/x-pentax-pef");
	this->_add_mime_image("pct", "image/x-pict");
	this->_add_mime_image("pic", "image/x-pict");
	this->_add_mime_image("pnm", "image/x-portable-anymap");
	this->_add_mime_image("pbm", "image/x-portable-bitmap");
	this->_add_mime_image("pgm", "image/x-portable-graymap");
	this->_add_mime_image("ppm", "image/x-portable-pixmap");
	this->_add_mime_image("rgb", "image/x-rgb");
	this->_add_mime_image("x3f", "image/x-sigma-x3f");
	this->_add_mime_image("arw", "image/x-sony-arw");
	this->_add_mime_image("sr2", "image/x-sony-sr2");
	this->_add_mime_image("srf", "image/x-sony-srf");
	this->_add_mime_image("xbm", "image/x-xbitmap");
	this->_add_mime_image("xpm", "image/x-xpixmap");
	this->_add_mime_image("xwd", "image/x-xwindowdump");
	
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	&Mime::get_mime_image(std::string extension)
{
	return (this->_mime_image[extension]);
}
