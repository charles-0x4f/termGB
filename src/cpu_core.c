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

/*
	Contains the main CPU core/jumptable to unclutter CPU function
	definitions.

	i.e. int CPU(word)
*/

#include <stdio.h>
#include "cpu.h"


/* The main GBZ80 emulation core, all descriptions and op information
	taken from the Gameboy CPU Manual:
	marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
*/
int CPU(word op)
{
	int result = 0;

	switch(op)
	{
	case 0xCB:
	{
		/*
		CB -

		Description:
		The Gameboy has an extended instruction set prefixed
		by CB. Here we will increment PC and call CPU_EXTENDED
		to handle the additional jump table
		*/
		PC++;

		result = CPU_EXTENDED(memory_readb(PC));
		break;
	}
	case 0x00:
	{
		/*
		NOP

		Description:
		No operation.
		*/
		PC++;
		cycles = 4;
		break;
	}

	

		/* ++++++ LOADS ++++++ */

		/* 8-BIT IMMEDIATE LOADS */
	case 0x06:
	{
		/*
		LD B, n

		Description:
		Put value n into register B

		Use with:
		n = immediate 8-bit value
		*/
		CPU_load_immediate(&B);
		cycles = 8;
		break;
	}
	case 0x0E:
	{
		/*
		LD C, n

		Description:
		Put value n into register C

		Use with:
		n = immediate 8-bit value
		*/
		CPU_load_immediate(&C);
		cycles = 8;
		break;
	}
	case 0x16:
	{
		/*
		LD D, n

		Description:
		Put value n into register D

		Use with:
		n = immediate 8-bit value
		*/
		CPU_load_immediate(&D);
		cycles = 8;
		break;
	}
	case 0x1E:
	{
		/*
		LD E, n

		Description:
		Put value n into register E

		Use with:
		n = immediate 8-bit value
		*/
		CPU_load_immediate(&E);
		cycles = 8;
		break;
	}
	case 0x26:
	{
		/*
		LD H, n

		Description:
		Put value n into register H

		Use with:
		n = immediate 8-bit value
		*/
		CPU_load_immediate(&H);
		cycles = 8;
		break;
	}
	case 0x2E:
	{
		/*
		LD L, n

		Description:
		Put value n into register L

		Use with:
		n = immediate 8-bit value
		*/
		CPU_load_immediate(&L);
		cycles = 8;
		break;
	}

		/* 8-BIT LOAD REGISTER/ADDRESS */
	case 0x7F:
	{
		/*
		LD A, A

		Description:
		Put value of A into register A
		*/
		CPU_load_register(&A, &A);
		cycles = 4;
		break;
	}
	case 0x78:
	{
		/*
		LD A, B

		Description:
		Put value of B into register A
		*/
		CPU_load_register(&A, &B);
		cycles = 4;
		break;
	}
	case 0x79:
	{
		/*
		LD A, C

		Description:
		Put value of C into register A
		*/
		CPU_load_register(&A, &C);
		cycles = 4;
		break;
	}
	case 0x7A:
	{
		/*
		LD A, D

		Description:
		Put value of D into register A
		*/
		CPU_load_register(&A, &D);
		cycles = 4;
		break;
	}
	case 0x7B:
	{
		/*
		LD A, E

		Description:
		Put value of E into register A
		*/
		CPU_load_register(&A, &E);
		cycles = 4;
		break;
	}
	case 0x7C:
	{
		/*
		LD A, H

		Description:
		Put value of H into register A
		*/
		CPU_load_register(&A, &H);
		cycles = 4;
		break;
	}
	case 0x7D:
	{
		/*
		LD A, L

		Description:
		Put value of L into register A
		*/
		CPU_load_register(&A, &L);
		cycles = 4;
		break;
	}
	case 0x0A:
	{
		/*
		LD A, (BC)

		Description:
		Put value pointed to by BC into register A
		*/
		word address;

		convert_to_pair(&B, &C, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&A, address, 0);
		cycles = 8;
		break;
	}
	case 0x1A:
	{
		/*
		LD A, (DE)

		Description:
		Put value pointed to by DE into register A
		*/
		word address;

		convert_to_pair(&D, &E, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&A, address, 0);
		cycles = 8;
		break;
	}
	case 0x7E:
	{
		/*
		LD A, (HL)

		Description:
		Put value pointed to by HL into register A
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&A, address, 0);
		cycles = 8;
		break;
	}
	case 0xFA:
	{
		/*
		LD A, (nn)

		Description:
		Put value pointed to by two byte immediate 
		address(LSB first) into register A
		*/
		word address = memory_readw(PC+1);

		address = swap_byte_order(&address);

		CPU_load_address(&A, address, 0);
		
		PC += 2;
		cycles = 16;
		break;
	}
	case 0x3E:
	{
		/*
		LD A, #

		Description:
		Put immediate byte into register A
		*/
		byte immediate = memory_readb(PC+1);

		CPU_load_register(&A, &immediate);

		PC++;
		cycles = 8;
		break;
	}
	case 0x47:
	{
		/*
		LD B, A

		Description:
		Put value of A into register B
		*/
		CPU_load_register(&B, &A);
		cycles = 4;
		break;
	}
	case 0x40:
	{
		/*
		LD B, B

		Description:
		Put value of B into register B
		*/
		CPU_load_register(&B, &B);
		cycles = 4;
		break;
	}
	case 0x41:
	{
		/*
		LD B, C

		Description:
		Put value of C into register B
		*/
		CPU_load_register(&B, &C);
		cycles = 4;
		break;
	}
	case 0x42:
	{
		/*
		LD B, D

		Description:
		Put value of D into register B
		*/
		CPU_load_register(&B, &D);
		cycles = 4;
		break;
	}
	case 0x43:
	{
		/*
		LD B, E

		Description:
		Put value of E into register B
		*/
		CPU_load_register(&B, &E);
		cycles = 4;
		break;
	}
	case 0x44:
	{
		/*
		LD B, H

		Description:
		Put value of H into register B
		*/
		CPU_load_register(&B, &H);
		cycles = 4;
		break;
	}
	case 0x45:
	{
		/*
		LD B, L

		Description:
		Put value of L into register B
		*/
		CPU_load_register(&B, &L);
		cycles = 4;
		break;
	}
	case 0x46:
	{
		/*
		LD B, (HL)

		Description:
		Put value pointed to by HL into register B
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&B, address, 0);
		cycles = 8;
		break;
	}
	case 0x4F:
	{
		/*
		LD C, A

		Description:
		Put value of A into register C
		*/
		CPU_load_register(&C, &A);
		cycles = 4;
		break;
	}
	case 0x48:
	{
		/*
		LD C, B

		Description:
		Put value of B into register C
		*/
		CPU_load_register(&C, &B);
		cycles = 4;
		break;
	}
	case 0x49:
	{
		/*
		LD C, C

		Description:
		Put value of C into register C
		*/
		CPU_load_register(&C, &C);
		cycles = 4;
		break;
	}
	case 0x4A:
	{
		/*
		LD C, D

		Description:
		Put value of D into register C
		*/
		CPU_load_register(&C, &D);
		cycles = 4;
		break;
	}
	case 0x4B:
	{
		/*
		LD C, E

		Description:
		Put value of E into register C
		*/
		CPU_load_register(&C, &E);
		cycles = 4;
		break;
	}
	case 0x4C:
	{
		/*
		LD C, H

		Description:
		Put value of H into register C
		*/
		CPU_load_register(&C, &H);
		cycles = 4;
		break;
	}
	case 0x4D:
	{
		/*
		LD C, L

		Description:
		Put value of L into register C
		*/
		CPU_load_register(&C, &L);
		cycles = 4;
		break;
	}
	case 0x4E:
	{
		/*
		LD C, (HL)

		Description:
		Put value pointed to by HL into register C
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&C, address, 0);
		cycles = 8;
		break;
	}
	case 0x57:
	{
		/*
		LD D, A

		Description:
		Put value of A into register D
		*/
		CPU_load_register(&D, &A);
		cycles = 4;
		break;
	}
	case 0x50:
	{
		/*
		LD D, B

		Description:
		Put value of B into register D
		*/
		CPU_load_register(&D, &B);
		cycles = 4;
		break;
	}
	case 0x51:
	{
		/*
		LD D, C

		Description:
		Put value of C into register D
		*/
		CPU_load_register(&D, &C);
		cycles = 4;
		break;
	}
	case 0x52:
	{
		/*
		LD D, D

		Description:
		Put value of D into register D
		*/
		CPU_load_register(&D, &D);
		cycles = 4;
		break;
	}
	case 0x53:
	{
		/*
		LD D, E

		Description:
		Put value of E into register D
		*/
		CPU_load_register(&D, &E);
		cycles = 4;
		break;
	}
	case 0x54:
	{
		/*
		LD D, H

		Description:
		Put value of H into register D
		*/
		CPU_load_register(&D, &H);
		cycles = 4;
		break;
	}
	case 0x55:
	{
		/*
		LD D, L

		Description:
		Put value of L into register D
		*/
		CPU_load_register(&D, &L);
		cycles = 4;
		break;
	}
	case 0x56:
	{
		/*
		LD D, (HL)

		Description:
		Put value pointed to by HL into register D
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&D, address, 0);
		cycles = 8;
		break;
	}
	case 0x5F:
	{
		/*
		LD E, A

		Description:
		Put value of A into register E
		*/
		CPU_load_register(&E, &A);
		cycles = 4;
		break;
	}
	case 0x58:
	{
		/*
		LD E, B

		Description:
		Put value of B into register E
		*/
		CPU_load_register(&E, &B);
		cycles = 4;
		break;
	}
	case 0x59:
	{
		/*
		LD E, C

		Description:
		Put value of C into register E
		*/
		CPU_load_register(&E, &C);
		cycles = 4;
		break;
	}
	case 0x5A:
	{
		/*
		LD E, D

		Description:
		Put value of D into register E
		*/
		CPU_load_register(&E, &D);
		cycles = 4;
		break;
	}
	case 0x5B:
	{
		/*
		LD E, E

		Description:
		Put value of E into register E
		*/
		CPU_load_register(&E, &E);
		cycles = 4;
		break;
	}
	case 0x5C:
	{
		/*
		LD E, H

		Description:
		Put value of H into register E
		*/
		CPU_load_register(&E, &H);
		cycles = 4;
		break;
	}
	case 0x5D:
	{
		/*
		LD E, L

		Description:
		Put value of L into register E
		*/
		CPU_load_register(&E, &L);
		cycles = 4;
		break;
	}
	case 0x5E:
	{
		/*
		LD E, (HL)

		Description:
		Put value pointed to by HL into register E
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&E, address, 0);
		cycles = 8;
		break;
	}
	case 0x67:
	{
		/*
		LD H, A

		Description:
		Put value of A into register H
		*/
		CPU_load_register(&H, &A);
		cycles = 4;
		break;
	}
	case 0x60:
	{
		/*
		LD H, B

		Description:
		Put value of B into register H
		*/
		CPU_load_register(&H, &B);
		cycles = 4;
		break;
	}
	case 0x61:
	{
		/*
		LD H, C

		Description:
		Put value of C into register H
		*/
		CPU_load_register(&H, &C);
		cycles = 4;
		break;
	}
	case 0x62:
	{
		/*
		LD H, D

		Description:
		Put value of D into register H
		*/
		CPU_load_register(&H, &D);
		cycles = 4;
		break;
	}
	case 0x63:
	{
		/*
		LD H, E

		Description:
		Put value of E into register H
		*/
		CPU_load_register(&H, &E);
		cycles = 4;
		break;
	}
	case 0x64:
	{
		/*
		LD H, H

		Description:
		Put value of H into register H
		*/
		CPU_load_register(&H, &H);
		cycles = 4;
		break;
	}
	case 0x65:
	{
		/*
		LD H, L

		Description:
		Put value of L into register H
		*/
		CPU_load_register(&H, &L);
		cycles = 4;
		break;
	}
	case 0x66:
	{
		/*
		LD H, (HL)

		Description:
		Put value pointed to by HL into register H
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&H, address, 0);
		cycles = 8;
		break;
	}
	case 0x6F:
	{
		/*
		LD L, A

		Description:
		Put value of A into register L
		*/
		CPU_load_register(&L, &A);
		cycles = 4;
		break;
	}
	case 0x68:
	{
		/*
		LD L, B

		Description:
		Put value of B into register L
		*/
		CPU_load_register(&L, &B);
		cycles = 4;
		break;
	}
	case 0x69:
	{
		/*
		LD L, C

		Description:
		Put value of C into register L
		*/
		CPU_load_register(&L, &C);
		cycles = 4;
		break;
	}
	case 0x6A:
	{
		/*
		LD L, D

		Description:
		Put value of D into register L
		*/
		CPU_load_register(&L, &D);
		cycles = 4;
		break;
	}
	case 0x6B:
	{
		/*
		LD L, E

		Description:
		Put value of E into register L
		*/
		CPU_load_register(&L, &E);
		cycles = 4;
		break;
	}
	case 0x6C:
	{
		/*
		LD L, H

		Description:
		Put value of H into register L
		*/
		CPU_load_register(&L, &H);
		cycles = 4;
		break;
	}
	case 0x6D:
	{
		/*
		LD L, L

		Description:
		Put value of L into register L
		*/
		CPU_load_register(&L, &L);
		cycles = 4;
		break;
	}
	case 0x6E:
	{
		/*
		LD L, (HL)

		Description:
		Put value pointed to by HL into register L
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&L, address, 0);
		cycles = 8;
		break;
	}
	case 0x70:
	{
		/*
		LD (HL), B

		Description:
		Put value of B into the location pointed to by HL
		*/
		word address;

		/*TODO remove all swap byte orders in loads? */
		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&B, address, 1);
		cycles = 8;
		break;
	}
	case 0x71:
	{
		/*
		LD (HL), C

		Description:
		Put value of C into the location pointed to by HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&C, address, 1);
		cycles = 8;
		break;
	}
	case 0x72:
	{
		/*
		LD (HL), D

		Description:
		Put value of D into the location pointed to by HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&D, address, 1);
		cycles = 8;
		break;
	}
	case 0x73:
	{
		/*
		LD (HL), E

		Description:
		Put value of E into the location pointed to by HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&E, address, 1);
		cycles = 8;
		break;
	}
	case 0x74:
	{
		/*
		LD (HL), H

		Description:
		Put value of H into the location pointed to by HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&H, address, 1);
		cycles = 8;
		break;
	}
	case 0x75:
	{
		/*
		LD (HL), L

		Description:
		Put value of L into the location pointed to by HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&L, address, 1);
		cycles = 8;
		break;
	}
	case 0x36:
	{
		/*
		LD (HL), n

		Description:
		Put value of n into the location pointed to by HL

		Use with:
		n - one byte immediate value
		*/
		byte immediate;
		word address;

		immediate = memory_readb(PC + 1);

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&B, address, 1);
		cycles = 12;

		/* Increment once more, the jump function does the other */
		PC++;

		break;
	}
	case 0x02:
	{
		/*
		LD (BC), A

		Description:
		Load the value of A into the location pointed to by BC
		*/
		word address;

		convert_to_pair(&B, &C, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&A, address, 1);
		cycles = 8;
		break;
	}
	case 0x12:
	{
		/*
		LD (DE), A

		Description:
		Load the value of A into the location pointed to by DE
		*/
		word address;

		convert_to_pair(&D, &E, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&A, address, 1);
		cycles = 8;
		break;
	}
	case 0x77:
	{
		/*
		LD (HL), A

		Description:
		Load the value of A into the location pointed to by HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_load_address(&A, address, 1);
		cycles = 8;
		break;
	}
	case 0xEA:
	{
		/*
		LD (nn), A

		Description:
		Load the value of A into the location pointed to by nn

		Use with:
		nn = two byte immediate value. (LS byte first.)
		*/
		word address;

		address = convert_to16m(PC+1, PC+2);

		CPU_load_address(&A, address, 1);

		PC += 2;
		cycles = 16;
		break;
	}
	case 0xF2:
	{
		/*
		LD A, (C) /
		LD A, (0xFF00 + C)

		Description:
		Load the value at address FF00 + C into register A
		*/
		CPU_load_address(&A, (0xFF00+C), 0);
		cycles = 8;
		break;
	}
	case 0xE2:
	{
		/*
		LD (C), A /
		LD (0xFF00 + C), A

		Description:
		Load A into the value at address FF00 + C
		*/
		CPU_load_address(&A, (0xFF00+C), 1);
		cycles = 8;
		break;
	}
	case 0x3A:
	{
		/*
		LDD A, (HL)

		Description:
		Put value at address HL into A; decrement HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/
		CPU_load_address(&A, address, 0);
		
		address--;

		/* Update decremented HL's registers */
		convert_to_bytes(&H, &L, &address);

		cycles = 8;
		break;
	}
	case 0x32:
	{
		/*
		LDD (HL), A

		Description:
		Put value of A into data at address HL; decrement HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/
		CPU_load_address(&A, address, 1);
		
		address--;

		/* Update decremented HL's registers */
		convert_to_bytes(&H, &L, &address);

		cycles = 8;
		break;
	}
	case 0x2A:
	{
		/*
		LDI A, (HL)

		Description:
		Put value at address HL into A; increment HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);

		printf("DBG 2A; Address from HL: %X\n", address);

		/* TODO why is swap_byte_order using pointer and return?*/
		/*address = swap_byte_order(&address);*/
		printf("DBG 2A; Address swap: %X\n", address);
		/* TODO switch cpu_load_address to use pointers */
		CPU_load_address(&A, address, 0);
		
		/* Revert address back to low-endian for non-addressing work */
		/*address = swap_byte_order(&address);*/
		address++;

		/* Update H and L with the incremented pair */
		convert_to_bytes(&H, &L, &address);

		cycles = 8;
		break;
	}
	case 0x22:
	{
		/*
		LDI (HL), A

		Description:
		Put value of A into data at address HL; increment HL
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		address = swap_byte_order(&address);
		CPU_load_address(&A, address, 1);

		/* Revert address back to low-endian for non-addressing work */
		address = swap_byte_order(&address);
		address++;

		/* Update H and L with the incremented pair */
		convert_to_bytes(&H, &L, &address);

		cycles = 8;
		break;
	}
	case 0xE0:
	{
		/*
		LDH (0xFF00+n), A

		Description:
		Load A into the address of 0xFF00 + n

		Use With:
		n = one byte immediate value
		*/
		word address = 0xFF00;

		address += memory_readb(PC + 1);
		CPU_load_address(&A, address, 1);
		PC++;

		cycles = 12;
		break;
	}
	case 0xF0:
	{
		/*
		LDH A, (0xFF00+n)

		Description:
		Load data at address (0xFF00 + n) into A

		Use With:
		n = one byte immediate value
		*/
		word address = 0xFF00;

		address += memory_readb(PC + 1);
		CPU_load_address(&A, address, 0);
		PC++;

		cycles = 12;
		break;
	}

		/* ++++ 16-BIT LOADS ++++ */

	case 0x01:
	{
		/*
		LD n, nn

		Description:
		Put immediate 16-bit value into n

		Use With:
		n = BC
		*/
		CPU_load_immediate16(&B, &C);

		cycles = 12;
		break;
	}
	case 0x11:
	{
		/*
		LD n, nn

		Description:
		Put immediate 16-bit value into n

		Use With:
		n = DE
		*/
		CPU_load_immediate16(&D, &E);

		cycles = 12;
		break;
	}
	case 0x21:
	{
		/*
		LD n, nn

		Description:
		Put immediate 16-bit value into n

		Use With:
		n = HL
		*/
		CPU_load_immediate16(&H, &L);

		cycles = 12;
		break;
	}
	case 0x31:
	{
		/*
		LD n, nn

		Description:
		Put immediate 16-bit value into n

		Use With:
		n = SP
		*/
		/* old: TODO delete? */
		/*byte a, b;

		a = memory_readb(PC+1);
		b = memory_readb(PC+2);

		CPU_load_sp_16(&a, &b);*/
		byte low, high;
		word address;

		/* Read immediate bytes and convert to a word */
		low = memory_readb(PC+1);
		high = memory_readb(PC+2);
		convert_to_pair(&high, &low, &address);

		/* Assign SP to the new address */
		SP = address;

		PC += 3;

		cycles = 12;
		break;
	}
	case 0xF9:
	{
		/*
		LD SP, HL

		Description:
		Load HL onto the stack
		*/
		CPU_load_sp_16(&H, &L);

		PC++;

		cycles = 8;
		break;
	}
	case 0xF8:
	{
		/*
		LDHL SP, n

		Description:
		Put SP + n effective address into HL

		Use with:
		n = one byte signed immediate value

		Flags:
		Z - Reset
		N - Reset
		H - Set if necessary
		C - Set if necessary
		*/
		byte n;
		/* Make a backup copy of SP */
		word original = SP;

		/* Get immediate value */
		n = memory_readb(PC+1);

		/* Add n to SP */
		CPU_add_sp_n(n);

		/* Update HL with result in SP */
		convert_to_bytes(&H, &L, &SP);

		/*
			Reinstate SP since we just want the
			result in HL and not in SP
		*/
		SP = original;

		PC += 2;

		cycles = 12;
		break;
	}
	case 0x08:
	{
		/*
		LD (nn), SP

		Description:
		Put Stack Pointer(tm) at address n

		Use with:
		nn = two byte immediate address
		*/
		SP = memory_readw(PC+1);

		PC += 3;

		cycles = 20;
		break;
	}
	case 0xF5:
	{
		/*
		PUSH nn

		Description:
		Push register pair AF onto stack
		*/

		/* Compile F struct into a byte for use */
		compiler_F(0);

		/* Push A and F register onto the stack */
		CPU_load_sp_16(&A, &F.F);

		cycles = 16;
		PC++;
		break;
	}
	case 0xC5:
	{
		/*
		PUSH nn

		Description:
		Push register pair BC onto stack
		*/

		/* Push B and C register onto the stack */
		CPU_load_sp_16(&B, &C);

		cycles = 16;
		PC++;
		break;
	}
	case 0xD5:
	{
		/*
		PUSH nn

		Description:
		Push register pair DE onto stack
		*/

		/* Push D and E register onto the stack */
		CPU_load_sp_16(&D, &E);

		cycles = 16;
		PC++;
		break;
	}
	case 0xE5:
	{
		/*
		PUSH nn

		Description:
		Push register pair HL onto stack
		*/

		/* Push H and L register onto the stack */
		CPU_load_sp_16(&H, &L);

		cycles = 16;
		PC++;
		break;
	}
	case 0xF1:
	{
		/*
		POP nn

		Description:
		Pop two bytes off of stack into pair AF
		*/

		/*
			Pop AF data from the stack, since these
			were pushed onto the stack in order, they
			must be retrieved backwards
		*/
		CPU_load_16_sp(&F.F, &A);

		/* Rebuild F flag variables from the assembled byte */
		compiler_F(1);

		PC++;

		cycles = 12;
		break;
	}
	case 0xC1:
	{
		/*
		POP nn

		Description:
		Pop two bytes off of stack into pair BC
		*/

		/*
			Pop BC data from the stack, since these
			were pushed onto the stack in order, they
			must be retrieved backwards
		*/
		CPU_load_16_sp(&C, &B);

		PC++;

		cycles = 12;
		break;
	}
	case 0xD1:
	{
		/*
		POP nn

		Description:
		Pop two bytes off of stack into pair DE
		*/

		/*
			Pop DE data from the stack, since these
			were pushed onto the stack in order, they
			must be retrieved backwards
		*/
		CPU_load_16_sp(&E, &D);

		PC++;

		cycles = 12;
		break;
	}
	case 0xE1:
	{
		/*
		POP nn

		Description:
		Pop two bytes off of stack into pair HL
		*/

		/*
			Pop HL data from the stack, since these
			were pushed onto the stack in order, they
			must be retrieved backwards
		*/
		CPU_load_16_sp(&L, &H);

		PC++;

		cycles = 12;
		break;
	}


		/* ++++++ 8-BIT ALU ++++++ */

	case 0x87:
	{
		/*
		ADD A, A

		Description:
		Add A to A and leave result in A(insert meme)

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&A, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x80:
	{
		/*
		ADD A, B

		Description:
		Add B to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&B, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x81:
	{
		/*
		ADD A, C

		Description:
		Add C to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&C, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x82:
	{
		/*
		ADD A, D

		Description:
		Add D to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&D, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x83:
	{
		/*
		ADD A, E

		Description:
		Add E to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&E, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x84:
	{
		/*
		ADD A, H

		Description:
		Add H to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&H, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x85:
	{
		/*
		ADD A, L

		Description:
		Add L to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&L, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x86:
	{
		/*
		ADD A, (HL)

		Description:
		Add value pointed to by HL to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		byte toAdd;
		word address;

		convert_to_pair(&H, &L, &address);
		toAdd = memory_readb(address);

		CPU_add_8(&toAdd, 0);

		PC++;

		cycles = 8;
		break;
	}
	case 0xC6:
	{
		/*
		ADD A, #

		Description:
		Add immediate value to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		byte immediate = memory_readb(PC+1);

		CPU_add_8(&immediate, 0);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0x8F:
	{
		/*
		ADC A, A

		Description:
		Add A + Carry flag to A and leave result in A(insert meme)

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&A, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x88:
	{
		/*
		ADC A, B

		Description:
		Add B + Carry flag to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&B, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x89:
	{
		/*
		ADC A, C

		Description:
		Add C + Carry flag to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&C, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x8A:
	{
		/*
		ADC A, D

		Description:
		Add D + Carry flag to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&D, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x8B:
	{
		/*
		ADC A, E

		Description:
		Add E + Carry flag to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&E, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x8C:
	{
		/*
		ADC A, H

		Description:
		Add H + Carry flag to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&H, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x8D:
	{
		/*
		ADC A, L

		Description:
		Add L + Carry flag to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		CPU_add_8(&L, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x8E:
	{
		/*
		ADC A, (HL)

		Description:
		Add value pointed to by HL + Carry flag to A and leave
		result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		byte toAdd;
		word address;

		convert_to_pair(&H, &L, &address);
		toAdd = memory_readb(address);

		CPU_add_8(&toAdd, 1);

		PC++;

		cycles = 8;
		break;
	}
	case 0xCE:
	{
		/*
		ADC A, #

		Description:
		Add immediate value + Carry flag to A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Set if carry from bit 7
		*/
		byte immediate = memory_readb(PC+1);

		CPU_add_8(&immediate, 1);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0x97:
	{
		/*
		SUB A, A

		Description:
		Subtract A from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&A, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x90:
	{
		/*
		SUB A, B

		Description:
		Subtract B from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&B, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x91:
	{
		/*
		SUB A, C

		Description:
		Subtract C from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&C, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x92:
	{
		/*
		SUB A, D

		Description:
		Subtract D from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&D, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x93:
	{
		/*
		SUB A, E

		Description:
		Subtract E from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&E, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x94:
	{
		/*
		SUB A, H

		Description:
		Subtract H from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&H, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x95:
	{
		/*
		SUB A, L

		Description:
		Subtract L from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&L, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x96:
	{
		/*
		SUB A, (HL)

		Description:
		Subtract the value pointed to by HL from A and leave
		result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		byte toSub;
		word address;

		convert_to_pair(&H, &L, &address);
		toSub = memory_readb(address);

		CPU_subtract_8(&toSub, 0);

		PC++;

		cycles = 8;
		break;
	}
	case 0xD6:
	{
		/*
		SUB A, #

		Description:
		Subtract immediate value from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		byte immediate = memory_readb(PC+1);

		CPU_subtract_8(&immediate, 0);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0x9F:
	{
		/*
		SBC A, A

		Description:
		Subtract A + Carry flag from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&A, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x98:
	{
		/*
		SBC A, B

		Description:
		Subtract B + Carry flag from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&B, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x99:
	{
		/*
		SBC A, C

		Description:
		Subtract C + Carry flag from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&C, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x9A:
	{
		/*
		SBC A, D

		Description:
		Subtract D + Carry flag from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&D, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x9B:
	{
		/*
		SBC A, E

		Description:
		Subtract E + Carry flag from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&E, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x9C:
	{
		/*
		SBC A, H

		Description:
		Subtract H + Carry flag from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&H, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x9D:
	{
		/*
		SBC A, L

		Description:
		Subtract L + Carry flag from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		CPU_subtract_8(&L, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x9E:
	{
		/*
		SBC A, (HL)

		Description:
		Subtract the value pointed to by HL from A and leave
		result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		byte toSub;
		word address;

		convert_to_pair(&H, &L, &address);
		toSub = memory_readb(address);

		CPU_subtract_8(&toSub, 1);

		PC++;

		cycles = 8;
		break;
	}
	case 0xDE:
	{
		/*
		SBC A, #

		Description:
		Subtract immediate value from A and leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Set if no borrow
		*/
		byte immediate = memory_readb(PC+1);

		CPU_subtract_8(&immediate, 1);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0xA7:
	{
		/*
		AND A, A

		Description:
		Bitwise AND A with A, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		CPU_and_8(&A);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA0:
	{
		/*
		AND A, B

		Description:
		Bitwise AND A with B, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		CPU_and_8(&B);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA1:
	{
		/*
		AND A, C

		Description:
		Bitwise AND A with C, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		CPU_and_8(&C);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA2:
	{
		/*
		AND A, D

		Description:
		Bitwise AND A with D, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		CPU_and_8(&D);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA3:
	{
		/*
		AND A, E

		Description:
		Bitwise AND A with E, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		CPU_and_8(&E);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA4:
	{
		/*
		AND A, H

		Description:
		Bitwise AND A with H, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		CPU_and_8(&H);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA5:
	{
		/*
		AND A, L

		Description:
		Bitwise AND A with L, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		CPU_and_8(&L);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA6:
	{
		/*
		AND A, (HL)

		Description:
		Bitwise AND A with the value pointed to by HL,
		leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		byte toAnd;
		word address;

		convert_to_pair(&H, &L, &address);
		toAnd = memory_readb(address);

		CPU_and_8(&toAnd);

		PC++;

		cycles = 8;
		break;
	}
	case 0xE6:
	{
		/*
		AND A, #

		Description:
		Bitwise AND A with immediate value, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set
		C - Reset
		*/
		byte immediate = memory_readb(PC+1);

		CPU_and_8(&immediate);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0xB7:
	{
		/*
		OR A, A

		Description:
		Bitwise OR A with A, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_or_8(&A);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB0:
	{
		/*
		OR A, B

		Description:
		Bitwise OR A with B, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_or_8(&B);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB1:
	{
		/*
		OR A, C

		Description:
		Bitwise OR A with C, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_or_8(&C);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB2:
	{
		/*
		OR A, D

		Description:
		Bitwise OR A with D, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_or_8(&D);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB3:
	{
		/*
		OR A, E

		Description:
		Bitwise OR A with E, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_or_8(&E);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB4:
	{
		/*
		OR A, H

		Description:
		Bitwise OR A with H, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_or_8(&H);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB5:
	{
		/*
		OR A, L

		Description:
		Bitwise OR A with L, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_or_8(&L);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB6:
	{
		/*
		OR A, (HL)

		Description:
		Bitwise OR A with the value pointed to by HL,
		leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		byte toOR;
		word address;

		convert_to_pair(&H, &L, &address);
		toOR = memory_readb(address);

		CPU_or_8(&toOR);

		PC++;

		cycles = 8;
		break;
	}
	case 0xF6:
	{
		/*
		OR A, #

		Description:
		Bitwise OR A with immediate value, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		byte immediate = memory_readb(PC+1);

		CPU_or_8(&immediate);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0xAF:
	{
		/*
		XOR A, A

		Description:
		Bitwise XOR A with A, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_xor_8(&A);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA8:
	{
		/*
		XOR A, B

		Description:
		Bitwise XOR A with B, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_xor_8(&B);

		PC++;

		cycles = 4;
		break;
	}
	case 0xA9:
	{
		/*
		XOR A, C

		Description:
		Bitwise XOR A with C, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_xor_8(&C);

		PC++;

		cycles = 4;
		break;
	}
	case 0xAA:
	{
		/*
		XOR A, D

		Description:
		Bitwise XOR A with D, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_xor_8(&D);

		PC++;

		cycles = 4;
		break;
	}
	case 0xAB:
	{
		/*
		XOR A, E

		Description:
		Bitwise XOR A with E, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_xor_8(&E);

		PC++;

		cycles = 4;
		break;
	}
	case 0xAC:
	{
		/*
		XOR A, H

		Description:
		Bitwise XOR A with H, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_xor_8(&H);

		PC++;

		cycles = 4;
		break;
	}
	case 0xAD:
	{
		/*
		XOR A, L

		Description:
		Bitwise XOR A with L, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		CPU_xor_8(&L);

		PC++;

		cycles = 4;
		break;
	}
	case 0xAE:
	{
		/*
		XOR A, (HL)

		Description:
		Bitwise XOR A with the value pointed to by HL,
		leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		byte toXOR;
		word address;

		convert_to_pair(&H, &L, &address);
		toXOR = memory_readb(address);

		CPU_xor_8(&toXOR);

		PC++;

		cycles = 8;
		break;
	}
	case 0xEE:
	{
		/*
		XOR A, #

		Description:
		Bitwise XOR A with immediate value, leave result in A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Reset
		*/
		byte immediate = memory_readb(PC+1);

		CPU_xor_8(&immediate);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0xBF:
	{
		/*
		CP A, A

		Description:
		Compare A with A

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		CPU_compare_8(&A);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB8:
	{
		/*
		CP A, B

		Description:
		Compare A with B

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		CPU_compare_8(&B);

		PC++;

		cycles = 4;
		break;
	}
	case 0xB9:
	{
		/*
		CP A, C

		Description:
		Compare A with C

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		CPU_compare_8(&C);

		PC++;

		cycles = 4;
		break;
	}
	case 0xBA:
	{
		/*
		CP A, D

		Description:
		Compare A with D

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		CPU_compare_8(&D);

		PC++;

		cycles = 4;
		break;
	}
	case 0xBB:
	{
		/*
		CP A, E

		Description:
		Compare A with E

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		CPU_compare_8(&E);

		PC++;

		cycles = 4;
		break;
	}
	case 0xBC:
	{
		/*
		CP A, H

		Description:
		Compare A with H

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		CPU_compare_8(&H);

		PC++;

		cycles = 4;
		break;
	}
	case 0xBD:
	{
		/*
		CP A, L

		Description:
		Compare A with L

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		CPU_compare_8(&L);

		PC++;

		cycles = 4;
		break;
	}
	case 0xBE:
	{
		/*
		CP A, (HL)

		Description:
		Compare A with the value pointed to by HL

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		byte toCompare;
		word address;

		convert_to_pair(&H, &L, &address);
		toCompare = memory_readb(address);

		CPU_compare_8(&toCompare);

		PC++;

		cycles = 8;
		break;
	}
	case 0xFE:
	{
		/*
		CP A, #

		Description:
		Compare A with immediate value

		Flags affected:
		Z - Set if N is equal to A
		N - Set
		H - Set if no borrow from bit 4
		C - Set if A < N
		*/
		byte immediate = memory_readb(PC+1);

		CPU_compare_8(&immediate);

		PC += 2;

		cycles = 8;
		break;
	}
	case 0x3C:
	{
		/*
		INC A

		Description:
		Increment A

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		CPU_incdec_8(&A, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x04:
	{
		/*
		INC B

		Description:
		Increment B

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		CPU_incdec_8(&B, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x0C:
	{
		/*
		INC C

		Description:
		Increment C

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		CPU_incdec_8(&C, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x14:
	{
		/*
		INC D

		Description:
		Increment D

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		CPU_incdec_8(&D, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x1C:
	{
		/*
		INC E

		Description:
		Increment E

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		CPU_incdec_8(&E, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x24:
	{
		/*
		INC H

		Description:
		Increment H

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		CPU_incdec_8(&H, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x2C:
	{
		/*
		INC L

		Description:
		Increment L

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		CPU_incdec_8(&L, 0);

		PC++;

		cycles = 4;
		break;
	}
	case 0x34:
	{
		/*
		INC (HL)

		Description:
		Increment value pointed to by HL

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Set if carry from bit 3
		C - Not affected (reset)
		*/
		byte inc;
		word address;

		convert_to_pair(&H, &L, &address);
		inc = memory_readb(address);

		CPU_incdec_8(&inc, 0);

		/* Update actual memory with the incremented value */
		memory_writeb(address, inc);

		PC++;

		cycles = 12;
		break;
	}
	case 0x3D:
	{
		/*
		DEC A

		Description:
		Decrement A

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		CPU_incdec_8(&A, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x05:
	{
		/*
		DEC B

		Description:
		Decrement B

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		CPU_incdec_8(&B, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x0D:
	{
		/*
		DEC C

		Description:
		Decrement C

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		CPU_incdec_8(&C, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x15:
	{
		/*
		DEC D

		Description:
		Decrement D

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		CPU_incdec_8(&D, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x1D:
	{
		/*
		DEC E

		Description:
		Decrement E

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		CPU_incdec_8(&E, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x25:
	{
		/*
		DEC H

		Description:
		Decrement H

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		CPU_incdec_8(&H, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x2D:
	{
		/*
		DEC L

		Description:
		Decrement L

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		CPU_incdec_8(&L, 1);

		PC++;

		cycles = 4;
		break;
	}
	case 0x35:
	{
		/*
		DEC (HL)

		Description:
		Decrement the value pointed to by HL

		Flags affected:
		Z - Set if result is zero
		N - Set
		H - Set if no borrow from bit 4
		C - Not affected (reset)
		*/
		byte dec;
		word address;

		convert_to_pair(&H, &L, &address);
		dec = memory_readb(address);

		CPU_incdec_8(&dec, 1);

		memory_writeb(address, dec);

		PC++;

		cycles = 12;
		break;
	}


		/* +++++ 16-BIT ARITHMETIC +++++ */
	case 0x09:
	{
		/*
		ADD HL, BC

		Description:
		Add BC to HL.

		Flags affected:
		Z - Not affected
		N - Reset
		H - Set if carry from bit 11
		C - Set if carry from bit 15
		*/
		CPU_add_16(&H, &L, &B, &C);
		cycles = 8;

		PC++;
		break;
	}
	case 0x19:
	{
		/*
		ADD HL, DE

		Description:
		Add DE to HL.

		Flags affected:
		Z - Not affected
		N - Reset
		H - Set if carry from bit 11
		C - Set if carry from bit 15
		*/
		CPU_add_16(&H, &L, &D, &E);
		cycles = 8;

		PC++;
		break;
	}
	case 0x29:
	{
		/*
		ADD HL, HL

		Description:
		Add HL to HL.

		Flags affected:
		Z - Not affected
		N - Reset
		H - Set if carry from bit 11
		C - Set if carry from bit 15
		*/
		CPU_add_16(&H, &L, &H, &L);
		cycles = 8;

		PC++;
		break;
	}
	case 0x39:
	{
		/*
		ADD HL, SP

		Description:
		Add SP to HL.

		Flags affected:
		Z - Not affected
		N - Reset
		H - Set if carry from bit 11
		C - Set if carry from bit 15
		*/

		/* We need to convert SP into bytes for the addition */
		byte temp1, temp2;
		convert_to_bytes(&temp1, &temp2, &SP);

		CPU_add_16(&H, &L, &temp1, &temp2);
		cycles = 8;

		PC++;
		break;
	}
	case 0xE8:
	{
		/*
		ADD SP, n

		Description:
		Add immediate byte to Stack Pointer.

		Use with:
		Signed one byte immediate value

		Flags affected:
		Z - Reset
		N - Reset
		H - Set or reset according to operation
		C - Set or reset according to operation
		*/
		/* Get immediate value */
		byte temp = memory_readb(PC+1);

		CPU_add_sp_n(temp);
		PC += 2;
		cycles = 16;

		break;
	}
	case 0x03:
	{
		/*
		INC BC

		Description:
		Increment register BC

		Flags affected:
		None
		*/
		CPU_incdec_16(&B, &C, 0);
		PC++;

		cycles = 8;
		break;
	}
	case 0x13:
	{
		/*
		INC DE

		Description:
		Increment register DE

		Flags affected:
		None
		*/
		CPU_incdec_16(&D, &E, 0);
		PC++;

		cycles = 8;
		break;
	}
	case 0x23:
	{
		/*
		INC HL

		Description:
		Increment register HL

		Flags affected:
		None
		*/
		CPU_incdec_16(&H, &L, 0);
		PC++;

		cycles = 8;
		break;
	}
	case 0x33:
	{
		/*
		INC SP

		Description:
		Increment register SP

		Flags affected:
		None
		*/
		SP++;
		PC++;

		cycles = 8;
		break;
	}
	case 0x0B:
	{
		/*
		DEC BC

		Description:
		Decrement register BC

		Flags affected:
		None
		*/
		CPU_incdec_16(&B, &C, 1);
		PC++;

		cycles = 8;
		break;
	}
	case 0x1B:
	{
		/*
		DEC DE

		Description:
		Decrement register DE

		Flags affected:
		None
		*/
		CPU_incdec_16(&D, &E, 1);
		PC++;

		cycles = 8;
		break;
	}
	case 0x2B:
	{
		/*
		DEC HL

		Description:
		Decrement register HL

		Flags affected:
		None
		*/
		CPU_incdec_16(&H, &L, 1);
		PC++;

		cycles = 8;
		break;
	}
	case 0x3B:
	{
		/*
		DEC SP

		Description:
		Decrement register SP

		Flags affected:
		None
		*/
		SP--;
		PC++;

		cycles = 8;
		break;
	}

	
		/* +++++ ROTATES AND SHIFTS +++++ */
	case 0x07:
	{
		/*
		RLCA

		Description:
		Rotate A left. Old bit 7 to Carry flag.

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Contains old bit 7 data
		*/
		CPU_rotate(&A, 0);

		PC++;
		cycles = 4;
		break;
	}
	case 0x17:
	{
		/*
		RLA

		Description:
		Rotate A left through Carry flag.

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Contains old bit 7 data
		*/
		CPU_rotate_through(&A, 0);

		PC++;
		cycles = 4;
		break;
	}
	case 0x0F:
	{
		/*
		RRCA

		Description:
		Rotate A right. Old bit 0 to Carry flag.

		Flags affected:
		Z - Set if result is zero
		N - Reset
		H - Reset
		C - Contains old bit 7 data
		*/
		CPU_rotate(&A, 1);

		PC++;
		cycles = 4;
		break;
	}
	case 0x1F:
	{
		/*
		RRA

		Description:
		Rotate A right through carry flag

		Flags affected:
		Z - Set if result is 0
		N - Reset
		H - Reset
		C - Contains old bit 0 data
		*/
		CPU_rotate_through(&A, 1);

		PC++;
		cycles = 4;
		break;
	}
		/* ++++++ END ROTATE/SHIFTS ++++++ */

		/* +++++ CALLS/RESTARTS/RETURNS +++++ */
	case 0xCD:
	{
		/*
		CALL nn

		Description:
		Push address of next instruction onto the stack then
		jump to address nn

		Use with:
		nn = two-byte immediate address (LS byte first)
		*/
		CPU_call();

		cycles = 12;
		break;
	}
	case 0xC4:
	{
		/*
		CALL NZ, nn

		Description:
		If Z flag reset, push address of next instruction onto the
		stack then jump to address nn

		Use with:
		nn = two-byte immediate address (LS byte first)
		*/

		if(!F.Z)
			CPU_call();
		else
			PC += 3;

		cycles = 12;
		break;
	}
	case 0xCC:
	{
		/*
		CALL Z, nn

		Description:
		If Z flag is set, push address of next instruction onto the
		stack then jump to address nn

		Use with:
		nn = two-byte immediate address (LS byte first)
		*/

		if(F.Z)
			CPU_call();
		else
			PC += 3;

		cycles = 12;
		break;
	}
	case 0xD4:
	{
		/*
		CALL NC, nn

		Description:
		If C flag reset, push address of next instruction onto the
		stack then jump to address nn

		Use with:
		nn = two-byte immediate address (LS byte first)
		*/

		if(!F.C)
			CPU_call();
		else
			PC += 3;

		cycles = 12;
		break;
	}
	case 0xDC:
	{
		/*
		CALL C, nn

		Description:
		If C flag is set, push address of next instruction onto the
		stack then jump to address nn

		Use with:
		nn = two-byte immediate address (LS byte first)
		*/

		if(F.C)
			CPU_call();
		else
			PC += 3;

		cycles = 12;
		break;
	}
	case 0xC7:
	{
		/*
		RST 0x00

		Description:
		Push present address onto stack;
		jump to 0x0000
		*/
		CPU_restart(0x00);

		cycles = 32;
		break;
	}
	case 0xCF:
	{
		/*
		RST 0x08

		Description:
		Push present address onto stack;
		jump to 0x0008
		*/
		CPU_restart(0x08);

		cycles = 32;
		break;
	}
	case 0xD7:
	{
		/*
		RST 0x10

		Description:
		Push present address onto stack;
		jump to 0x0010
		*/
		CPU_restart(0x10);

		cycles = 32;
		break;
	}
	case 0xDF:
	{
		/*
		RST 0x18

		Description:
		Push present address onto stack;
		jump to 0x0018
		*/
		CPU_restart(0x18);

		cycles = 32;
		break;
	}
	case 0xE7:
	{
		/*
		RST 0x20

		Description:
		Push present address onto stack;
		jump to 0x0020
		*/
		CPU_restart(0x20);

		cycles = 32;
		break;
	}
	case 0xEF:
	{
		/*
		RST 0x28

		Description:
		Push present address onto stack;
		jump to 0x0028
		*/
		CPU_restart(0x28);

		cycles = 32;
		break;
	}
	case 0xF7:
	{
		/*
		RST 0x30

		Description:
		Push present address onto stack;
		jump to 0x0030
		*/
		CPU_restart(0x30);

		cycles = 32;
		break;
	}
	case 0xFF:
	{
		/*
		RST 0x38

		Description:
		Push present address onto stack;
		jump to 0x0038
		*/
		CPU_restart(0x38);

		cycles = 32;
		break;
	}
	case 0xC9:
	{
		/*
		RET

		Description:
		Pop two bytes from the stack and jump to it
		*/
		CPU_return();

		cycles = 8;
		break;
	}
	case 0xC0:
	{
		/*
		RET NZ

		Description:
		Return if Zero flag is reset
		*/

		if(!F.Z)
			CPU_return();
		else
			PC++;

		cycles = 8;
		break;
	}
	case 0xC8:
	{
		/*
		RET Z

		Description:
		Return if Zero flag is set
		*/

		if(F.Z)
			CPU_return();
		else
			PC++;

		cycles = 8;
		break;
	}
	case 0xD0:
	{
		/*
		RET NC

		Description:
		Return if Carry flag is reset
		*/

		if(!F.C)
			CPU_return();
		else
			PC++;

		cycles = 8;
		break;
	}
	case 0xD8:
	{
		/*
		RET C

		Description:
		Return if Carry flag is set
		*/

		if(F.C)
			CPU_return();
		else
			PC++;

		cycles = 8;
		break;
	}
	case 0xD9:
	{
		/*
		RETI

		Description:
		Same as normal return, but also enable intercepts
		*/
		CPU_return();

		/*delet me*/
		printf("RETI D9 PC: %X\n", PC);

		/* Looks like RETI enabled interrupts immediately */
		/*ie = 1;*/

		interrupt_direction = 1;
		interrupt_step = 2;

		cycles = 8;
		break;
	}
		/* +++++ END CALLS/RESTARTS/RETURNS +++++ */

		/* ++++++ JUMPS ++++++ */
	case 0xC3:
	{
		/*
		JP nn

		Description:
		Jump to address nn.

		Use with:
		nn = two byte immediate value. (LS byte first.)
		*/
		word address = convert_to16m(PC+1, PC+2);

		CPU_jump(0, address, 0);
		cycles = 12;
		break;
	}
	case 0xC2:
	{
		/*
		JP cc(NZ), nn

		Description:
		Jump to address nn if Z flag is reset

		Use with:
		nn = two byte immediate value. (LS byte first.)
		*/
		word address;

		address = convert_to16m(PC+1, PC+2);

		/* If condition is false, increase PC, else, jump */
		if(!CPU_jump(F.Z, address, 0))
			PC += 3;

		cycles = 12;

		break;
	}
	case 0xCA:
	{
		/*
		JP cc(Z), nn

		Description:
		Jump to address nn if Z flag is set

		Use with:
		nn = two byte immediate value. (LS byte first.)
		*/
		word address;

		address = convert_to16m(PC+1, PC+2);

		/* If condition is false, increase PC, else, jump */
		if(!CPU_jump(F.Z, address, 1))
			PC += 3;

		cycles = 12;

		break;
	}
	case 0xD2:
	{
		/*
		JP cc(NC), nn

		Description:
		Jump to address nn if C flag is reset

		Use with:
		nn = two byte immediate value. (LS byte first.)
		*/
		word address;

		address = convert_to16m(PC+1, PC+2);

		/* If condition is false, increase PC, else, jump */
		if(!CPU_jump(F.C, address, 0))
			PC += 3;

		cycles = 12;
		break;
	}
	case 0xDA:
	{
		/*
		JP cc(C), nn

		Description:
		Jump to address nn if C flag is set

		Use with:
		nn = two byte immediate value. (LS byte first.)
		*/
		word address;

		address = convert_to16m(PC+1, PC+2);

		/* If condition is false, increase PC, else, jump */
		if(!CPU_jump(F.C, address, 1))
			PC += 3;

		cycles = 12;
		break;
	}
	case 0xE9:
	{
		/*
		JP (HL)

		Description:
		Jump to the address contained in the HL register
		*/
		word address;

		convert_to_pair(&H, &L, &address);
		/*address = swap_byte_order(&address);*/

		CPU_jump(0, address, 0);
		cycles = 4;
		break;
	}
	case 0x18:
	{
		/*
		JR n

		Description:
		Add n to the current address and jump to it

		Use with:
		n = one byte signed immediate value
		*/
		s_byte nextb = memory_readb(PC + 1);

		CPU_jump(0, (PC + nextb), 0);
		cycles = 8;
		break;
	}
	case 0x20:
	{
		/*
		JR cc(NZ), n

		Description:
		If Z flag is reset, add n to current address and jump to it

		Use with:
		n = one byte signed immediate value
		*/
		s_byte nextb = memory_readb(PC + 1);
		/* updated TODO jumps seem funky, they should PC+=2 no matter what? 
			TODO updated similar one byte immediate jumps to do this
			refer back to this before shipping*/

		printf("DBG JRCC PC: %X; Z: %X\n", PC, F.Z);
		printf("DBG JRCC PC+nb: %X+%X(%i): %X\n", PC, nextb, nextb, PC+nextb);
		/* If condition is false, increase PC, else, jump */
		CPU_jump(F.Z, (PC + nextb), 0);


		/* TODO this is how these jumps were before minus the else part */
		/*if(!CPU_jump(F.Z, (PC + nextb), 0))
			PC += 2;*/
		/*else
		{
			printf("DBG JRCC ELSE PC+=2: %X\n", PC+2);*/
			/* THIS IS A HACK, FIGURE OUT WTF IS GOING ON TODO */
			/*PC += 2;
		}*/

		PC += 2;
		cycles = 8;
		break;
	}
	case 0x28:
	{
		/*
		JR cc(Z), n

		Description:
		If Z flag is set, add n to current address and jump to it

		Use with:
		n = one byte signed immediate value
		*/
		s_byte nextb = memory_readb(PC + 1);

		/* If condition is false, increase PC, else, jump */
		CPU_jump(F.Z, (PC + nextb), 1);
		
		PC += 2;
		cycles = 8;
		break;
	}
	case 0x30:
	{
		/*
		JR cc(NC), n

		Description:
		If C flag is reset, add n to current address and jump to it

		Use with:
		n = one byte signed immediate value
		*/
		s_byte nextb = memory_readb(PC + 1);

		/* If condition is false, increase PC, else, jump */
		CPU_jump(F.C, (PC + nextb), 0);
		
		PC += 2;
		cycles = 8;
		break;
	}
	case 0x38:
	{
		/*
		JR cc(C), n

		Description:
		If C flag is set, add n to current address and jump to it

		Use with:
		n = one byte signed immediate value
		*/
		s_byte nextb = memory_readb(PC + 1);

		/* If condition is false, increase PC, else, jump */
		CPU_jump(F.C, (PC + nextb), 1);
		
		PC += 2;
		cycles = 8;
		break;
	}
		/* END JUMPS */

		/* MISC */
	case 0x10:
	{
		/*
		STOP

		Description:
		Halt CPU and LCD until button press
		*/

		/* TODO */

		PC += 2;
		cycles = 4;
		break;
	}
	case 0xF3:
	{
		/*
		DI

		Description:
		Disable interupts after the next instruction
		*/

		CPU_interrupt_switch();

		/*
			Set interrupt direction to specify that
			interrupts should be disabled after step
			is zeroed
		*/
		interrupt_direction = 0;

		break;
	}
	case 0xFB:
	{
		/*
		EI

		Description:
		Enable interupts after the next instruction
		*/

		CPU_interrupt_switch();


		/*
			Set interrupt direction to specify that
			interrupts should be disabled after step
			is zeroed
		*/
		interrupt_direction = 1;

		break;
	}
	case 0x2F:
	{
		/*
		CPL

		Description:
		Complement A register

		Flags affected:
		Z - Not affected
		N - Set
		H - Set
		C - Not affected
		*/
		CPU_complement(&A);

		F.N = 1;
		F.H = 1;

		PC++;
		cycles = 4;
		break;
	}
	case 0x3F:
	{
		/*
		CCF

		Description:
		Complement Carry Flag

		If Carry flag set, reset
		If reset, set

		Flags affected:
		Z - Not affected
		N - Reset
		H - Reset
		C - Complemented
		*/
		F.N = 0;
		F.H = 0;

		if(F.C)
			F.C = 0;
		else
			F.C = 1;

		PC++;
		cycles = 4;
		break;
	}

	default:
	{
		printf("OP not handled: 0x%.2X\n", op);
		result--;
		break;
	}

	} /* Lost? This is where the switch ends. */

	return result;
}
