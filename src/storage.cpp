//#define DEBUG_STORAGE		// Debug

#include        <stdio.h>
#include	"gr.h"
#include	"extern.h"
#include	"image_ex.h"
#include	"private.h"
extern char     _grTempString[];

/*
 * Allocate storage.
 */
bool
create_arrays()
{
#if 0
	GET_STORAGE(_dstack, double, _num_dstackMAX);
	// done-another-way ... GET_STORAGE(_errorMsg, char, LineLength);
	GET_STORAGE(_cmdLine, char, LineLength);
	GET_STORAGE(_cmdLineCOPY, char, LineLength);
#endif
	return true;
}

bool
allocate_image_storage(int nx, int ny)
{
	if (nx < 0 || ny < 0)
		return false;
	if (image_exists())
		delete [] _image.image;
	_image.ras_magic = RAS_MAGIC;
	_image.ras_width = nx;
	_image.ras_height = ny;
	_image.ras_depth = 8;
	_image.ras_length = _image.ras_width * _image.ras_height;
	_image.ras_type = RT_STANDARD;
	_image.ras_maptype = RMT_NONE;
	_image.ras_maplength = 0;
	_image.image = new unsigned char[_image.ras_length];
	if (!_image.image) OUT_OF_MEMORY;
	return true;
}

bool
allocate_imageMask_storage(int nx, int ny)
{
	if (nx < 0 || ny < 0)
		return false;
	if (imageMask_exists())
		delete [] _imageMask.image;
	_imageMask.ras_magic = RAS_MAGIC;
	_imageMask.ras_width = nx;
	_imageMask.ras_height = ny;
	_imageMask.ras_depth = 8;
	_imageMask.ras_length = _imageMask.ras_width * _imageMask.ras_height;
	_imageMask.ras_type = RT_STANDARD;
	_imageMask.ras_maptype = RMT_NONE;
	_imageMask.ras_maplength = 0;
	_imageMask.image = new unsigned char[_imageMask.ras_length];
	if (!_imageMask.image) OUT_OF_MEMORY;
	_imageHist_exists = false;
	return true;
}

bool
allocate_grid_storage(int nx, int ny)
{
	if (nx < 0 || ny < 0)
		return false;
	_f_xy.set_size(nx, ny);	// *** NOT SURE ABOUT THIS ***
	_f_xy.set_value(0.0);
	_legit_xy.set_size(nx, ny);
	_legit_xy.set_value(true);
	_grid_exists = true;
	return true;
}
