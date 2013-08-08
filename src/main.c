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

/* Main.c */

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "cpu.h"
#include "lcd.h"
#include "gl_sdl.h"

int main(int argc, char *argv[])
{
	char deleteme;
	int debugmode = 0;
	int instruction_count = 0;

	if(argc < 2)
	{
		printf("Not enough arguments\n");
		return 0;
	}

	if(argc == 3)
	{
		debugmode = atoi(argv[2]);
		printf("Debug set: %i\n", debugmode);
	}

	if(load_rom(&argv[1]) < 0)
	{
		printf("Failed to load the ROM\n");
		return 0;
	}

	memory_init();
	LCD_init();
	GL_SDL_init();

	CPU_reset();

	/*loadBIOS();*/
	/*memory[0x9904] = 1;
	memory[0x9905] = 2;
	memory[0x9910] = 19;*/

	while(!(CPU(memory_readb(PC)) < 0))
	{
		total_cycles += cycles;

		printf("PC: %X ", PC);
		printf("OP: %X ", memory_readb(PC));
		printf("ICount: %i ", instruction_count);

		/*	Debug printing	*/
		if(debugmode == 0)
			printf("\n");
		if(debugmode >= 1)
			printf("NextB: %X\n", memory_readb(PC+1));
		if(debugmode >= 2)
			printf("Z: %X; N: %X; H: %X; C: %X\n", F.Z, F.N, F.H, F.C);
		if(debugmode >= 3)
			printf("A: %X; BC: %X%X; DE: %X%X; HL: %X%X; SP: %X\n", A, B, C, D, E, H, L, SP);
		if(debugmode >= 4)
			scanf("%c", &deleteme);

		LCD_update(cycles);

		CPU_check_interrupts();

		if(total_cycles >= max_cycles)
		{
			total_cycles = 0;

			/* Do graphics stuff here */
		}

		/* Increase total instruction count for debugging */
		instruction_count++;
	}

	/*printMEMORY();*/

	GL_SDL_exit();

	return 0;
}
