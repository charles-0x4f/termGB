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
#include <signal.h>
#include "gl_sdl.h"
#include "memory.h"
#include "gl.h"

void GL_SDL_init()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	signal(SIGINT, SIG_DFL);

	LCD = SDL_SetVideoMode(160, 144, 32, SDL_SWSURFACE);
	SDL_Flip(LCD);

	color[0] = SDL_MapRGB(LCD->format, 227, 227, 227);
	color[1] = SDL_MapRGB(LCD->format, 163, 163, 163);
	color[2] = SDL_MapRGB(LCD->format, 105, 105, 105);
	color[3] = SDL_MapRGB(LCD->format, 56, 56, 56);
}

void GL_SDL_exit()
{
	SDL_Quit();
}

void GL_SDL_draw_scanline()
{
	byte x, scanline;
	Uint32 *pixels;
	
	/*
		Read the scanline in, i.e. the y location
	*/
	scanline = memory_readb(0xFF44);

	printf("DBG VIDEO SCANLINE: %i\n", scanline);

	for(x = 0; x < 160; x++)
	{
		printf("DBG VIDEOB %i: %i\n", x, video_buffer[scanline][x]);

		pixels = (Uint32*)LCD->pixels + (scanline * LCD->w) + x;
		*pixels = color[video_buffer[scanline][x]];
	}

	SDL_UpdateRect(LCD, 0, scanline, 160, 1);
}

