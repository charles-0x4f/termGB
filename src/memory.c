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

/* Memory.c */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "cpu.h"
#include "gl.h"

/* Load ROM file into allocated memory */
int load_rom(char **filename)
{
	int sigh;
	size_t error;
	unsigned long size;
	FILE *file;

	file = fopen(*filename, "rb");
	if(file == NULL)
		return -1;
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        rewind(file);

	ROM = malloc(size * sizeof(char*));
	if(ROM == NULL)
		return -1;

	error = fread(ROM, 1, size, file);
	if(error != size)
		return -1;

	fclose(file);

	return 0;
}

/* Initialize the memory map, load ROM initial ROM banks */
void memory_init()
{
	/* 0 out the memory for safety */
	memset(memory, 0, 0xFFFF);

	/* Memory map always stores bank 0 of ROM in the first 16KB */
	memmove(memory, ROM, 0x3FFF);
	/* ROM bank 1 will be initialized here, carts with MBCs will be able
		to switch this bank with other ROM banks */
	memmove(memory+0x3FFF, ROM+0x3FFF, 0x3FFF);
}

/* Get and return a byte from memory */
byte memory_readb(word address)
{
	return memory[address];
}

/* Read two bytes from memory and return a combined word */
word memory_readw(word address)
{
	word result;
	byte n1, n2;

	n1 = memory[address];
	n2 = memory[address+1];

	convert_to_pair(&n1, &n2, &result);

	return result;
}

/* Write byte into memory address */
void memory_writeb(word address, byte data)
{
	memory[address] = data;

	/* If address is the DMA register, start DMA transfer */
	if(address == 0xFF46)
	{
		GL_dma(data);
	}
}

void printROM()
{
	int x;
	int size;

	/* note: this doesn't work with dynamic arrays */
	size = (sizeof(ROM) / sizeof(ROM[0]));

	for(x = 0; x < size; x++)
		printf("ROM %X: %X\n", x, ROM[x]);
}

void printMEMORY()
{
	int x;
	int size;

	size = (sizeof(memory) / sizeof(uint8_t));

	for(x = 0; x < size; x++)
		printf("memory %X: %X\n", x, memory[x]);
}

void printBANK0()
{
	int x;

	for(x = 0; x <= 0x3FFF; x++)
		printf("BANK0 %X: %X\n", x, memory[x]);
}

void printBANK1()
{
        int x;

        for(x = 0x4000; x <= 0x7FFF; x++)
                printf("BANK1 %X: %X\n", x, memory[x]);
}

void printZEROPAGE()
{
	int x;

	for(x = 0xFF80; x <= 0xFFFE; x++)
		printf("ZEROP %X: %X\n", x, memory[x]);
}
