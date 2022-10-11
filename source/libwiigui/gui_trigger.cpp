/****************************************************************************
 * libwiigui
 *
 * Tantric 2009-2012
 *
 * gui_trigger.cpp
 *
 * GUI class definitions
 ***************************************************************************/

#include "gui.h"
#include <ogc/lwp_watchdog.h>
#include <gctypes.h>

static u64 prev[4];
static u64 now[4];
static u32 delay[4];

static u8 stickTimer = 0;

/**
 * Constructor for the GuiTrigger class.
 */
GuiTrigger::GuiTrigger()
{
	chan = -1;
	memset(&wpaddata, 0, sizeof(WPADData));
	memset(&pad, 0, sizeof(PADData));
	memset(&cpad, 0, sizeof(ctr_state_t));
	wpad = &wpaddata;
}

/**
 * Destructor for the GuiTrigger class.
 */
GuiTrigger::~GuiTrigger()
{
}

// overloaded new operator
void *GuiTrigger::operator new(size_t size)
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
void GuiTrigger::operator delete(void *p)
{
	gui_free(p);
}

// overloaded new operator for arrays
void *GuiTrigger::operator new[](size_t size)
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
void GuiTrigger::operator delete[](void *p)
{
	gui_free(p);
}

/**
 * Sets a simple trigger. Requires:
 * - Element is selected
 * - Trigger button is pressed
 */
void GuiTrigger::SetSimpleTrigger(s32 ch, u32 wiibtns, u16 gcbtns, uint32_t ctrbtns)
{
	type = TRIGGER_SIMPLE;
	chan = ch;
	wpaddata.btns_d = wiibtns;
	pad.btns_d = gcbtns;
	cpad.data.down = ctrbtns;
}

/**
 * Sets a held trigger. Requires:
 * - Element is selected
 * - Trigger button is pressed and held
 */
void GuiTrigger::SetHeldTrigger(s32 ch, u32 wiibtns, u16 gcbtns, uint32_t ctrbtns)
{
	type = TRIGGER_HELD;
	chan = ch;
	wpaddata.btns_h = wiibtns;
	pad.btns_h = gcbtns;
	cpad.data.held = ctrbtns;
}

/**
 * Sets a button trigger. Requires:
 * - Trigger button is pressed
 */
void GuiTrigger::SetButtonOnlyTrigger(s32 ch, u32 wiibtns, u16 gcbtns, uint32_t ctrbtns)
{
	type = TRIGGER_BUTTON_ONLY;
	chan = ch;
	wpaddata.btns_d = wiibtns;
	pad.btns_d = gcbtns;
	cpad.data.down = ctrbtns;
}

/**
 * Sets a button trigger. Requires:
 * - Trigger button is pressed
 * - Parent window is in focus
 */
void GuiTrigger::SetButtonOnlyInFocusTrigger(s32 ch, u32 wiibtns, u16 gcbtns, uint32_t ctrbtns)
{
	type = TRIGGER_BUTTON_ONLY_IN_FOCUS;
	chan = ch;
	wpaddata.btns_d = wiibtns;
	pad.btns_d = gcbtns;
	cpad.data.down = ctrbtns;
}

/****************************************************************************
 * WPAD_Stick
 *
 * Get X/Y value from Wii Joystick (classic, nunchuk) input
 ***************************************************************************/

s8 GuiTrigger::WPAD_Stick(u8 stick, int axis)
{
	#ifdef HW_RVL

	float mag = 0.0;
	float ang = 0.0;

	switch (wpad->exp.type)
	{
		case WPAD_EXP_NUNCHUK:
		case WPAD_EXP_GUITARHERO3:
			if (stick == 0)
			{
				mag = wpad->exp.nunchuk.js.mag;
				ang = wpad->exp.nunchuk.js.ang;
			}
			break;

		case WPAD_EXP_CLASSIC:
			if (stick == 0)
			{
				mag = wpad->exp.classic.ljs.mag;
				ang = wpad->exp.classic.ljs.ang;
			}
			else
			{
				mag = wpad->exp.classic.rjs.mag;
				ang = wpad->exp.classic.rjs.ang;
			}
			break;

		default:
			break;
	}

	/* calculate x/y value (angle need to be converted into radian) */
	if (mag > 1.0) mag = 1.0;
	else if (mag < -1.0) mag = -1.0;
	double val;

	if(axis == 0) // x-axis
		val = mag * sin((PI * ang)/180.0f);
	else // y-axis
		val = mag * cos((PI * ang)/180.0f);

	return (s8)(val * 128.0f);

	#else
	return 0;
	#endif
}

s8 GuiTrigger::WPAD_StickX(u8 stick)
{
	return WPAD_Stick(stick, 0);
}

s8 GuiTrigger::WPAD_StickY(u8 stick)
{
	return WPAD_Stick(stick, 1);
}

bool GuiTrigger::Left()
{
	u32 wiibtn = WPAD_BUTTON_LEFT;

	if((wpad->btns_d | wpad->btns_h) & (wiibtn | WPAD_CLASSIC_BUTTON_LEFT)
			|| (pad.btns_d | pad.btns_h) & PAD_BUTTON_LEFT
			|| (cpad.data.down | cpad.data.held) & CTR_BUTTON_LEFT
			|| (cpad.data.down | cpad.data.held) & CTR_STICK_LEFT
			|| pad.stickX < -PADCAL
			|| WPAD_StickX(0) < -PADCAL)
	{
		if(wpad->btns_d & (wiibtn | WPAD_CLASSIC_BUTTON_LEFT)
			|| pad.btns_d & PAD_BUTTON_LEFT || cpad.data.down & CTR_BUTTON_LEFT || cpad.data.down & CTR_STICK_LEFT)
		{
			prev[chan] = gettime();
			delay[chan] = SCROLL_DELAY_INITIAL; // reset scroll delay
			return true;
		}

		now[chan] = gettime();

		if(diff_usec(prev[chan], now[chan]) > delay[chan])
		{
			prev[chan] = now[chan];
			
			if(delay[chan] == SCROLL_DELAY_INITIAL)
				delay[chan] = SCROLL_DELAY_LOOP;
			else if(delay[chan] > SCROLL_DELAY_DECREASE)
				delay[chan] -= SCROLL_DELAY_DECREASE;
			return true;
		}
	}
	return false;
}

