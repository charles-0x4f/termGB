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

#include <stdio.h>
#include "gl.h"

/*
	This function replicates the DMA transfer the Gameboy does when
	a game/program writes to register 0xFF46. The DMA transfer is usually
	called from within the V-blank when the CPU has access to all of
	the available RAM and transfers 0xA0 bytes of data at the address
	defined by the byte written to 0xFF46 to the OAM (sprite attribute
	memory) at 0xFE00-0xFE9F.

	The byte written to 0xFF46 is a little odd, the source address the
	data is copied from must be an interval of 0x100, anywhere from
	0x0000 to 0xF100.

	So, the source address must be multiplied by 100 to find the real
	source address.

	Example:
	a = 0xA0
	load a into 0xFF46
	real source address = a(0xA0) * 100
	real source address = 0x3E80
	DMA: transfer all bytes from 0x3E80 to 0x3F1F(0xA0 bytes)
		to OAM (0xFE00 through 0xFE9F)
*/
void GL_dma(byte source)
{
	int x;
	word address = (word)source * 100;

	for(x = 0; x < 0xA0; x++)
	{
		memory_writeb((0xFE00 + x), memory_readb(address + x));
	}
}

/*
	Return true color value of TODO finish this
*/
byte GL_get_bit_color(byte color)
{
	byte final_color, palette;

	palette = memory_readb(0xFF47);

	if(color > 0)
		printf("DBG VIDEO BIT COLOR: %x\n", color);
	printf("DBG VIDEO PALETTE: %x\n", palette);

	switch(color)
	{
		case 0:
		{
			/*
				If color is 0, we need to look for the bit
				code in bit 0-1 of the palette.
			*/
			final_color = palette & 0x3;

			break;
		}
		case 1:
		{
			/*
				Color == 1 (01) is associated with bit 2-3 of
				the palette.
			*/
			final_color = palette & 0xC;
			final_color >>= 2;

			break;
		}
		case 2:
		{
			/*
				Color == 2 (10) is associated with bit 4-5
				of the color palette.
			*/
			final_color = palette & 0x30;
			final_color >>= 4;

			break;
		}
		case 3:
		{
			/*
				Color == 3 (11) is associated with bit 6-7
				of the color palette.
			*/
			final_color = palette & 0xC0;
			final_color >>= 6;

			break;
		}
	}

	if(final_color > 0)
		printf("DBG VIDEO FINAL COLOR: %i\n", final_color);

	return final_color;
}

void GL_draw_scanline()
{
	byte LCD_status, LCD_control;

	LCD_status = memory_readb(0xFF41);
	LCD_control = memory_readb(0xFF40);

	/*
		If bit 0 of LCD control is set, then the background
		is enabled, and we should draw them.
	*/
	if(bitset(&LCD_control, 0))
		GL_draw_tiles();

	/*
		If bit 1 of the LCD control is set, then sprites are
		enabled and we should draw them.
	*/
	if(bitset(&LCD_control, 1))
	{
		/*GL_draw_sprites();*/
	}

	/*
		Now that the video buffer has been filled for this scanline,
		go ahead and start the video library and put that data on
		the screen.
	*/
	GL_SDL_draw_scanline();
}

/*
	Fill video buffer with appropriate pixel data from this scanline
	so that it can be rendered to the screen later.
*/
/*void GL_draw_tiles()
{*/
	/*
		Store memory regions for the maps and data

		Map: A region of memory that contains a number that
			corresponds to a particular tile to be displayed

		Data: A region of memory that contains the actual tile
			data that the map makes reference to
	*/
/*	word bg_map, bg_data, win_map, tileX, tileY, tile_location;
	word temp_address;*/

	/*
		The background is 32*32 tiles (256*256 pixels) in total.

		Only part of this can be displayed on the 160*144 screen.
		We'll call this the viewing area. ScrollX and ScrollY are
		registers that determine where in this background the
		viewing area is showing.

		In addition to this, we have the window, which is drawn on
		top of the background. The window is positioned relative
		to the viewing area and doesn't move. WinX and WinY are
		where in the viewing area the window should be drawn.

		(Note: to draw the window in the top left position, WinX
			must be 7 and WinY must be 0. The window X position
			is offset by 7 for reasons unknown at this time)
	*/ 
