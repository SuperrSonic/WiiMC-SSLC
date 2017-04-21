/****************************************************************************
 * libwiigui
 *
 * Tantric 2009-2012
 *
 * gui_image.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"

static u64 prev;
static u64 now;

/**
 * Constructor for the GuiImage class.
 */
GuiImage::GuiImage()
{
	image = NULL;
	width = 0;
	height = 0;
	imageangle = 0;
	tile = -1;
	tileVertical = -1;
	stripe = 0;
	imgType = IMAGE_DATA;
	format = GX_TF_RGBA8;
}

GuiImage::GuiImage(GuiImageData * img)
{
	image = NULL;
	width = 0;
	height = 0;
	if(img)
	{
		image = img->GetImage();
		width = img->GetWidth();
		height = img->GetHeight();
		format = img->GetFormat();
	}
	imageangle = 0;
	tile = -1;
	tileVertical = -1;
	stripe = 0;
	imgType = IMAGE_DATA;
}

GuiImage::GuiImage(u8 * img, int w, int h)
{
	if(img) DCFlushRange(img, w*h*4);
	image = img;
	width = w;
	height = h;
	imageangle = 0;
	tile = -1;
	tileVertical = -1;
	stripe = 0;
	imgType = IMAGE_TEXTURE;
	format = GX_TF_RGBA8;
}

GuiImage::GuiImage(int w, int h, GXColor c)
{
	image = (u8 *)gui_memalign (32, w * h << 2);
	width = w;
	height = h;
	imageangle = 0;
	tile = -1;
	tileVertical = -1;
	stripe = 0;
	imgType = IMAGE_COLOR;
	format = GX_TF_RGBA8;

	if(!image)
		return;

	int x, y;

	for(y=0; y < h; ++y)
	{
		for(x=0; x < w; ++x)
		{
			this->SetPixel(x, y, c);
		}
	}
	int len = w * h << 2;
	if(len%32) len += (32-len%32);
	DCFlushRange(image, len);
}

/**
 * Destructor for the GuiImage class.
 */
GuiImage::~GuiImage()
{
	if(imgType == IMAGE_COLOR && image)
		gui_free(image);
}

// overloaded new operator
void *GuiImage::operator new(size_t size)
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
void GuiImage::operator delete(void *p)
{
	gui_free(p);
}

// overloaded new operator for arrays
void *GuiImage::operator new[](size_t size)
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
void GuiImage::operator delete[](void *p)
{
	gui_free(p);
}

u8 * GuiImage::GetImage()
{
	return image;
}

void GuiImage::SetImage(GuiImageData * img)
{
	image = NULL;
	width = 0;
	height = 0;
	if(img)
	{
		image = img->GetImage();
		width = img->GetWidth();
		height = img->GetHeight();
		format = img->GetFormat();
	}
	imgType = IMAGE_DATA;
}

void GuiImage::SetImage(u8 * img, int w, int h)
{
	image = NULL;
	width = 0;
	height = 0;
	if(img)
	{
		DCFlushRange(img, w*h*4);
		image = img;
		width = w;
		height = h;
		imgType = IMAGE_TEXTURE;
		format = GX_TF_RGBA8;
	}
}

void GuiImage::SetAngle(float a)
{
	imageangle = a;
}

void GuiImage::SetTile(int t)
{
	tile = t;
}

void GuiImage::SetTileVertical(int t)
{
	tileVertical = t;
}

GXColor GuiImage::GetPixel(int x, int y)
{
	if(!image || this->GetWidth() <= 0 || x < 0 || y < 0)
		return (GXColor){0, 0, 0, 0};

	u32 offset = (((y >> 2)<<4)*this->GetWidth()) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) << 1);
	GXColor color;
	color.a = *(image+offset);
	color.r = *(image+offset+1);
	color.g = *(image+offset+32);
	color.b = *(image+offset+33);
	return color;
}

void GuiImage::SetPixel(int x, int y, GXColor color)
{
	if(!image || this->GetWidth() <= 0 || x < 0 || y < 0)
		return;

	u32 offset = (((y >> 2)<<4)*this->GetWidth()) + ((x >> 2)<<6) + (((y%4 << 2) + x%4 ) << 1);
	*(image+offset) = color.a;
	*(image+offset+1) = color.r;
	*(image+offset+32) = color.g;
	*(image+offset+33) = color.b;
}

void GuiImage::SetStripe(int s)
{
	stripe = s;
}

void GuiImage::ColorStripe(int shift)
{
	GXColor color;
	int x, y=0;
	int alt = 0;
	
	int thisHeight =  this->GetHeight();
	int thisWidth =  this->GetWidth();

	for(; y < thisHeight; ++y)
	{
		if(y % 3 == 0)
			alt ^= 1;

		if(alt)
		{
			for(x=0; x < thisWidth; ++x)
			{
				color = GetPixel(x, y);

				if(color.r < 255-shift)
					color.r += shift;
				else
					color.r = 255;
				if(color.g < 255-shift)
					color.g += shift;
				else
					color.g = 255;
				if(color.b < 255-shift)
					color.b += shift;
				else
					color.b = 255;

				color.a = 255;
				SetPixel(x, y, color);
			}
		}
		else
		{
			for(x=0; x < thisWidth; ++x)
			{
				color = GetPixel(x, y);

				if(color.r > shift)
					color.r -= shift;
				else
					color.r = 0;
				if(color.g > shift)
					color.g -= shift;
				else
					color.g = 0;
				if(color.b > shift)
					color.b -= shift;
				else
					color.b = 0;

				color.a = 255;
				SetPixel(x, y, color);
			}
		}
	}
}

/**
 * Draw the button on screen
 */
void GuiImage::Draw()
{
	if(!image || width == 0 || height == 0 || !this->IsVisible() || tile == 0)
		return;

	float currScaleX = this->GetScaleX();
	float currScaleY = this->GetScaleY();
	int currLeft = this->GetLeft();
	int currTop = this->GetTop();
	int alpha = this->GetAlpha();

	if(tile >= 0)
	{
		for(int i=0; i<tile; ++i)
		{
			Menu_DrawImg(currLeft+width*i, currTop, width, height, image, imageangle, currScaleX, currScaleY, alpha, format);
		}
	}
	else if(tileVertical >= 0)
	{
		for(int i=0; i<tileVertical; ++i)
		{
			Menu_DrawImg(currLeft, currTop+height*i, width, height, image, imageangle, currScaleX, currScaleY, alpha, format);
		}
	}
	else
	{
		Menu_DrawImg(currLeft, currTop, width, height, image, imageangle, currScaleX, currScaleY, alpha, format);
	}

	if(stripe > 0)
	{
		int thisHeight = this->GetHeight();
		int thisWidth = this->GetWidth();
		for(int y=0; y < thisHeight; y+=6)
			Menu_DrawRectangle(currLeft,currTop+y,thisWidth,3,(GXColor){0, 0, 0, stripe},1);
	}
	this->UpdateEffects();

	if(effects & EFFECT_ROTATE)
	{
		if(effectAmount == 0)
		{
			effects = 0;
			imageangle = 0;
			return;
		}

		now = gettime();

		if(diff_usec(prev, now) > (u32)(effectAmount*1000))
		{
			prev = now;

			imageangle+=45.0f;
			if(imageangle >= 360.0f)
				imageangle = 0;
		}
	}
}