bool GuiTrigger::Right()
{
	u32 wiibtn = WPAD_BUTTON_RIGHT;

	if((wpad->btns_d | wpad->btns_h) & (wiibtn | WPAD_CLASSIC_BUTTON_RIGHT)
			|| (pad.btns_d | pad.btns_h) & PAD_BUTTON_RIGHT
			|| (cpad.data.down | cpad.data.held) & CTR_BUTTON_RIGHT
			|| (cpad.data.down | cpad.data.held) & CTR_STICK_RIGHT
			|| pad.stickX > PADCAL
			|| WPAD_StickX(0) > PADCAL)
	{
		if(wpad->btns_d & (wiibtn | WPAD_CLASSIC_BUTTON_RIGHT)
			|| pad.btns_d & PAD_BUTTON_RIGHT || cpad.data.down & CTR_BUTTON_RIGHT || cpad.data.down & CTR_STICK_RIGHT)
		{
			prev[chan] = gettime();
			delay[chan] = SCROLL_DELAY_INITIAL; // reset scroll delay
			return true;
		}

		now[chan] = gettime();

		if(diff_usec(prev[chan], now[chan]) > delay[chan])
		{
			prev[chan] = now[chan];
			
			if(delay[chan] == SCROLL_DELAY_INITIAL)
				delay[chan] = SCROLL_DELAY_LOOP;
			else if(delay[chan] > SCROLL_DELAY_DECREASE)
				delay[chan] -= SCROLL_DELAY_DECREASE;
			return true;
		}
	}
	return false;
}

bool GuiTrigger::Up()
{
	u32 wiibtn = WPAD_BUTTON_UP;

	++stickTimer;
	if(pad.stickY > PADCAL && stickTimer > 50) {
		pad.btns_d = PAD_BUTTON_UP;
		stickTimer = 0;
	} else if(WPAD_StickY(0) > PADCAL && stickTimer > 50) {
		pad.btns_d = PAD_BUTTON_UP;
		stickTimer = 0;
	}

	if((wpad->btns_d | wpad->btns_h) & (wiibtn | WPAD_CLASSIC_BUTTON_UP)
			|| (pad.btns_d | pad.btns_h) & PAD_BUTTON_UP
			|| (cpad.data.down | cpad.data.held) & CTR_BUTTON_UP
			|| (cpad.data.down | cpad.data.held) & CTR_STICK_UP)
	{
		if(wpad->btns_d & (wiibtn | WPAD_CLASSIC_BUTTON_UP)
			|| pad.btns_d & PAD_BUTTON_UP || cpad.data.down & CTR_BUTTON_UP || cpad.data.down & CTR_STICK_UP)
		{
			prev[chan] = gettime();
			delay[chan] = SCROLL_DELAY_INITIAL; // reset scroll delay
			return true;
		}

		now[chan] = gettime();

		if(diff_usec(prev[chan], now[chan]) > delay[chan])
		{
			prev[chan] = now[chan];
			
			if(delay[chan] == SCROLL_DELAY_INITIAL)
				delay[chan] = SCROLL_DELAY_LOOP;
			else if(delay[chan] > SCROLL_DELAY_DECREASE)
				delay[chan] -= SCROLL_DELAY_DECREASE;
			return true;
		}
	}
	return false;
}

bool GuiTrigger::Down()
{
	u32 wiibtn = WPAD_BUTTON_DOWN;

	++stickTimer;
	if(stickTimer > 50 && pad.stickY < -PADCAL) {
		pad.btns_d = PAD_BUTTON_DOWN;
		stickTimer = 0;
	} else if(WPAD_StickY(0) < -PADCAL && stickTimer > 50) {
		pad.btns_d = PAD_BUTTON_DOWN;
		stickTimer = 0;
	}

	if((wpad->btns_d | wpad->btns_h) & (wiibtn | WPAD_CLASSIC_BUTTON_DOWN)
			|| (pad.btns_d | pad.btns_h) & PAD_BUTTON_DOWN
			|| (cpad.data.down | cpad.data.held) & CTR_BUTTON_DOWN
			|| (cpad.data.down | cpad.data.held) & CTR_STICK_DOWN)
	{
		if(wpad->btns_d & (wiibtn | WPAD_CLASSIC_BUTTON_DOWN)
			|| pad.btns_d & PAD_BUTTON_DOWN || cpad.data.down & CTR_BUTTON_DOWN || cpad.data.down & CTR_STICK_DOWN)
		{
			prev[chan] = gettime();
			delay[chan] = SCROLL_DELAY_INITIAL; // reset scroll delay
			return true;
		}

		now[chan] = gettime();

		if(diff_usec(prev[chan], now[chan]) > delay[chan])
		{
			prev[chan] = now[chan];
			
			if(delay[chan] == SCROLL_DELAY_INITIAL)
				delay[chan] = SCROLL_DELAY_LOOP;
			else if(delay[chan] > SCROLL_DELAY_DECREASE)
				delay[chan] -= SCROLL_DELAY_DECREASE;
			return true;
		}
	}
	return false;
}

