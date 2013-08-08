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

/* memory.h */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef uint8_t byte;
typedef int8_t s_byte;
typedef uint16_t word;


/* Array of bytes to hold the entire ROM */
byte *ROM;
/* Raw emulation of the GB memory map, may change in the future. */
byte memory[0xFFFF+1];




/* Load ROM from file name into an array for easy access. */
int load_rom(char **rom);
/* Initialize memory */
void memory_init();
/* Read byte from memory */
byte memory_readb(word);
/* Read word from memory */
word memory_readw(word);
/* Write byte to memory */
void memory_writeb(word, byte);
/* Write word to memory */
void memory_writew(word, word);



/* DEBUG FUNCTIONS - Remove these when real debugging is in */
void printROM();
void printMEMORY();
void printBANK0();
void printBANK1();
void printZEROPAGE();
void loadBIOS();
#endif
