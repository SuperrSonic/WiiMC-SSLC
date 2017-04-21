/*
   getch2_gekko.c - MPlayer TermIO driver for Wii

   Copyright (C) 2008 dhewg

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <math.h>

#include "config.h"
#include "keycodes.h"
#include "input/input.h"
#include "mp_fifo.h"

int screen_width = 80;
int screen_height = 24;
char *erase_to_end_of_line = NULL;


void get_screen_size() {
}

void getch2_enable() {
}

void getch2_disable() {
}


void getch2(void)
{
}


char* get_term_charset(void) {
	return "ASCII";
}

