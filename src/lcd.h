/*
	Copyright 2012, 2013 Charles O.
	Email: charles.0x4f@gmail.com
	Github: https://github.com/charles-0x4f/

	This file is part of TermGB.

	TermGB is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	TermGB is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with TermGB.  If not, see <http://www.gnu.org/licenses/>.
*/

/* lcd.h */

#ifndef LCD_H
#define LCD_H

#include "memory.h"

/* This will hold the number of cycles until the next scanline */
int scanline_cycles;


/* +++++ FUNCTIONS +++++ */
void LCD_init();
void LCD_update(int);
void LCD_update_status();
byte LCD_enabled();
byte LCD_get_mode();

#endif
