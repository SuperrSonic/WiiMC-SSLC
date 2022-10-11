/* 
 * Copyright (c) 2015-2022, Extrems' Corner.org
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <math.h>
#include <gccore.h>
#include <ogc/machine/processor.h>
#include "3ds.h"
#include "network.h"
#include "state.h"

static syswd_t alarm = SYS_WD_NULL;

ctr_state_t ctr = {
	.sv.sd = INVALID_SOCKET,
	.sv.nb = 1,
	.sv.sin.sin_family = AF_INET,
	.sv.sin.sin_port = 15708,
	.sv.sin.sin_addr.s_addr = INADDR_ANY,
	.sv.sinlen = sizeof(struct sockaddr),

	.tv.tv_sec  = 1,
	.tv.tv_nsec = 0,
};

static void timeout_cb(syswd_t alarm, void *arg)
{
	ctr.data.held = 0;
	ctr.data.touch.x = 0;
	ctr.data.touch.y = 0;
	ctr.data.stick.x = 0;
	ctr.data.stick.y = 0;
	ctr.data.substick.x = 0;
	ctr.data.substick.y = 0;
	ctr.data.gyro.x = 0;
	ctr.data.gyro.y = 0;
	ctr.data.gyro.z = 0;
	ctr.data.gforce.x = 0;
	ctr.data.gforce.y = 0;
	ctr.data.gforce.z = 0;
}

void CTRScanPads(void)
{
	ctr_packet_t packet = {0};
	ctr.data.last = ctr.data.held;

	while (ctr.sv.sd != INVALID_SOCKET) {
		int ret = net_read(ctr.sv.sd, &packet, sizeof(packet));
		if (ret < 0) break;
		if (packet.magic == 0x3D5C) {
			SYS_CancelAlarm(alarm);
			ctr.data.held = packet.held;
			ctr.data.touch.x = packet.touch.x;
			ctr.data.touch.y = packet.touch.y;
			ctr.data.stick.x = packet.stick.x;
			ctr.data.stick.y = packet.stick.y;
			ctr.data.substick.x = packet.substick.x;
			ctr.data.substick.y = packet.substick.y;
			ctr.data.gyro.x = -packet.gyro.x / 14.375;
			ctr.data.gyro.y = -packet.gyro.y / 14.375;
			ctr.data.gyro.z = -packet.gyro.z / 14.375;
			ctr.data.gforce.x = -packet.accel.x / 512.;
			ctr.data.gforce.y = -packet.accel.y / 512.;
			ctr.data.gforce.z = -packet.accel.z / 512.;
			SYS_SetAlarm(alarm, &ctr.tv, timeout_cb, NULL);
		}
	}

	ctr.data.down = ctr.data.held & ~ctr.data.last;
	ctr.data.up = ~ctr.data.held & ctr.data.last;

	if (ctr.data.gforce.z >= -1. &&
		ctr.data.gforce.z <=  1.) {
		if (ctr.data.gforce.x >= -1. &&
			ctr.data.gforce.x <=  1.)
			ctr.data.orient.roll = atan2f(ctr.data.gforce.x, ctr.data.gforce.z);
		if (ctr.data.gforce.y >= -1. &&
			ctr.data.gforce.y <=  1.)
			ctr.data.orient.pitch = atan2f(ctr.data.gforce.y, ctr.data.gforce.z);
	}
}

bool CTRInit(void)
{
	ctr.sv.sd = net_socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

	if (ctr.sv.sd == INVALID_SOCKET)
		goto fail;
	if (net_ioctl(ctr.sv.sd, FIONBIO, &ctr.sv.nb) < 0)
		goto fail;
	if (net_bind(ctr.sv.sd, (struct sockaddr *)&ctr.sv.sin, ctr.sv.sinlen) < 0)
		goto fail;

	SYS_CreateAlarm(&alarm);
	return true;

fail:
	net_close(ctr.sv.sd);
	ctr.sv.sd = INVALID_SOCKET;
	return false;
}
