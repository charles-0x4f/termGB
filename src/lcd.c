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

/* LCD.c */

/* TODO make this file suck less */
#include "lcd.h"


/* Initialize the LCD, obviously */
void LCD_init()
{
	scanline_cycles = 456;
}

/* Main graphical function

	TODO refactor this.

	Big thanks to Codeslinger for the current iteration of this
	function.
	http://www.codeslinger.co.uk
*/
void LCD_update(int cycles)
{
	byte currentline;

	LCD_update_status();

	if(LCD_enabled())
		scanline_cycles -= cycles;
	else
		return;

	if(scanline_cycles <= 0)
	{
		memory[0xFF44]++;

		currentline = memory[0xFF44];

		scanline_cycles = 456;

		if(currentline == 144)
		{
			/* Request V-blank interrupt */
			CPU_request_interrupt(0);
		}
		else if(currentline > 153)
		{
			memory[0xFF44] = 0;
		}
		else if(currentline <= 144)
		{
			GL_draw_scanline();
		}
	}
}

/*
	Update the LCD status register(0xFF41)

	(Put more info here)
	TODO write the 90 page comment for this function
*/
void LCD_update_status()
{
	byte mode, currentmode, requestint = 0;
	byte current_status = memory_readb(0xFF41);
	byte currentline = memory_readb(0xFF44);
	byte mode_offset = 456 - scanline_cycles;

	/*
		Get the current mode

		current_status = ????-??XX
		& 0x3 = get             ^^
	*/
	currentmode = LCD_get_mode();

	/*
		If we're at scanline >= 144, we're in V-blank
		and need to set the mode to 01, check interrupt
		bit, if it's set, then request the LCD interrupt
	*/
	if(currentline >= 144)
	{
		mode = 1;
		requestint = bitset(&current_status, 4);
	}

	/*
		If mode_offset <= 204, we're in H-blank and need
		to set mode to 00, check interrupt bit, if set,
		request LCD interrupt
	*/ 
	if(mode_offset <= 204)
	{
		mode = 0;
		requestint = bitset(&current_status, 3);
	}
	/*
		If the offset is >= 376, we're in mode 2, which
		is when the Gameboy is searching sprite attribute
		data(OAM) and the CPU can't access the data there
	*/
	else if(mode_offset >= 376)
	{
		mode = 2;
		requestint = bitset(&current_status, 5);
	}
	/*
		If the offset is somewhere between, then we're in
		mode 3, when data is being transferred to the LCD.
	*/
	else
	{
		mode = 3;
	}

	/*
		See if mode is different than it was before, indicating
		a mode change, if there was a change, and the corresponding
		interrupt bit is set, request the appropriate interrupt.

		We make sure that the mode has actually changed so that
		we're not requesting an interrupt 79 million times a frame
	*/
	if((mode != currentmode) && requestint > 0)
	{
		/* Request LCD interrupt (bit 1, duh) */
		CPU_request_interrupt(1);
	}

	/*
		Don't bother to change the mode bits if they're gonna
		be the same.
	*/
	if(mode != currentmode)
	{
		/* Switch to handle the bit-level mode setting */
		switch(mode)
		{
			/*
				H-blank, mode 0.
				bit-level: ...00
			*/
			case 0:
			{
				CPU_res(0, &currentmode);
				CPU_res(1, &currentmode);
				break;
			}
			/*
				V-blank, mode 1.
				bit-level: ...01
			*/
			case 1:
			{
				CPU_set(0, &currentmode);
				CPU_res(1, &currentmode);
				break;
			}
			/*
				OAM search, mode 2.
				bit-level: ...10
			*/
			case 2:
			{
				CPU_res(0, &currentmode);
				CPU_set(1, &currentmode);
				break;
			}
		}
	}

	/*
		If the current line is the same as LYC(0xFF45),
		set the coincidence flag. If bit 6 of the status
		register is set, request an LCD interrupt.

		If they're not equal, reset the coincidence bit.
	*/
	if(currentline == memory_readb(0xFF45))
	{
		CPU_set(2, &currentmode);

		/* TODO change to CPU_bit when defined */
		if(bitset(&currentmode, 6))
			CPU_request_interrupt(1);
	}
	else
	{
		CPU_res(2, &currentmode);
	}

	/*
		Finally, write the updated currentmode to the real
		status register (0xFF41)
	*/
	memory_writeb(0xFF41, currentmode);
}

/*
	Check to see if the LCD is enabled

	Returns:
	0 - LCD not enabled, probably Vblank
	1 - LCD enabled
*/
byte LCD_enabled()
{
	/* TODO change this function to use the CPU's bit functions when they're done */
	byte LCDstatus = memory_readb(0xFF40);

	/* get bit 7 from 0xFF40, if set, LCD is enabled */
	if(LCDstatus & 0x80)
		return 1;
	else
		return 0;
}

/*
	Return the current LCD mode
	(bit 0 and 1 of the LCD status register 0xFF41)
*/
byte LCD_get_mode()
{
	byte ret;

	/* Read in the status register */
	ret = memory_readb(0xFF41);

	/*
		Get just bit 0 and 1 of the status register
		ret = 1010-1010
		ret = 0000-00^^
	*/
	ret = ret & 0x3;

	return ret;
}
