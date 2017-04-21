/****************************************************************************
 * libwiigui
 *
 * Tantric 2009-2012
 *
 * gui_optionbrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"
#include "filebrowser.h"

/**
 * Constructor for the GuiOptionBrowser class.
 */
GuiOptionBrowser::GuiOptionBrowser(int w, int s, OptionList * l)
{
	width = w;
	height = s*32;
	size = s;
	options = l;
	selectable = true;
	listOffset = this->FindMenuItem(-1, 1);
	listChanged = false;
	selectedItem = 0;
	focus = 0; // allow focus

	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	bgOptionsEntry = new GuiImageData(bg_entry_png);
	bgOptionsEntryOver = new GuiImageData(bg_entry_over_png);

	iconSMB = new GuiImageData(icon_smb_png);
	iconFTP = new GuiImageData(icon_ftp_png);

	scrollbarTop = new GuiImageData(scrollbar_top_png);
	scrollbarMid = new GuiImageData(scrollbar_mid_png);
	scrollbarBottom = new GuiImageData(scrollbar_bottom_png);
	scrollbarTopImg = new GuiImage(scrollbarTop);
	scrollbarTopImg->SetParent(this);
	scrollbarTopImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarTopImg->SetPosition(-45, 35);
	scrollbarMidImg = new GuiImage(scrollbarMid);
	scrollbarMidImg->SetParent(this);
	scrollbarMidImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarMidImg->SetPosition(-45, 55);
	scrollbarMidImg->SetTileVertical((s*32-112)/16);
	scrollbarBottomImg = new GuiImage(scrollbarBottom);
	scrollbarBottomImg->SetParent(this);
	scrollbarBottomImg->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarBottomImg->SetPosition(-45, height-57);

	arrowDown = new GuiImageData(arrow_down_png);
	arrowDownImg = new GuiImage(arrowDown);
	arrowDownOver = new GuiImageData(arrow_down_over_png);
	arrowDownOverImg = new GuiImage(arrowDownOver);
	arrowUp = new GuiImageData(arrow_up_png);
	arrowUpImg = new GuiImage(arrowUp);
	arrowUpOver = new GuiImageData(arrow_up_over_png);
	arrowUpOverImg = new GuiImage(arrowUpOver);

	arrowUpBtn = new GuiButton(arrowUpImg->GetWidth(), arrowUpImg->GetHeight());
	arrowUpBtn->SetParent(this);
	arrowUpBtn->SetImage(arrowUpImg);
	arrowUpBtn->SetImageOver(arrowUpOverImg);
	arrowUpBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	arrowUpBtn->SetPosition(-45, 4);
	arrowUpBtn->SetSelectable(false);
	arrowUpBtn->SetTrigger(trigA);

	arrowDownBtn = new GuiButton(arrowDownImg->GetWidth(), arrowDownImg->GetHeight());
	arrowDownBtn->SetParent(this);
	arrowDownBtn->SetImage(arrowDownImg);
	arrowDownBtn->SetImageOver(arrowDownOverImg);
	arrowDownBtn->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	arrowDownBtn->SetPosition(-45, -8);
	arrowDownBtn->SetSelectable(false);
	arrowDownBtn->SetTrigger(trigA);

	if(options->length <= size) // we don't need a scrollbar
	{
		scrollbarTopImg->SetVisible(false);
		scrollbarMidImg->SetVisible(false);
		scrollbarBottomImg->SetVisible(false);
		arrowUpBtn->SetVisible(false);
		arrowUpBtn->SetState(STATE_DISABLED);
		arrowDownBtn->SetVisible(false);
		arrowDownBtn->SetState(STATE_DISABLED);
	}

	for(int i=0; i<size; i++)
	{
		optionTxt[i] = new GuiText(NULL, 18, (GXColor){255, 255, 255, 0xff});
		optionTxt[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		optionTxt[i]->SetPosition(4,-2);

		optionVal[i] = new GuiText(NULL, 18, (GXColor){255, 255, 255, 0xff});
		optionVal[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
		optionVal[i]->SetPosition(340,-2);
		optionVal[i]->SetMaxWidth(screenwidth-340-90);

		optionBg[i] = new GuiImage(bgOptionsEntry);
		optionBg[i]->SetTile(w/16);
		optionBg[i]->SetPosition(-45, 0);

		optionBgOver[i] = new GuiImage(bgOptionsEntryOver);
		optionBgOver[i]->SetTile(w/16);
		optionBgOver[i]->SetPosition(-45, -2);

		optionIcon[i] = new GuiImage;
		optionIcon[i]->SetPosition(0, -2);

		optionBtn[i] = new GuiButton(w-122, 32);
		optionBtn[i]->SetParent(this);
		optionBtn[i]->SetLabel(optionTxt[i], 0);
		optionBtn[i]->SetLabel(optionVal[i], 1);
		optionBtn[i]->SetImage(optionBg[i]);
		optionBtn[i]->SetImageOver(optionBgOver[i]);
		optionBtn[i]->SetIcon(optionIcon[i]);
		optionBtn[i]->SetPosition(45,32*i);
		optionBtn[i]->SetTrigger(trigA);

		optionBtn[i]->SetVisible(false);
		optionBtn[i]->SetState(STATE_DISABLED);
	}
}

/**
 * Destructor for the GuiOptionBrowser class.
 */
GuiOptionBrowser::~GuiOptionBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;

	delete scrollbarTopImg;
	delete scrollbarMidImg;
	delete scrollbarBottomImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;

	delete iconSMB;
	delete iconFTP;
	delete scrollbarTop;
	delete scrollbarMid;
	delete scrollbarBottom;
	delete arrowDown;
	delete arrowDownOver;
	delete arrowUp;
	delete arrowUpOver;

	delete trigA;

	for(int i=0; i<size; i++)
	{
		delete optionTxt[i];
		delete optionVal[i];
		delete optionBg[i];
		delete optionBgOver[i];
		delete optionIcon[i];
		delete optionBtn[i];
	}
}

// overloaded new operator
void *GuiOptionBrowser::operator new(size_t size)
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
void GuiOptionBrowser::operator delete(void *p)
{
	gui_free(p);
}

// overloaded new operator for arrays
void *GuiOptionBrowser::operator new[](size_t size)
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
void GuiOptionBrowser::operator delete[](void *p)
{
	gui_free(p);
}

void GuiOptionBrowser::SetCol1Position(int x)
{
	for(int i=0; i<size; i++)
		optionTxt[i]->SetPosition(x,-2);
}

void GuiOptionBrowser::SetCol2Position(int x)
{
	for(int i=0; i<size; i++)
	{
		optionVal[i]->SetPosition(x,-2);
		optionVal[i]->SetMaxWidth(screenwidth-x-90);
	}
}

void GuiOptionBrowser::ResetState()
{
	if(state != STATE_DISABLED)
	{
		state = STATE_DEFAULT;
		stateChan = -1;
	}

	for(int i=0; i<size; i++)
	{
		optionBtn[i]->ResetState();
	}
}

int GuiOptionBrowser::GetClickedOption()
{
	int found = -1;
	for(int i=0; i<size; i++)
	{
		if(optionBtn[i]->GetState() == STATE_CLICKED)
		{
			optionBtn[i]->SetState(STATE_SELECTED);
			found = optionIndex[i];
			break;
		}
	}
	return found;
}

int GuiOptionBrowser::GetSelectedOption()
{
	int found = -1;
	for(int i=0; i<size; i++)
	{
		if(optionBtn[i]->GetState() == STATE_SELECTED)
		{
			found = optionIndex[i];
			break;
		}
	}
	return found;
}

/****************************************************************************
 * FindMenuItem
 *
 * Help function to find the next visible menu item on the list
 ***************************************************************************/

int GuiOptionBrowser::FindMenuItem(int currentItem, int direction)
{
	int nextItem = currentItem + direction;

	if(nextItem < 0 || nextItem >= options->length)
		return -1;

	if(strlen(options->name[nextItem]) > 0)
		return nextItem;
	else
		return FindMenuItem(nextItem, direction);
}

/**
 * Draw the button on screen
 */
void GuiOptionBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	int next = listOffset;

	for(int i=0; i<size; i++)
	{
		if(next >= 0)
		{
			optionBtn[i]->Draw();
			next = this->FindMenuItem(next, 1);
		}
		else
			break;
	}

	scrollbarTopImg->Draw();
	scrollbarMidImg->Draw();
	scrollbarBottomImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();

	this->UpdateEffects();
}