/*	byte scrollX, scrollY, winX, winY;*/

	/*
		drawWindow - Are we drawing the window in this scanline?
			Used as a boolean.

		scanline - What scanline we're currently drawing

		usingSigned - Certain tile data regions use signed numbers,
			this will designate if we're using one. Used as
			boolean

		LCDC - Used to store the LCD Control register

		pixelLoc - Where on the scanline we're smactually drawing
	*/
/*	byte drawWindow, scanline, usingSigned, LCDC, pixelLoc;*/

	/*
		These will be used to store actual tile data
	*/
/*	byte tile_ident, tile_byte1, tile_byte2, tile_line, tile_color;
	byte tile_offset, true_color;*/

	/* Assume we're not using the window until proven guilty */
/*	drawWindow = 0;*/
	/* Get current ScrollX (register 0xFF43) */
/*	scrollX = memory_readb(0xFF43);*/
	/* Get current ScrollY (register 0xFF42) */
/*	scrollY = memory_readb(0xFF42);*/
	/* Get current scanline (register LY: 0xFF44) */
/*	scanline = memory_readb(0xFF44);*/
	/* Get current LCDC (register 0xFF40) */
/*	LCDC = memory_readb(0xFF40);*/

	/* Check if the window is enabled (bit 5 in the LCDC) */
/*	if(bitset(&LCDC, 5))
	{*/
		/*
			See if the window will be being drawn this scanline:

			If winY <= scanline, then the window may be being
			drawn, if the Y position of the window is greater
			than the current scanline, then the window has not
			come into play yet and drawWindow will remain reset
			(false)
		*/
/*		if(winY <= scanline)
			drawWindow = 1;*/

		/*
			If window is enabled, check to see where the
			window map is located.
			-TODO figure out what the ****'s up with the
				window being in the bg map

			Bit 6 of the LCDC register determines the location
			of the window map.

			0 - 0x9800-9BFF
			1 - 0x9C00-9FFF
		*/
/*		if(bitset(&LCDC, 6))
			bg_map = 0x9C00;
		else
			bg_map = 0x9800;
	}
	else
	{*/
		/*
			If window not enabled, the bg map will be determined
			by a different bit, though the locations will be the
			same per bit as if the window was enabled

			(Bit 3 of LCDC is the non-windowed BG map select)
		*/
/*		if(bitset(&LCDC, 3))
			bg_map = 0x9C00;
		else
			bg_map = 0x9800;
	}*/

	/*
		Now that we have the map locations figured out, let's deal
		with the tile data locations.

		There are two locations the tile data can be, this can be
		determined by bit 4 in the LCDC.

		0: 0x8800-0x97FF
		1: 0x8000-0x8FFF

		One major thing to keep in mind with the tile data is that
		if bit 4 is 0, then the tile data TODO ? will be signed, not
		unsigned
	*/
/*	if(bitset(&LCDC, 4))
	{
		bg_data = 0x8000;
		usingSigned = 0;
	}
	else
	{
		bg_data = 0x8800;
		usingSigned = 1;
	}*/

	/*
		Get the overall vertical tile we're scanning, since
		tileY is a byte type, this should just give us the
		rounded vertical tile number
	*/
/*	tileY = ((scrollY + scanline) / 8);*/
	/*printf("DBG VIDEO TILEY: %i\n", tileY);*/

/*	for(pixelLoc = 0; pixelLoc < 160; pixelLoc++)
	{*/
		/*
			Now we need to find out a bunch of information
			before we can start drawing to the video buffer.

			First we need to determine which horizontal and
			vertical tile in the overall background map (not
			just the viewing area, but the complete 32*32 map)
			we're at.
		*/
/*		tileX = ((pixelLoc + scrollX) / 8);*/
		/*printf("DBG VIDEO TILEX: %i\n", tileX);*/

		/*
			Now let's find out the tile identification number
			for the tile we're over

			TODO strip these under 80 chars
		*/
/*		if(usingSigned == 0)
			tile_ident = memory_readb(bg_map + tileX + (tileY * 32));
		else
			tile_ident = (signed)memory_readb(bg_map + tileX + (tileY * 32));

		printf("DBG VIDEO TILE MAKEUP: BG_MAP: %X; tileX: %i; tileY: %i; identloc: %X\n",
			bg_map, tileX, tileY, (bg_map+tileX+(tileY*32)));

		if(tile_ident > 0)
			printf("DBG VIDEO TILEI: %i\n", tile_ident);*/

		/*
			Now we know the number of the tile we want, let's
			get dat tile data.

			First we'll find the exact location in memory the
			tile data is located at. bg_data is our starting
			tile data location, we'll add an offset of tile
			identification number * 16 (each tile is 16 bytes)
			Example: bg_data starts at 0x8000, tile_ident is
			35, 0x8000 + (35 * size of tile(16)); tile_location
			= 0x8230 TODO add info on signed  location
		*/
