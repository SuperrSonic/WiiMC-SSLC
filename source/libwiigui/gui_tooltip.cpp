/****************************************************************************
 * libwiigui
 *
 * Tantric 2009-2012
 *
 * gui_tooltip.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"

static GuiImageData *tooltipLeft = NULL;
static GuiImageData *tooltipTile = NULL;
static GuiImageData *tooltipRight = NULL;

/**
 * Constructor for the GuiTooltip class.
 */
GuiTooltip::GuiTooltip(const char *t)
{
	if(tooltipLeft == NULL)
	{
		tooltipLeft = new GuiImageData(tooltip_left_png);
		tooltipTile = new GuiImageData(tooltip_tile_png);
		tooltipRight = new GuiImageData(tooltip_right_png);
	}

	leftImage.SetImage(tooltipLeft);
	tileImage.SetImage(tooltipTile);
	rightImage.SetImage(tooltipRight);
	
	height = leftImage.GetHeight();

	text = NULL;
	if(t)
		SetText(t);

	leftImage.SetParent(this);
	tileImage.SetParent(this);
	rightImage.SetParent(this);
	text->SetParent(this);
	
	time1 = 0;
	time2 = 0;
}

/*
 * Destructor for the GuiTooltip class.
 */
GuiTooltip::~GuiTooltip()
{
	if(text)
		delete text;
}

// overloaded new operator
void *GuiTooltip::operator new(size_t size)
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
void GuiTooltip::operator delete(void *p)
{
	gui_free(p);
}

// overloaded new operator for arrays
void *GuiTooltip::operator new[](size_t size)
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
void GuiTooltip::operator delete[](void *p)
{
	gui_free(p);
}

float GuiTooltip::GetScale()
{
	return (xscale * scaleDyn);
}

/* !Sets the text of the GuiTooltip element
 * !\param t Text
 */
void GuiTooltip::SetText(const char * t)
{
	if(text)
	{
		delete text;
		text = NULL;
	}

	if(!t)
		return;

	text = new GuiText(t, 16, (GXColor){0, 0, 0, 255});
	text->SetParent(this);

	int tile = ((float)text->GetTextWidth() / (float)tileImage.GetWidth()) + 1;

	if(tile < 0)
		tile = 0;

	tileImage.SetPosition(leftImage.GetWidth(), 0);
	tileImage.SetTile(tile);
	rightImage.SetPosition(leftImage.GetWidth() + tile * tileImage.GetWidth(), 0);
	width = leftImage.GetWidth() + tile * tileImage.GetWidth() + rightImage.GetWidth();
}

void GuiTooltip::ResetText()
{
	if(text)
		text->ResetText();

	int tile = ((float)text->GetTextWidth() / (float)tileImage.GetWidth()) + 1;

	if(tile < 0)
		tile = 0;

	tileImage.SetTile(tile);
	rightImage.SetPosition(leftImage.GetWidth() + tile * tileImage.GetWidth(), 0);
	width = leftImage.GetWidth() + tile * tileImage.GetWidth() + rightImage.GetWidth();
}

/*
 * Draws the tooltip
 */
void GuiTooltip::DrawTooltip()
{
	if(!text || !this->IsVisible() || !parentElement)
		return;

	if(parentElement->GetState() == STATE_SELECTED)
	{
		if (time2 == 0)
		{
			time(&time1);
			time2 = time1;
		}
		if(time1 != 0)
			time(&time1);

		if(time1 == 0 || difftime(time1, time2) >= 1.0)
		{
			if(time1 != 0)
				SetEffect(EFFECT_FADE, 20);
			time1 = 0;
			goto draw;
		}
	}
	else
	{
		if(time2 != 0 && time1 == 0)
			SetEffect(EFFECT_FADE, -20);
		time2 = 0;
	}

	if(GetEffect())
	{
		draw:
		leftImage.Draw();
		tileImage.Draw();
		rightImage.Draw();
		text->Draw();
	}

	this->UpdateEffects();
}
