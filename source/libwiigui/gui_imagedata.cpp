/****************************************************************************
 * libwiigui
 *
 * Tantric 2009-2012
 *
 * gui_imagedata.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "../utils/pngu.h"
#include "../utils/jmemsrc.h"
#include "../utils/easybmp.h"
#include "../utils/giflib.h"
#include "gui.h"

/**
 * Constructor for the GuiImageData class.
 */
GuiImageData::GuiImageData(const u8 * i)
{
	data = NULL;
	width = 0;
	height = 0;
	format = GX_TF_RGBA8;

	if(i)
		LoadPNG(i);
}

GuiImageData::GuiImageData(const u8 * i, int s, u8 f)
{
	data = NULL;
	width = 0;
	height = 0;
	format = f;

	if(!i || s < 8)
		return;

	if (i[0] == 0xFF && i[1] == 0xD8)
		LoadJPEG(i, s);
	else if (i[0] == 'B' && i[1] == 'M')
		LoadBMP(i, s);
	else if (i[0] == 'G' && i[1] == 'I' && i[2] == 'F')
		LoadGIF(i, s);
	else if (i[0] == 0x89 && i[1] == 'P' && i[2] == 'N' && i[3] == 'G')
		LoadPNG(i);
}

/**
 * Destructor for the GuiImageData class.
 */
GuiImageData::~GuiImageData()
{
	if(data)
	{
		gui_free(data);
		data = NULL;
	}
}

// overloaded new operator
void *GuiImageData::operator new(size_t size)
{
	void *p = gui_malloc(size);

	if (!p)
	{
		bad_alloc ba;
		throw ba;
	}
	return p;
}

// overloaded delete operator
void GuiImageData::operator delete(void *p)
{
	gui_free(p);
}

// overloaded new operator for arrays
void *GuiImageData::operator new[](size_t size)
{
	void *p = gui_malloc(size);

	if (!p)
	{
		bad_alloc ba;
		throw ba;
	}
	return p;
}

// overloaded delete operator for arrays
void GuiImageData::operator delete[](void *p)
{
	gui_free(p);
}

void GuiImageData::SetData(u8 * i)
{
	data = i;
}

void GuiImageData::SetImage(const u8 * i, int s)
{
	if(!i)
		return;

	width = 0;
	height = 0;

	if (i[0] == 0xFF && i[1] == 0xD8)
		LoadJPEG(i, s);
	else if (i[0] == 'B' && i[1] == 'M')
		LoadBMP(i, s);
	else if (i[0] == 'G' && i[1] == 'I' && i[2] == 'F')
		LoadGIF(i, s);
	else if (i[0] == 0x89 && i[1] == 'P' && i[2] == 'N' && i[3] == 'G')
		LoadPNG(i);
}

void GuiImageData::LoadPNG(const u8 *i)
{
	data = DecodePNG(i, &width, &height, data);
}

void GuiImageData::LoadJPEG(const u8 *i, int s)
{
	data = DecodeJPEG(i, s, &width, &height, data);
}

void GuiImageData::LoadBMP(const u8 *i, int s)
{
	data = DecodeBMP(i, s, &width, &height, data);
}

void GuiImageData::LoadGIF(const u8 *i, int s)
{
	data = DecodeGIF(i, s, &width, &height, data);
}

u8 * GuiImageData::GetImage()
{
	return data;
}

int GuiImageData::GetWidth()
{
	return width;
}

int GuiImageData::GetHeight()
{
	return height;
}

u8 GuiImageData::GetFormat()
{
	return format;
}
