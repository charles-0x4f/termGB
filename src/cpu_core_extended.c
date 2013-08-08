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

/* Jump table for the extended CPU instructions */
#include <stdio.h>
#include "cpu.h"
#include "memory.h"

/* Emulation for the extended CPU instructions */
int CPU_EXTENDED(word OP)
{
	switch(OP)
	{

	/* ++++++ MISC +++++ */

	case 0x37:
	{
		/*
		SWAP A

		Description:
		Swap the lower and upper nibbles of A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_swap(&A);

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x30:
	{
		/*
		SWAP B

		Description:
		Swap the lower and upper nibbles of B

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_swap(&B);

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x31:
	{
		/*
		SWAP C

		Description:
		Swap the lower and upper nibbles of C

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_swap(&C);

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x32:
	{
		/*
		SWAP D

		Description:
		Swap the lower and upper nibbles of D

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_swap(&D);

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x33:
	{
		/*
		SWAP E

		Description:
		Swap the lower and upper nibbles of E

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_swap(&E);

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x34:
	{
		/*
		SWAP H

		Description:
		Swap the lower and upper nibbles of H

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_swap(&H);

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x35:
	{
		/*
		SWAP L

		Description:
		Swap the lower and upper nibbles of L

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_swap(&L);

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x36:
	{
		/*
		SWAP (HL)

		Description:
		Swap the lower and upper nibbles of the location
		pointed to by HL

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		word address;
		byte temp;

		/* Convert HL to an address */
		convert_to_pair(&H, &L, &address);

		/* Get byte pointed to by HL */
		temp = memory_readb(address);

		CPU_swap(&temp);

		/* Write swaped byte back to memory */
		memory_writeb(address, temp);

		PC += 2;
		cycles = 8;
		break;
	}

	/* ++++++++ BIT MANIPULATION ++++++++ */

	case 0x87:
	{
		/*
		RES 0, A

		Description:
		Reset bit 0 in register A
		*/
		CPU_res(0, &A);

		PC++;
		cycles = 8;
		break;
	}
	case 0x8F:
	{
		/*
		RES 1, A

		Description:
		Reset bit 1 in register A
		*/
		CPU_res(1, &A);

		PC++;
		cycles = 8;
		break;
	}
	case 0x97:
	{
		/*
		RES 2, A

		Description:
		Reset bit 2 in register A
		*/
		CPU_res(2, &A);

		PC++;
		cycles = 8;
		break;
	}
	case 0x9F:
	{
		/*
		RES 3, A

		Description:
		Reset bit 3 in register A
		*/
		CPU_res(3, &A);

		PC++;
		cycles = 8;
		break;
	}
	case 0xA7:
	{
		/*
		RES 4, A

		Description:
		Reset bit 4 in register A
		*/
		CPU_res(4, &A);

		PC++;
		cycles = 8;
		break;
	}
	case 0xAF:
	{
		/*
		RES 5, A

		Description:
		Reset bit 5 in register A
		*/
		CPU_res(5, &A);

		PC++;
		cycles = 8;
		break;
	}
	case 0xB7:
	{
		/*
		RES 6, A

		Description:
		Reset bit 6 in register A
		*/
		CPU_res(6, &A);

		PC++;
		cycles = 8;
		break;
	}
	case 0xBF:
	{
		/*
		RES 7, A

		Description:
		Reset bit 7 in register A
		*/
		CPU_res(7, &A);

		PC++;
		cycles = 8;
		break;
	}

	default:
	{
		printf("OP not handled: CB %X\n", OP);
		return -1;
		break;
	}

	} /* End switch */

	return 0;
}
