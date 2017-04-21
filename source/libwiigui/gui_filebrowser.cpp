/****************************************************************************
 * libwiigui
 *
 * Tantric 2009-2012
 *
 * gui_filebrowser.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"
#include "filebrowser.h"

GuiFileBrowser::GuiFileBrowser(int w, int s)
{
	width = w;
	height = s*32;
	size = s;
	selectedItem = 0;
	selectable = true;
	listChanged = false;
	focus = 0; // allow focus

	trigA = new GuiTrigger;
	trigA->SetSimpleTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	trigHeldA = new GuiTrigger;
	trigHeldA->SetHeldTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);

	bgBrowseEntry = new GuiImageData(bg_entry_png);
	bgBrowseEntryOver = new GuiImageData(bg_entry_over_png);
	iconFolder = new GuiImageData(icon_folder_png);
	iconFolderChecked = new GuiImageData(icon_folder_checked_png);
	iconFile = new GuiImageData(icon_file_png);
	iconFileChecked = new GuiImageData(icon_file_checked_png);
	iconCheck = new GuiImageData(icon_check_png);
	iconPlay = new GuiImageData(icon_play_png);
	iconSD = new GuiImageData(icon_sd_png);
	iconUSB = new GuiImageData(icon_usb_png);
	iconDVD = new GuiImageData(icon_dvd_png);
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
	scrollbarBox = new GuiImageData(scrollbar_box_png);
	scrollbarBoxImg = new GuiImage(scrollbarBox);
	scrollbarBoxOver = new GuiImageData(scrollbar_box_over_png);
	scrollbarBoxOverImg = new GuiImage(scrollbarBoxOver);

	arrowUpBtn = new GuiButton(arrowUpImg->GetWidth(), arrowUpImg->GetHeight());
	arrowUpBtn->SetParent(this);
	arrowUpBtn->SetImage(arrowUpImg);
	arrowUpBtn->SetImageOver(arrowUpOverImg);
	arrowUpBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	arrowUpBtn->SetPosition(-45, 4);
	arrowUpBtn->SetSelectable(false);
	arrowUpBtn->SetClickable(false);
	arrowUpBtn->SetHoldable(true);
	arrowUpBtn->SetTrigger(trigHeldA);

	arrowDownBtn = new GuiButton(arrowDownImg->GetWidth(), arrowDownImg->GetHeight());
	arrowDownBtn->SetParent(this);
	arrowDownBtn->SetImage(arrowDownImg);
	arrowDownBtn->SetImageOver(arrowDownOverImg);
	arrowDownBtn->SetAlignment(ALIGN_RIGHT, ALIGN_BOTTOM);
	arrowDownBtn->SetPosition(-45, -8);
	arrowDownBtn->SetSelectable(false);
	arrowDownBtn->SetClickable(false);
	arrowDownBtn->SetHoldable(true);
	arrowDownBtn->SetTrigger(trigHeldA);

	scrollbarBoxBtn = new GuiButton(scrollbarBoxImg->GetWidth(), scrollbarBoxImg->GetHeight());
	scrollbarBoxBtn->SetParent(this);
	scrollbarBoxBtn->SetImage(scrollbarBoxImg);
	scrollbarBoxBtn->SetImageOver(scrollbarBoxOverImg);
	scrollbarBoxBtn->SetAlignment(ALIGN_RIGHT, ALIGN_TOP);
	scrollbarBoxBtn->SetPosition(-45, 8);
	scrollbarBoxBtn->SetMinY(8);
	scrollbarBoxBtn->SetMaxY(height-128);
	scrollbarBoxBtn->SetSelectable(false);
	scrollbarBoxBtn->SetClickable(false);
	scrollbarBoxBtn->SetHoldable(true);
	scrollbarBoxBtn->SetTrigger(trigHeldA);
	
	// hide scrollbar until we have sufficient entries
	scrollbarTopImg->SetVisible(false);
	scrollbarMidImg->SetVisible(false);
	scrollbarBottomImg->SetVisible(false);
	arrowUpBtn->SetVisible(false);
	arrowDownBtn->SetVisible(false);
	scrollbarBoxBtn->SetVisible(false);
	arrowUpBtn->SetState(STATE_DISABLED);
	arrowDownBtn->SetState(STATE_DISABLED);
	scrollbarBoxBtn->SetState(STATE_DISABLED);

	for(int i=0; i<size; i++)
		MakeEntry(i);
}

GuiFileBrowser::~GuiFileBrowser()
{
	delete arrowUpBtn;
	delete arrowDownBtn;
	delete scrollbarBoxBtn;

	delete scrollbarTopImg;
	delete scrollbarMidImg;
	delete scrollbarBottomImg;
	delete arrowDownImg;
	delete arrowDownOverImg;
	delete arrowUpImg;
	delete arrowUpOverImg;
	delete scrollbarBoxImg;
	delete scrollbarBoxOverImg;

	delete bgBrowseEntry;
	delete bgBrowseEntryOver;
	delete iconFolder;
	delete iconFolderChecked;
	delete iconFile;
	delete iconFileChecked;
	delete iconCheck;
	delete iconPlay;
	delete iconSD;
	delete iconUSB;
	delete iconDVD;
	delete iconSMB;
	delete iconFTP;
	delete scrollbarTop;
	delete scrollbarMid;
	delete scrollbarBottom;
	delete arrowDown;
	delete arrowDownOver;
	delete arrowUp;
	delete arrowUpOver;
	delete scrollbarBox;
	delete scrollbarBoxOver;

	delete trigA;
	delete trigHeldA;

	for(int i=0; i<size; i++)
	{
		delete fileListText[i];
		delete fileList[i];
		delete fileListBg[i];
		delete fileListBgOver[i];
		delete fileListIcon[i];
	}
}

// overloaded new operator
void *GuiFileBrowser::operator new(size_t size)
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
void GuiFileBrowser::operator delete(void *p)
{
	gui_free(p);
}

// overloaded new operator for arrays
void *GuiFileBrowser::operator new[](size_t size)
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
void GuiFileBrowser::operator delete[](void *p)
{
	gui_free(p);
}

void GuiFileBrowser::MakeEntry(int i)
{
	fileListText[i] = new GuiText(NULL, 18, (GXColor){255, 255, 255, 0xff});
	fileListText[i]->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);
	fileListText[i]->SetPosition(32,-2);
	fileListText[i]->SetMaxWidth(width-150);

	fileListBg[i] = new GuiImage(bgBrowseEntry);
	fileListBg[i]->SetTile(width/16);
	fileListBg[i]->SetPosition(-45, 0);

	fileListBgOver[i] = new GuiImage(bgBrowseEntryOver);
	fileListBgOver[i]->SetTile(width/16);
	fileListBgOver[i]->SetPosition(-45, -2);

	fileListIcon[i] = new GuiImage;
	fileListIcon[i]->SetPosition(0, -2);

	fileList[i] = new GuiButton(width-140, 32);
	fileList[i]->SetParent(this);
	fileList[i]->SetLabel(fileListText[i]);
	fileList[i]->SetImage(fileListBg[i]);
	fileList[i]->SetImageOver(fileListBgOver[i]);
	fileList[i]->SetIcon(fileListIcon[i]);
	fileList[i]->SetPosition(45,32*i);
	fileList[i]->SetTrigger(trigA);
	fileList[i]->SetVisible(false);
	fileList[i]->SetState(STATE_DISABLED);
}

void GuiFileBrowser::ResetState()
{
	for(int i=0; i<size; i++)
		fileList[i]->ResetState();

	fileList[0]->SetState(STATE_SELECTED);
	selectedItem = 0;
	stateChan = -1;
	state = STATE_DEFAULT;
	listChanged = true;
}

void GuiFileBrowser::TriggerUpdate()
{
	if(browser.selIndex)
	{
		int pos = browser.selIndex->pos - browser.pageIndex;

		if(pos >= 0 && pos < size)
			selectedItem = pos;
	}

	listChanged = true;
}

void GuiFileBrowser::SetRightCutoff()
{
	for(int i=0; i<size; i++)
	{
		fileListBg[i]->SetTile(width/16-2); 
		fileListBgOver[i]->SetTile(width/16-2); 
	}
}

void GuiFileBrowser::ChangeSize(int newsize)
{
	if(newsize == size)
		return;
	
	if(newsize < size)
	{
		for(int i=newsize; i<size; i++)
		{
			delete fileListText[i];
			delete fileList[i];
			delete fileListBg[i];
			delete fileListBgOver[i];
			delete fileListIcon[i];
		}
	}
	else
	{
		for(int i=size; i<newsize; i++)
			MakeEntry(i);
	}
	size = newsize;
	height = size*32;
	scrollbarMidImg->SetTileVertical((size*32-112)/16);
	scrollbarBottomImg->SetPosition(-45, height-57);
	scrollbarBoxBtn->SetMaxY(height-128);
	listChanged = true;
}

void GuiFileBrowser::Draw()
{
	if(!this->IsVisible())
		return;

	for(int i=0; i<size; ++i)
		fileList[i]->Draw();

	scrollbarTopImg->Draw();
	scrollbarMidImg->Draw();
	scrollbarBottomImg->Draw();
	arrowUpBtn->Draw();
	arrowDownBtn->Draw();
	scrollbarBoxBtn->Draw();
	this->UpdateEffects();
}

void GuiFileBrowser::Update(GuiTrigger * t)
{
	if(state == STATE_DISABLED || !t)
		return;

	int position = 0;
	int positionWiimote = 0;
	int newIndex;

	arrowUpBtn->Update(t);
	arrowDownBtn->Update(t);
	scrollbarBoxBtn->Update(t);

	// move the file listing to respond to wiimote cursor movement
	if(scrollbarBoxBtn->GetState() == STATE_HELD &&
		scrollbarBoxBtn->GetStateChan() == t->chan &&
		t->wpad->ir.valid &&
		browser.numEntries > size
		)
	{
		scrollbarBoxBtn->SetPosition(0,0);
		positionWiimote = t->wpad->ir.y - 60 - scrollbarBoxBtn->GetTop();
		if(positionWiimote < scrollbarBoxBtn->GetMinY())
			positionWiimote = scrollbarBoxBtn->GetMinY();
		else if(positionWiimote > scrollbarBoxBtn->GetMaxY())
			positionWiimote = scrollbarBoxBtn->GetMaxY();

		if(size >= browser.numEntries)
		{
			newIndex = 0;
		}
		else
		{
			int top = scrollbarBoxBtn->GetMinY();
			int range = scrollbarBoxBtn->GetMaxY() - top;
			int at = positionWiimote - top;

			if(at < 0)
				at = 0;

			if(at > range)
				at = range;

			int maxIndex = browser.numEntries - size;
			newIndex = (maxIndex + 1) * at / range;

			if(newIndex > maxIndex)
				newIndex = maxIndex;
		}

		browser.pageIndex = newIndex;
		listChanged = true;
		focus = false;
	}

	if(arrowDownBtn->GetState() == STATE_HELD && arrowDownBtn->GetStateChan() == t->chan)
	{
		t->wpad->btns_d |= WPAD_BUTTON_DOWN;
		if(!this->IsFocused())
			((GuiWindow *)this->GetParent())->ChangeFocus(this);
	}
	else if(arrowUpBtn->GetState() == STATE_HELD && arrowUpBtn->GetStateChan() == t->chan)
	{
		t->wpad->btns_d |= WPAD_BUTTON_UP;
		if(!this->IsFocused())
			((GuiWindow *)this->GetParent())->ChangeFocus(this);
	}

	if(t->Right())
	{
		if(browser.pageIndex < browser.numEntries && browser.numEntries > size)
		{
			newIndex = browser.pageIndex + size;
			if(newIndex+size >= browser.numEntries)
				newIndex = browser.numEntries-size;

			if(newIndex < 0)
				newIndex = 0;

			browser.pageIndex = newIndex;
			listChanged = true;
		}
	}
	else if(t->Left())
	{
		if(browser.pageIndex > 0)
		{
			newIndex = browser.pageIndex - size;

			if(newIndex < 0)
				newIndex = 0;

			browser.pageIndex = newIndex;
			listChanged = true;
		}
	}
	else if(t->Down())
	{
		if(browser.pageIndex + selectedItem + 1 < browser.numEntries)
		{
			if(selectedItem == size-1)
			{
				// move list down by 1
				++browser.pageIndex;
				listChanged = true;
			}
			else if(fileList[selectedItem+1]->IsVisible())
			{
				fileList[selectedItem]->ResetState();
				fileList[++selectedItem]->SetState(STATE_SELECTED, -1);
			}
		}
	}
	else if(t->Up())
	{
		if(selectedItem == 0 &&	browser.pageIndex + selectedItem > 0)
		{
			// move list up by 1
			--browser.pageIndex;
			listChanged = true;
		}
		else if(selectedItem > 0)
		{
			fileList[selectedItem]->ResetState();
			fileList[--selectedItem]->SetState(STATE_SELECTED, -1);
		}
	}

	if(listChanged)
	{
		if(browser.numEntries > size)
		{
			scrollbarTopImg->SetVisible(true);
			scrollbarMidImg->SetVisible(true);
			scrollbarBottomImg->SetVisible(true);
			arrowUpBtn->SetVisible(true);
			arrowDownBtn->SetVisible(true);
			scrollbarBoxBtn->SetVisible(true);

			if(arrowUpBtn->GetState() == STATE_DISABLED)
			{
				arrowUpBtn->SetState(STATE_DEFAULT);
				arrowDownBtn->SetState(STATE_DEFAULT);
				scrollbarBoxBtn->SetState(STATE_DEFAULT);
			}
		}
		else
		{
			scrollbarTopImg->SetVisible(false);
			scrollbarMidImg->SetVisible(false);
			scrollbarBottomImg->SetVisible(false);
			arrowUpBtn->SetVisible(false);
			arrowDownBtn->SetVisible(false);
			scrollbarBoxBtn->SetVisible(false);
			arrowUpBtn->SetState(STATE_DISABLED);
			arrowDownBtn->SetState(STATE_DISABLED);
			scrollbarBoxBtn->SetState(STATE_DISABLED);
		}
	}

	BROWSERENTRY *entry = PositionToEntry(&browser, browser.pageIndex);
	char disp[255];

	for(int i=0; i<size ; ++i)
	{
		if(listChanged)
		{
			if(entry)
			{
				if(fileList[i]->GetState() == STATE_DISABLED)
					fileList[i]->SetState(STATE_DEFAULT);

				fileList[i]->SetVisible(true);

				GetDisplay(entry, disp, 255);
				fileListText[i]->SetText(disp);

				switch(entry->icon)
				{
					case ICON_FOLDER:
						fileListIcon[i]->SetImage(iconFolder);
						break;
					case ICON_FOLDER_CHECKED:
						fileListIcon[i]->SetImage(iconFolderChecked);
						break;
					case ICON_FILE:
						fileListIcon[i]->SetImage(iconFile);
						break;
					case ICON_FILE_CHECKED:
						fileListIcon[i]->SetImage(iconFileChecked);
						break;
					case ICON_CHECK:
						fileListIcon[i]->SetImage(iconCheck);
						break;
					case ICON_PLAY:
						fileListIcon[i]->SetImage(iconPlay);
						break;
					case ICON_SD:
						fileListIcon[i]->SetImage(iconSD);
						break;
					case ICON_USB:
						fileListIcon[i]->SetImage(iconUSB);
						break;
					case ICON_DVD:
						fileListIcon[i]->SetImage(iconDVD);
						break;
					case ICON_SMB:
						fileListIcon[i]->SetImage(iconSMB);
						break;
					case ICON_FTP:
						fileListIcon[i]->SetImage(iconFTP);
						break;
					default:
						fileListIcon[i]->SetImage(NULL);
						break;
				}
			}
			else
			{
				fileList[i]->SetVisible(false);
				fileList[i]->SetState(STATE_DISABLED);
			}
		}

		if(i != selectedItem && fileList[i]->GetState() == STATE_SELECTED)
			fileList[i]->ResetState();
		else if(i == selectedItem && fileList[i]->GetState() == STATE_DEFAULT)
			fileList[selectedItem]->SetState(STATE_SELECTED, -1);
		
		if(!(t->wpad->btns_h & WPAD_BUTTON_DOWN) && !(t->wpad->btns_h & WPAD_BUTTON_UP))
		{
			int currChan = t->chan;
			if(t->wpad->ir.valid && !fileList[i]->IsInside(t->wpad->ir.x, t->wpad->ir.y))
				t->chan = -1;
			fileList[i]->Update(t);
			t->chan = currChan;

			// override state chan
			if(fileList[i]->GetStateChan() != -1)
				fileList[i]->SetStateChan(-1);
		}

		if(fileList[i]->GetState() == STATE_SELECTED)
		{
			selectedItem = i;
			if(entry) browser.selIndex = entry;
		}

		if(selectedItem == i)
			fileListText[i]->SetScroll(SCROLL_HORIZONTAL);
		else
			fileListText[i]->SetScroll(SCROLL_NONE);
		if(entry) entry = entry->next;
	}

	// update the location of the scroll box based on the position in the file list
	if(positionWiimote > 0)
	{
		position = positionWiimote; // follow wiimote cursor
		scrollbarBoxBtn->SetPosition(-45,position+30);
	}
	else if(listChanged)
	{
		int top = scrollbarBoxBtn->GetMinY();

		if(browser.numEntries <= size)
		{
			position = top;
		}
		else
		{
			int range = scrollbarBoxBtn->GetMaxY() - top;
			position = top + range * browser.pageIndex / (browser.numEntries - size);
		}

		scrollbarBoxBtn->SetPosition(-45, position+30);
	}

	listChanged = false;

	if(updateCB)
		updateCB(this);
}