void GuiOptionBrowser::TriggerUpdate()
{
	listChanged = true;
}

void GuiOptionBrowser::ResetText()
{
	int next = listOffset;

	for(int i=0; i<size; i++)
	{
		if(next >= 0)
		{
			optionBtn[i]->ResetText();
			next = this->FindMenuItem(next, 1);
		}
		else
			break;
	}
}

void GuiOptionBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	int next, prev;

	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);

	next = listOffset;

	if(listChanged)
	{
		listChanged = false;
		for(int i=0; i<size; i++)
		{
			if(next >= 0)
			{
				if(optionBtn[i]->GetState() == STATE_DISABLED || !optionBtn[i]->IsVisible())
				{
					optionBtn[i]->SetVisible(true);
					optionBtn[i]->SetState(STATE_DEFAULT);
				}

				optionTxt[i]->SetText(options->name[next]);
				optionVal[i]->SetText(options->value[next]);

				if(options->icon[next] > 0)
				{
					switch(options->icon[next])
					{
						case ICON_SMB:
							optionIcon[i]->SetImage(iconSMB);
							break;
						case ICON_FTP:
							optionIcon[i]->SetImage(iconFTP);
							break;
						default:
							optionIcon[i]->SetImage(NULL);
					}
				}
				optionIndex[i] = next;
				next = this->FindMenuItem(next, 1);
			}
			else
			{
				optionBtn[i]->SetVisible(false);
				optionBtn[i]->SetState(STATE_DISABLED);
			}
		}
	}

	for(int i=0; i<size; i++)
	{
		if(i != selectedItem && optionBtn[i]->GetState() == STATE_SELECTED)
			optionBtn[i]->ResetState();
		else if(focus && i == selectedItem && optionBtn[i]->GetState() == STATE_DEFAULT)
			optionBtn[selectedItem]->SetState(STATE_SELECTED, -1);

		if(!(t->wpad->btns_h & WPAD_BUTTON_DOWN) && !(t->wpad->btns_h & WPAD_BUTTON_UP))		
		{
			int currChan = t->chan;

			if(t->wpad->ir.valid && !optionBtn[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
				t->chan = -1;

			optionBtn[i]->Update(t);
			t->chan = currChan;
			
			// override state chan
			if(optionBtn[i]->GetStateChan() != -1)
				optionBtn[i]->SetStateChan(-1);
		}

		if(optionBtn[i]->GetState() == STATE_SELECTED)
			selectedItem = i;
	}

	if(t->Down() || arrowDownBtn->GetState() == STATE_CLICKED)
	{
		next = this->FindMenuItem(optionIndex[selectedItem], 1);

		if(next >= 0)
		{
			if(selectedItem == size-1)
			{
				// move list down by 1
				listOffset = this->FindMenuItem(listOffset, 1);
				listChanged = true;
			}
			else if(optionBtn[selectedItem+1]->IsVisible())
			{
				optionBtn[selectedItem]->ResetState();
				optionBtn[selectedItem+1]->SetState(STATE_SELECTED, -1);
				selectedItem++;
			}
		}
		arrowDownBtn->ResetState();
	}
	else if(t->Up() || arrowUpBtn->GetState() == STATE_CLICKED)
	{
		prev = this->FindMenuItem(optionIndex[selectedItem], -1);

		if(prev >= 0)
		{
			if(selectedItem == 0)
			{
				// move list up by 1
				listOffset = prev;
				listChanged = true;
			}
			else
			{
				optionBtn[selectedItem]->ResetState();
				optionBtn[selectedItem-1]->SetState(STATE_SELECTED, -1);
				selectedItem--;
			}
		}
		arrowUpBtn->ResetState();
	}

	if(updateCB)
		updateCB(this);
}
