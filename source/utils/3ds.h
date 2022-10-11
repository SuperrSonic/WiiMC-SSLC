/* 
 * Copyright (c) 2015-2022, Extrems' Corner.org
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef GBI_3DS_H
#define GBI_3DS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <network.h>

#define CTR_BUTTON_A       0x00000001
#define CTR_BUTTON_B       0x00000002
#define CTR_BUTTON_SELECT  0x00000004
#define CTR_BUTTON_START   0x00000008
#define CTR_BUTTON_RIGHT   0x00000010
#define CTR_BUTTON_LEFT    0x00000020
#define CTR_BUTTON_UP      0x00000040
#define CTR_BUTTON_DOWN    0x00000080
#define CTR_BUTTON_R       0x00000100
#define CTR_BUTTON_L       0x00000200
#define CTR_BUTTON_X       0x00000400
#define CTR_BUTTON_Y       0x00000800
#define CTR_BUTTON_ZL      0x00004000
#define CTR_BUTTON_ZR      0x00008000

#define CTR_TOUCH          0x00100000

#define CTR_SUBSTICK_RIGHT 0x01000000
#define CTR_SUBSTICK_LEFT  0x02000000
#define CTR_SUBSTICK_UP    0x04000000
#define CTR_SUBSTICK_DOWN  0x08000000
#define CTR_STICK_RIGHT    0x10000000
#define CTR_STICK_LEFT     0x20000000
#define CTR_STICK_UP       0x40000000
#define CTR_STICK_DOWN     0x80000000

typedef struct {
	struct {
		int sd;
		int nb;
		struct sockaddr_in sin;
		socklen_t sinlen;
	} sv;

	struct timespec tv;

	struct {
		uint32_t held, last, down, up;
		struct { uint16_t x, y; } touch;
		struct { int16_t x, y; } stick;
		struct { int16_t x, y; } substick;
		struct { float x, y, z; } gyro;
		struct { float x, y, z; } gforce;
		struct { float roll, pitch; } orient;
	} data;
} ctr_state_t;

extern ctr_state_t ctr;

typedef struct {
	uint16_t magic;
	uint16_t version;
	uint32_t up, down, held;
	struct { uint16_t x, y; } touch;
	struct { int16_t x, y; } stick;
	struct { int16_t x, y; } substick;
	struct { int16_t x, y, z; } gyro;
	struct { int16_t x, y, z; } accel;
} ATTRIBUTE_PACKED ctr_packet_t;

void CTRScanPads(void);
bool CTRInit(void);

#ifdef __cplusplus
}
#endif

#endif /* GBI_3DS_H */