/*		if(usingSigned == 0)
			tile_location = bg_data + (tile_ident * 16);
		else
			tile_location = bg_data + ((tile_ident + 128) * 16);

		printf("DBG VIDEO BGDATA: BG_MAP: %X; U_OFFSET: %X; TOTAL: %X\n",
			bg_data, (tile_ident * 16), tile_location);*/

		/*
			Now let's figure out which line of the tile we're
			going to be drawing
		*/
/*		tile_line = (tileY % 8) * 2;*/

		/*
			Read in the actual tile data finally
		*/
/*		tile_byte1 = memory_readb(tile_location + tile_line);
		tile_byte2 = memory_readb(tile_location + tile_line + 1);*/

		/*
			Now we have the whole tile block we need, but
			because of scrolling we'll often be drawing
			part of a tile. Let's figure out how far into
			the tile (horizontally) we're going to actually
			start from.
		*/
/*		tile_offset = (scrollX % 8) * 2;*/

		/*
			Are we done yet? NOPE! Now we need to read in
			the color information from the tile data bytes.

			The color data is two bits, say we're reading
			the fourth pixel in the tile. The first color bit
			is in bit 3 of tile_data2, the second color bit
			will be in bit 3 of tile_data1.

			So, bit 3 of tile_data2: 1
			bit 3 of tile_data1: 0

			Color value: 10 (2)
		*/
/*		tile_color = bitset(&tile_byte2, tile_offset) << 1;
		tile_color += bitset(&tile_byte1, tile_offset);*/

		/*
			My brain hurts, seriously. Now we must compare
			this color value to the palette register (0xFF47).

			tile_color will correspond to a bit pair in the
			palette register. This bit pair in the palette
			register will correspond to the actual color.
			Doing colors this way will allow games to switch
			colors around without having to modify tile data.
			Think mario when he becomes invincible, blinking
			with different colors, this is done by simple
			palette manipulation.

			Here we're calling GL_get_color to get the true
			color identification number which will be put
			into the video buffer.
		*/
/*		true_color = GL_get_bit_color(tile_color);*/

		/*
			Here we're going to put the true color identifier
			number into the video buffer at the appropriate
			location.

			We will be done here and soon the graphics library
			will do the final drawing to the screen according
			to the data in the video buffer.

			We're done now. Congrats.
		*/
/*		if(true_color > 0)
			printf("DBG VIDEO TRUECOLOR > 0: %i\n", true_color);

		video_buffer[scanline][pixelLoc] = true_color;
	}
}*/

void GL_draw_tiles()
{
        int i;
        word map, tiles, tile_loc;
        byte tileY, tileX, tile_ident, tile_line, tile_data1, tile_data2;
        byte tile_offset, tile_color;
	byte scrollX, scrollY, scanline, LCDC, true_color;

        scrollX = memory_readb(0xFF43);
        /* Get current ScrollY (register 0xFF42) */
        scrollY = memory_readb(0xFF42);
        /* Get current scanline (register LY: 0xFF44) */
        scanline = memory_readb(0xFF44);
        /* Get current LCDC (register 0xFF40) */
        LCDC = memory_readb(0xFF40);

        if(bitset(&LCDC, 3))
                map = 0x9C00;
        else
                map = 0x9800;

        if(bitset(&LCDC, 4))
                tiles = 0x8000;
        else
                tiles = 0x8800;

        tileY = ((scrollY + scanline) / 8);

        for(i = 0; i < 160; i++)
        {
                tileX = ((i + scrollX) / 8);

                tile_ident = memory_readb(map + tileX + (tileY * 32));

                tile_loc = tiles + (tile_ident * 16);

		tile_line = (scanline % 8);

                tile_data1 = memory_readb(tile_loc + tile_line);
                tile_data2 = memory_readb(tile_loc + tile_line + 1);

                /* no horizontal offset yet */

                tile_offset = (i % 8) * 2;
                tile_color = bitset(&tile_data2, tile_offset) << 1;
                tile_color += bitset(&tile_data1, tile_offset);

                true_color = GL_get_bit_color(tile_color);

                video_buffer[scanline][i] = true_color;
        }
}

