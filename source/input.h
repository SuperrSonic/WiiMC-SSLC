/****************************************************************************
 * WiiMC
 * Tantric 2009-2012
 *
 * input.h
 * Wii/GameCube controller management
 ***************************************************************************/

#ifndef _INPUT_H_
#define _INPUT_H_

#include <gccore.h>
#include <wiiuse/wpad.h>

#define PI 				3.14159265f
#define PADCAL			50

void SetupPads();
void UpdatePads();
void DisableRumble();
void EnableRumble();
void ShutoffRumble();
void RequestRumble(int i);
void DoRumble(int i);
void MPlayerInput();

#endif
