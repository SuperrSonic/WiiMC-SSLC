/****************************************************************************
 * libwiigui
 *
 * Tantric 2009-2012
 *
 * gui_keypad.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"

static char tmptxt[MAX_KEYBOARD_DISPLAY];

static char * GetDisplayText(char * t)
{
	if(!t)
		return NULL;

	int len = strlen(t);

	if(len < MAX_KEYBOARD_DISPLAY)
		return t;

	strncpy(tmptxt, &t[len-MAX_KEYBOARD_DISPLAY], MAX_KEYBOARD_DISPLAY);
	tmptxt[MAX_KEYBOARD_DISPLAY-1] = 0;
	return &tmptxt[0];
}

/**
 * Constructor for the GuiKeypad class.
 */

GuiKeypad::GuiKeypad(char * t, u32 max)
{
	width = 540;
	height = 400;
	selectable = true;
	alignmentHor = ALIGN_CENTRE;
	alignmentVert = ALIGN_MIDDLE;
	strncpy(kptextstr, t, max);
	kptextstr[max] = 0;
	kptextmaxlen = max;

	char thekeys[4][3] = {
		{ '1', '2', '3' },	
		{ '4', '5', '6' },
		{ '7', '8',	'9'	},
		{ '-', '0', '.' }
	};
	memcpy(keys, thekeys, sizeof(thekeys));

	keyTextbox = new GuiImageData(keyboard_textbox_png);
	keyTextboxImg = new GuiImage(keyTextbox);
	keyTextboxImg->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	keyTextboxImg->SetPosition(0, 0);
	this->Append(keyTextboxImg);

	kpText = new GuiText(GetDisplayText(kptextstr), 20, (GXColor){255, 255, 255, 0xff});
	kpText->SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	kpText->SetPosition(0, 13);
	this->Append(kpText);

	key = new GuiImageData(keyboard_key_png);
	keyOver = new GuiImageData(keyboard_key_over_png);
	keyMedium = new GuiImageData(keyboard_mediumkey_png);
	keyMediumOver = new GuiImageData(keyboard_mediumkey_over_png);

	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	
	keyBackImg = new GuiImage(keyMedium);
	keyBackOverImg = new GuiImage(keyMediumOver);
	keyBackText = new GuiText("Back", 20, (GXColor){255, 255, 255, 0xff});
	keyBack = new GuiButton(keyMedium->GetWidth(), keyMedium->GetHeight());
	keyBack->SetImage(keyBackImg);
	keyBack->SetImageOver(keyBackOverImg);
	keyBack->SetLabel(keyBackText);
	keyBack->SetTrigger(trigA);
	keyBack->SetPosition(3*42+207, 100);
	keyBack->SetEffectGrow();
	this->Append(keyBack);

	char txt[2] = { 0, 0 };

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(keys[i][j] != '\0')
			{
				txt[0] = keys[i][j];
				keyImg[i][j] = new GuiImage(key);
				keyImgOver[i][j] = new GuiImage(keyOver);
				keyTxt[i][j] = new GuiText(txt, 20, (GXColor){255, 255, 255, 0xff});
				keyTxt[i][j]->SetAlignment(ALIGN_CENTRE, ALIGN_BOTTOM);
				keyTxt[i][j]->SetPosition(0, -10);
				keyBtn[i][j] = new GuiButton(key->GetWidth(), key->GetHeight());
				keyBtn[i][j]->SetImage(keyImg[i][j]);
				keyBtn[i][j]->SetImageOver(keyImgOver[i][j]);
				keyBtn[i][j]->SetTrigger(trigA);
				keyBtn[i][j]->SetLabel(keyTxt[i][j]);
				keyBtn[i][j]->SetPosition(j*42+207, i*42+100);
				keyBtn[i][j]->SetEffectGrow();
				this->Append(keyBtn[i][j]);
			}
		}
	}
	
	// disable '-' by default
	keyBtn[3][0]->SetState(STATE_DISABLED);
	keyBtn[3][0]->SetVisible(false);
}

/**
 * Destructor for the GuiKeypad class.
 */
GuiKeypad::~GuiKeypad()
{
	delete kpText;
	delete keyTextbox;
	delete keyTextboxImg;
	delete keyBackText;
	delete keyBackImg;
	delete keyBackOverImg;
	delete keyBack;
	delete key;
	delete keyOver;
	delete keyMedium;
	delete keyMediumOver;
	delete trigA;

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(keys[i][j] != '\0')
			{
				delete keyImg[i][j];
				delete keyImgOver[i][j];
				delete keyTxt[i][j];
				delete keyBtn[i][j];
			}
		}
	}
}

// overloaded new operator
void *GuiKeypad::operator new(size_t size)
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
void GuiKeypad::operator delete(void *p)
{
	gui_free(p);
}

// overloaded new operator for arrays
void *GuiKeypad::operator new[](size_t size)
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
void GuiKeypad::operator delete[](void *p)
{
	gui_free(p);
}

void GuiKeypad::EnableNegative()
{
	keyBtn[3][0]->SetState(STATE_DEFAULT);
	keyBtn[3][0]->SetVisible(true);
}

void GuiKeypad::Update(GuiTrigger * t)
{
	if(_elements.size() == 0 || (state == STATE_DISABLED && parentElement))
		return;

	for (u8 i = 0; i < _elements.size(); i++)
	{
		try	{ _elements.at(i)->Update(t); }
		catch (const std::exception& e) { }
	}
	
	if(keyBack->GetState() == STATE_CLICKED)
	{
		if(strlen(kptextstr) > 0)
		{
			kptextstr[strlen(kptextstr)-1] = 0;
			kpText->SetText(GetDisplayText(kptextstr));
		}
		keyBack->SetState(STATE_SELECTED, t->chan);
	}

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(keys[i][j] != '\0')
			{
				if(keyBtn[i][j]->GetState() == STATE_CLICKED)
				{
					if(strlen(kptextstr) < kptextmaxlen)
						kptextstr[strlen(kptextstr)] = keys[i][j];

					kpText->SetText(GetDisplayText(kptextstr));
					keyBtn[i][j]->SetState(STATE_SELECTED, t->chan);
				}
			}
		}
	}
}
