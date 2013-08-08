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
#include "cpu.h"

/*
Resets the CPU's registers to the state it should be in after the BIOS
releases control of the system.
*/
void CPU_reset()
{
	A = 0x01;
	B = 0;
	C = 0x13;
	D = 0;
	E = 0xD8;
	H = 0x01;
	L = 0x4D;

	F.Z = 1;
	F.N = 0;
	F.H = 1;
	F.C = 1;
	F.F = 0xB0;

	/*AF = 0x01B0;
	BC = 0x0013;
	DE = 0x00D8;
	HL = 0x014D;*/

	PC = 0x0100;
	SP = 0xFFFE;

	memory[0xFF05] = 0x00;
	memory[0xFF06] = 0x00;
	memory[0xFF07] = 0x00;
	/* TODO finish these */

	memory[0xFF40] = 0x91;
	memory[0xFF42] = 0x00;
	memory[0xFF43] = 0x00;
	memory[0xFF45] = 0x00;
	memory[0xFF47] = 0xFC;
	memory[0xFF48] = 0xFF;
	memory[0xFF49] = 0xFF;
	memory[0xFF4A] = 0x00;
	memory[0xFF4B] = 0x00;
	memory[0xFFFF] = 0x00;

	cycles = 0;
	max_cycles = 69905;
	ie = 0;
	interrupt_step = 0;
}




/* +++++++++ CPU HELPER FUNCTIONS +++++++++ */



/* ++++ JUMPS ++++ */

/*
	Most of the jumps are conditional based on the Flag registers,
	this function will cover the majority of them, the remaining
	few can do the necessary work within the CPU switch and
	send a 0 in the flag and condition parameter

	Returns 0 if the condition wasn't true, this is done so that
	if there were no jump, the switch case can increase the PC to
	prevent endless loops. Returns 1 on success.
*/
byte CPU_jump(byte flag, word address, byte condition)
{
	byte ret = 1;

	if(flag == condition)
		PC = address;
	else
		ret = 0;

	return ret;
}


/* ++++ LOADS ++++ */

/* 8-BIT LOADS */

/* Load immediate data(PC + 1) into register */
void CPU_load_immediate(byte *reg)
{
	*reg = memory_readb(PC + 1);
	PC += 2;
}

/* Load the value in the from register into the to register */
void CPU_load_register(byte *to, byte *from)
{
	*to = *from;
	PC += 1;
}

/*
	Load the value at address into a register or vice versa
	direction = 0: load register with the data at address
	direction = 1: load data at address with register
*/
void CPU_load_address(byte *reg, word address, byte direction)
{
	if(direction == 0)
		*reg = memory_readb(address);
	else
		memory_writeb(address, *reg);

	PC += 1;
}

/* 16-BIT LOADS */

/*
	Load the immediate 16-bit value into a register pair
*/
void CPU_load_immediate16(byte *pair_a, byte *pair_b)
{
	/* These are switched TODO */
	*pair_a = memory_readb(PC + 2);
	*pair_b = memory_readb(PC + 1);

	PC += 3;
}

/*
	Push two bytes onto the stack, used with register
	pairs or immediate addresses.
*/
void CPU_load_sp_16(byte *pair_a, byte *pair_b)
{
	SP_push(*pair_a);
	SP_push(*pair_b);
}

/*
	Pop two bytes off the stack into registers or
	immediate addresses.
*/
void CPU_load_16_sp(byte *pair_a, byte *pair_b)
{
	SP_pop(pair_a);
	SP_pop(pair_b);
}

/* ++++ ALU ++++ */

/* 8-BIT ALU */

/*
	Add a byte to the A register, if Carry is
	set, then add 1 to the toAdd value (this will
	cover all of the add + Carry opcodes as well
	as the normal 8-bit add)

	Zero flag is set if the addition results in 0
	Carry flag is set if the addition overflows
	Half Carry flag is set if there is a carry
		from bit 3 to bit 4.
*/
void CPU_add_8(byte* toAdd, byte Carry)
{
	/*
	Example:
		A = 1011-1001
		toAdd = 1111-1111
	*/
	byte half = A;
	CPU_clear_flags();

	/*
		If Carry is set to 1, add it to toAdd,
		if it's 0, this will not effect the toAdd
		value

		This is to cover the Add + Carry flag instructions
		into the same function
	*/
	*toAdd += Carry;

	/* If A will overflow after the addition, set Carry flag */
	/* If:
		(1011-1001 + 1111-1111) > 1111-1111
	*/
	if((A + *toAdd) > 0xFF)
		F.C = 1;

	A += *toAdd;

	/* If A is zero after the addition, set Zero flag */
	if(A == 0)
		F.Z = 1;

	half = (half & 0xF);
	/* half = 1001 (lower nibble of original A) */
	half += (*toAdd & 0xF);
	/* half += 1111 (lower nibble of toAdd) */

	/*
		If half is greater than 0xF, then we know that
		there was a Carry from the lower to upper nibble
		and that the Half Carry flag should be set.

		In our example, half is now equal to 0001-1000,
		so Half Carry should be set
	*/
	if(half > 0xF)
		F.H = 1;
}

/*
	Subtract a byte from the A register, if Carry is set,
	add one to toSub (this covers the sub + Carry instructions)

	This is pretty much the same as add but for subtraction

	Zero flag is set if the subtraction results in a 0
	Subtract flag is set because subtraction will be used
		(obvious flag is obvious)
	Half Carry flag is set if there's no borrow from bit 4
	Carry flag is set if there's no borrow(if the result < 0)
*/
void CPU_subtract_8(byte *toSub, byte Carry)
{
	/*
	Example:
		A = 1011-1001
		toSub = 1111-1111
	*/
	byte half = A;
	CPU_clear_flags();

	/*
		If Carry is set, toSub will be incremented
		if not, toSub will be the same

		This is so a single function can handle both
		the regular 8 bit subtraction instructions
		as well as the 8 bit sub + Carry instructions
	*/
	*toSub += Carry;

	/*
		Set the subtraction flag as this function will
		likely use subtraction
	*/
	F.N = 1;

	/*
		If the result of A - toSub is less than 0, then
		set the Carry flag

		If:
		(1011-1001 - 1111-1111) < 0000-0000
	*/
	if((A - *toSub) < 0)
		F.C = 1;

	/* Do the actual subtraction */
	A -= *toSub;

	/* If the result of the subtraction is 0, set the Zero flag */
	if(A == 0)
		F.Z = 1;

	half = (half & 0xF);
	/* half = 1001 (lower nibble of half) */
	half -= (*toSub & 0xF);
	/* half -= 1111 (lower nibble of toSub) */

	/*
		If half is less than zero, then we borrowed from bit 4

		In our example, half is now equal to -6(9-15), so we
		need to set the Half Carry flag
	*/
	if(half < 0)
		F.H = 1;
}

/*
	Bitwise AND a byte with A and put the result into A

	Zero flag is set if the result is 0
	Subtract flag is reset
	Half Carry flag is set
	Carry flag is reset
*/
void CPU_and_8(byte *toAnd)
{
	CPU_clear_flags();

	/* Do the actual ANDing */
	A &= *toAnd;

	/* If result is zero, set Zero flag */
	if(A == 0)
		F.Z = 1;

	/* Set Half Carry flag */
	F.H = 1;
}

/*
	Bitwise OR a byte with A and put the result into A

	Zero flag is set if the result is 0
	Subtract flag is reset
	Half Carry flag is reset
	Carry flag is reset
*/
void CPU_or_8(byte *toOR)
{
	CPU_clear_flags();

	/* Do the ORing */
	A |= *toOR;

	/* If the result was 0, set Zero flag */
	if(A == 0)
		F.Z = 1;
}

/*
	Bitwise XOR a byte with A and put the result into A

	Zero flag is set if the result is 0
	Subtract flag is reset
	Half Carry flag is reset
	Carry flag is reset
*/
void CPU_xor_8(byte* toXOR)
{
	CPU_clear_flags();

	/* Do the XORing */
	A ^= *toXOR;

	/* If the result was 0, set Zero flag */
	if(A == 0)
		F.Z = 1;
}

/*
	Compare a byte with A

	Zero flag is set if A == byte
	Subtract flag is set
	Half Carry flag is set if no borrow from bit 4
	Carry is set if A < byte

	Since this instruction is essentially the subtract
	instruction with a couple special flag conditions
	where the results are thrown away, we will piggy
	back on the subtract function
*/
void CPU_compare_8(byte *toCompare)
{
	/*
		Make a backup of A so we can reinstate
		A to clear the results of the subtract
	*/
	byte before = A;

	/*
		We'll call subtract as it will set the
		subtract flag and properly work out the
		Half Carry flag for us
	*/
	CPU_subtract_8(toCompare, 0);

	/*
		If the original A is the same as the
		compared value, set the Zero flag
	*/
	if(before == *toCompare)
		F.Z = 1;

	/*
		If the original A is less than the
		compared value, set the Carry flag
	*/
	if(before < *toCompare)
		F.C = 1;

	/*
		Since we don't care about the result
		in this instruction (just the flags),
		we'll reinstate the original value of
		A here
	*/
	A = before;
}

/*
	Increment or decrement a register or byte

	direction == 0: increment
	direction == 1: decrement

	Zero flag will be set if the result is 0
	Subtract flag will be set if decrementing, reset
		if increment
	Half Carry will be set if there's a cary from bit 3 if doing an
		increment; set if no borrow from bit 4 on decrement
	Carry flag is not affected, resetting it
*/
void CPU_incdec_8(byte *reg, byte direction)
{
	CPU_clear_flags();

	if(direction == 0)
	{
		/*
			If the lower nibble of reg is 1111, then
			the Half Carry flag should be set as adding
			one to this will cause a cary from bit 3
		*/
		if((*reg & 0xF) == 0xF)
			F.H = 1;

		*reg += 1;
	}
	else
	{
		F.N = 1;

		/*
			If the lower nibble of reg is 0, then
			the Half Carry flag should be set as subtracting
			one from this will mean there was no borrow
			from bit 4
		*/
		if((*reg & 0xF) == 0)
			F.H = 1;

		*reg -= 1;
	}

	/* If reg is zero, set Zero flag */
	if(*reg == 0)
		F.Z = 1;
}



/* 16-BIT ALU */

/*
	Add two 16-bit numbers(a pair), leave result in pair 1

	Zero flag unaffected(reset)
	Subtract flag reset
	Half Carry set if carry from bit 11
	Carry set if carry from bit 15
*/
void CPU_add_16(byte *pair1_a, byte *pair1_b, byte *pair2_a, byte *pair2_b)
{
	word pair1, pair2;

	CPU_clear_flags();

	/* Convert individual registers to pairs */
	convert_to_pair(pair1_a, pair1_b, &pair1);
	convert_to_pair(pair2_a, pair2_b, &pair2);

	/*
		If the two added pairs are larger
		than 16 bits, then set the Carry flag
	*/
	if((pair1 + pair2) > 0xFFFF)
		F.C = 1;

	/*
		If the two added pairs are larger
		than 12 bits, set the Half Carry
		flag
	*/
	if(((pair1 & 0xFFF) + pair2) > 0xFFF)
		F.H = 1;

	/* Do the actual adding */
	pair1 += pair2;

	/*
		Update the individual registers with
		the calculated numbers
	*/
	convert_to_bytes(pair1_a, pair1_b, &pair1);
}

/*
	Add byte to SP

	Zero flag is reset
	Subtract flag is reset
	Half Carry flag is set on carry from bit 3 to 4
	Carry flag is set on carry from bit 7 to 8
*/
void CPU_add_sp_n(byte immediate)
{
	CPU_clear_flags();

	/*
		The result will be the value of SP plus the
		immediate byte(n), since this may go over the
		limits of a 16-bit variable, we will want to
		limit the result to the size of a word. ANDing
		the result with 0xFFFF will ensure that result
		will only be the last 16 bits of the operation.

		Ex.
		FF9B + CB = 10066(overflow)
		10066 AND FFFF = 66(good)

		The extra data will be used to calculate Carry
		and Half carry flags
	*/
	SP = (SP + immediate) & 0xFFFF;

	/*
		Carry is set when there's an overflow
		from bit 7 to bit 8(00FF to 010F). Here we will
		grab the lower byte from SP(SP & 0xFF) and add n
		to it, if it overflows from 7 to 8, then we need
		to set the carry flag
	*/
	if(((SP & 0xFF) + immediate) > 0xFF)
	{
		F.C = 1;
	}

	/*
		The Half carry flag is set when there's an overflow
		from bit 3 to bit 4(000F to 0010). This is just
		ike the carry flag, except we need just the lower
		4 bits from both SP and n(SP & 0xF; n & 0xF), if
		these combined nibbles are greater than 0xF, then
		the half carry should be set.
	*/
	if(((SP & 0xF) + (immediate & 0xF)) > 0xF)
	{
		F.H = 1;
	}
}

/*
	Increment or decrement a register pair

	direction == 0: Increment
	direction == 1: Decrement

	Flags affected:
	None!
*/
void CPU_incdec_16(byte *pair_a, byte *pair_b, byte direction)
{
	word pair;

	/*
		group the two registers into a word for
		easy manipulation
	*/
	convert_to_pair(pair_a, pair_b, &pair);

	/* Increment or decrement */
	if(direction == 0)
		pair++;
	else
		pair--;

	/* Update the registers with the manipulated pair */
	convert_to_bytes(pair_a, pair_b, &pair);
}

/* ++++++ CALLS, RESTARTS, AND RETURNS ++++++ */

/*
	Push next address onto the stack and jump to the 2 byte
	immediate address
*/
void CPU_call()
{
	word address;
	byte high, low;

	printf("DBG CALL CURRENT OP: %X\n", PC);

	/* Get next instruction address */
	address = PC + 3;

	printf("DBG CALL NEXT INSTR: %X\n", PC+3);

	/* Break next instruction's address into bytes */
	convert_to_bytes(&low, &high, &address);

	printf("DBG CALL HIGH/LOW PUSH: %X%X\n", high, low);

	/* Push the instruction's bytes onto the stack */
	SP_push(low);
	SP_push(high);

	/* Get immediate two-byte value */
	high = memory_readb(PC+1);
	low = memory_readb(PC+2);

	printf("DBG CALL HIGH;LOW: %X;%X\n", high, low);

	/* Convert to LSB-first address */
	convert_to_pair(&low, &high, &address);

	printf("DBG CALL PAIRED ADDRESS: %X\n", address);

	/* Finally, we jump, but we do not ask how high */
	PC = address;
	/* TODO DELETE THIS DEBUG CALLS */
	printf("DBG CALL PC: %X\n", PC);
}

/*
	Push current address onto the stack and jump to an
	offset

	TODO clarify restart
	note: all documentation says current address but it
	appears that current means the OP after restart
		i.e. next instruction
*/
void CPU_restart(byte offset)
{
	byte lower, upper;
	word address = PC + 1;

	printf("DBG RST OFFSET: %X\n", offset);

	/* Break the current address into bytes
		NOTE adding +1 to PC to satisfy
		possible weirdness with restarts*/
	convert_to_bytes(&lower, &upper, &address);

	/* Push current addres onto stack, LSB first */
	SP_push(lower);
	SP_push(upper);

	/* Jump to 0x00 + offset */
	PC = offset;
}

/*
	Pop an address from the stack and jump to it
*/
void CPU_return()
{
	word address;
	byte temp;

	/* Pop the most significant byte and insert */
	SP_pop(&temp);
	printf("DBG RET TEMP1: %X\n", temp);
	address = temp;
	printf("DBG RET ADDRESS1: %X\n", address);

	/*
		Push the MSB to the left side of the word
		????????-MMMMMMMM -> MMMMMMMM-????????
	*/
	address <<= 8;
	printf("DBG RET ADDRESS2: %X\n", address);

	/* Pop the least significant byte and insert into address */
	SP_pop(&temp);
	printf("DBG RET TEMP2: %X\n", temp);
	address += temp;
	printf("DBG RET ADDRESS3: %X\n", address);

	/* Jump */
	PC = address;
}

/* ++++++ ROTATES AND SHIFTS ++++++ */

/*
	Rotates the bits in a register and saves a certain bit in the
	flag register's carry bit.

	reg - Register to be rotated, duh
	direction - Rotate left of right (0 == left, 1 == right)
*/
void CPU_rotate(byte *reg, byte direction)
{
	if(direction == 0)
	{
		/* Save the left-most bit in the Carry flag */
		F.C = *reg & 0x80;
		/* Shift left one position */
		*reg <<= 1;
		/* Add the most sig. bit back to the right side */
		*reg += F.C;
	}
	else
	{
		/* Save the right-most bit in the Carry flag */
		F.C = *reg & 0x01;
		/* Shift right */
		*reg >>= 1;

		/*
			If the carry flag is set, then we need to set
			the most significant bit. Shifting leaves 0's
			in the data, so if the Carry bit is zero, then
			we're already done
		*/
		if(F.C == 1)
		{
			*reg = *reg | 0x80;
		}
	}

	/* Check to see if register is empty, if it is, set Zero flag */
	if(*reg == 0)
		F.Z = 1;
	else
		F.Z = 0;

	/* Reset N and H flag */
	F.N = 0;
	F.H = 0;
}

/*
	This is exactly the same as rotate, except the Carry flag is
	included in the rotation, i.e. most significant bit is pushed
	to the Carry flag and the bit in the Carry flag is pushed to
	the least significant bit in the register.

	NOTE: Please see the other rotate function if you're lost

	reg - Register to be rotated
	direction - Rotate left or right(0 == left, 1 == right)
*/
void CPU_rotate_through(byte *reg, byte direction)
{
	byte temp = F.C;

	if(direction == 0)
	{
		F.C = *reg & 0x80;
		*reg <<= 1;
		*reg += temp;
	}
	else
	{
		F.C = *reg & 0x01;
		*reg >>= 1;

		if(temp == 1)
		{
			*reg = *reg | temp;
		}
	}

	if(*reg == 0)
		F.Z = 1;
	else
		F.Z = 0;

	/* Reset N and H flags */
	F.N = 0;
	F.H = 0;
}


/* ++++++ EXTENDED ++++++ */

/*
	Set a particular bit in a memory location
*/
void CPU_set(byte bit, byte *reg)
{
	/*
		Ex.

	reg = 1001-1001
	bit = 2

	1 << bit = 1001-1X01
	|= is an OR operation, if either bit in the comparison is
		a 1, the result will be 1.

	By OR'ing whatever's in the position bit with the 1 we're
	shifting over, the result shall be one.

		1001-1001
		   |=0100
		---------
		1001-1101
	*/
	*reg |= 1 << bit;
}

/*
	Reset a bit in a memory location
*/
void CPU_res(byte bit, byte *reg)
{
	/*
		Ex:

	reg == 1111-1111
	bit = 2

	(1 << bit) = 0000-0100
	~(1 << bit) = inverse, the 1 becomes zero above^

	AND'ing a zero with whatever's in the bit position
	will become zero.

	finally:
		1111-1111
	&	------0--
	=	1111-1011

	TODO change the ridiculous bit operations to sane ones
	with good explaination like this one?
	*/
	*reg &= ~(1 << bit);
}


/* ++++++ MISC ++++++ */

/*
	Function to swap the lower and upper nibbles of an
	address/register

	Flags affected:
	Z - Set if result is zero
	N - Reset
	H - Reset
	C - Reset
*/
void CPU_swap(byte *reg)
{
	/* Ex. Reg == UUUU-LLLL */
	byte upper;

	/* Shift *reg over, leaving just the upper nibble */
	/*	upper == 0000-UUUU	*/
	upper = *reg >> 4;

	/*
		Shift *reg over, leaving the lower nibble in
		the upper nibble of "lower"
	*/
	/*	*reg == LLLL-0000	*/
	*reg <<= 4;
	/*
		Add upper back to the register
	*/
	/*	*reg == LLLL-UUUU	*/
	*reg += upper;

	CPU_clear_flags();

	/* See if Z should be set */
	if(*reg == 0)
		F.Z = 1;
}

/*
	Function complements(flips all bits) in a register
*/
void CPU_complement(byte *reg)
{
	/* A simple bit flip on the register (thanks C!) */
	*reg = ~*reg;
}

/* ++++++ INTERRUPT RELATED FUNCTIONS ++++++ */

/*
	Function handles both the Enable/Disable Interrupts
	instructions.
*/
void CPU_interrupt_switch()
{
	/* Setup the wonky EI/DI work-around */
	interrupt_step = 2;

	PC++;
	cycles = 4;
}

/* See if interrupts are enabled, if they are, see if any have fired */
void CPU_check_interrupts()
{
	/*
		If interrupt step is more than zero, then the master
		interrupt enabled flag will be getting switched soon.
	*/
	if(interrupt_step > 0)
	{
		interrupt_step--;

		/* 
			If interrupt_step hits zero, flip the master
			IE flag.
		*/
		if(interrupt_step == 0)
		{
			printf("DBG INTSTEP 0, IE-B: %i\n", ie);
			if(interrupt_direction == 1)
				ie = 1;
			else
				ie = 0;
			printf("DBG INTSTEP 0, IE-A: %i\n", ie);
		}
	}

	if(ie)
	{
		byte intfired, intenabled;
		byte bit;

		/*
			Read the interrupt fired(0xFF0F) and the
			interrupt enabled(0xFFFF) registers into bytes
			that we can work with later
		*/
		intfired = memory_readb(0xFF0F);
		intenabled = memory_readb(0xFFFF);

		/*
			If no interrupts have fired, just leave cause we
			don't want you here.
		*/
		if(intfired == 0)
			return;

		printf("DBG IE; INTF: %X, INTE: %X\n", intfired, intenabled);

		/* 
			Iterate through all possible interrupts, starting with
			the highest priority interrupt(bit 0 - V-blank)

			Interrupt registers(0xFF0F and 0xFFFF):
			bit	-	interrupt	-	priority
			0		V-blank			1
			1		LCD			2
			2		Timer			3
			3		Serial I/O*		4
			4		Joypad			5

			*Serial I/O interrupts will be ignored
		*/
		for(bit = 0; bit <= 4; bit++)
		{
			/* 
				If we're looking at the Serial I/O interrupt,
				skip it.
			*/
			if(bit == 3)
				continue;

			/*
				Check if the iterating bit is set in the
				interrupt fired register(0xFF0F).
			*/
			if(bitset(&intfired, bit))
			{
				/*
					If the interrupt has been fired, see
					if it's enabled. If both are true,
					we will service the interrupt.
				*/
				if(bitset(&intenabled, bit))
				{
					CPU_service_interrupt(bit);

					/*
						Remove this if we find out ALL
						intercepts must be serviced at
						once
					*/
					/*break;*/
				}
			}
		}
	}
}

/*
	In this function, we will handle the intercepts.

	More or less, we push the current address to the stack and jump to
	the appropriate handler in the ROM which are at set addresses
*/
void CPU_service_interrupt(byte bit)
{
	/* TODO implement a proper SP_push/pop for words */
	byte low, high;
	word address;

	printf("DBG SERV INT: %i\n", bit);

	/* Disable interrupts and reset interrupt_step */
	ie = 0;
	interrupt_step = 0;

	/* Push PC to the stack, LSB first */
	address = PC;
	printf("DBG INTSERV PC: %X\n", PC);
	convert_to_bytes(&high, &low, &address);

	printf("DBG INTSERV low: %X; high: %X\n", low, high);
	SP_push(low);
	SP_push(high);

	/* Reset the interrupt we're about to service's bit */
	/* TODO make this more clear? */
	memory[0xFF0F] &= ~(1 << bit);

	/* Jump to the interrupt handler in the ROM */
	switch(bit)
	{
		case 0:
		{
			PC = 0x40;
			break;
		}
		case 1:
		{
			PC = 0x48;
			break;
		}
		case 2:
		{
			PC = 0x50;
			break;
		}
		case 4:
		{
			PC = 0x60;
			break;
		}
	}
}

/*
	Set the appropriate bit in the flag fired register (0xFF0F)
*/
void CPU_request_interrupt(byte interrupt)
{
	byte flagreg = memory_readb(0xFF0F);
	printf("DBG REQ INT: %i\n", interrupt);

	printf("DBG REQ INT FLAG: %X\n", flagreg);
	/* Set the proper interrupt bit in the flag copy */
	setbit(&flagreg, interrupt);

	/* Update real interrupt flag with updated bits */
	memory_writeb(0xFF0F, flagreg);

	printf("DBG REQ INT READ: %X\n", memory_readb(0xFF0F));
}

/* ++++ MISC. CPU-RELATED HELPER FUNCTIONS ++++ */

/* Function to convert two bytes into a word */
word convert_to16m(word location1, word location2)
{
	word result;
	/* 0000-0000 */
	byte least = memory_readb(location1);
	byte most = memory_readb(location2);
	
	/*
		Put the significant byte into the 16-bit variable(word)
		and shift it left 8 places, then add the least significant
		byte to the remaining space and return
	*/
	result = most << 8;
	/*
	0000-mmmm
	mmmm-0000
	*/
	result += least;
	/* mmmm-llll */

	return result;
}

/*
	Registers within the GameBoy can be paired in order to simulate
	16-bit registers, these 16-bit registers are not the result
	of pairing within this program, so we need this function to
	update the paired 16-bit registers(AF, BC, etc.) with the
	values within the individual 8-bit registers
*/
void convert_to_pair(byte *reg1, byte *reg2, word *fullreg)
{
	/*
		Set fullreg's LSB to reg1
		????-reg1
	*/
	*fullreg = *reg1;

	/*
		Move reg1 over to the MSB
		reg1-0000
	*/
	*fullreg <<= 8;

	/*
		Fill LSB with reg2
		reg1-reg2
	*/
	*fullreg += *reg2;
}

/*
	Like convert_to_pair, sometimes we need to update the individual
	8-bit registers when the equivalent 16-bit pseudo registers are
	worked on, this function takes in a 16-bit word and breaks it
	down into the individual bytes and updates the single registers
*/
void convert_to_bytes(byte *reg1, byte *reg2, word *fullreg)
{
	word temp;
	/* fullreg == mmmm-llll */

	*reg1 = *fullreg >> 8;
	/* reg1 == 0000-mmmm */

	temp = *fullreg << 8;
	/* temp == llll-0000 */

	temp = temp >> 8;
	/* temp == 0000-llll */

	*reg2 = temp;
}

/*
	Sometimes we need to use the paired 16-bit registers as addresses,
	in order to do this within the program, we need to swap the
	high and low bytes for addressing reasons
*/
word swap_byte_order(word *reg)
{
	/* reg == llll-mmmm */
	word result = *reg << 8;
	/* result == mmmm-0000 */

	result += *reg >> 8;
	/* result == mmmm-llll */

	return result;
}

/*
	This function will either take all the individual parts
	of the F register struct and compile them into one byte
	for use in the CPU or it will break the compiled byte
	into individual variables for easy access depending on
	the value of direction

	direction:
	0: compile into a byte
	1: decompile byte into variables
*/
void compiler_F(byte direction)
{
	if(direction == 0)
	{
		/*
		Example data:
			Z = 1
			N = 0
			H = 1
			C = 1

		F:
			????-????
		*/

		F.F += F.Z;
		/* F: ????-???1 */
		F.F = F.F << 1;
		/* F: ????-??10 */

		F.F += F.N;
		/* F: ????-??10 */
		F.F = F.F << 1;
		/* F: ????-?100 */

		F.F += F.H;
		/* F: ????-?101 */
		F.F = F.F << 1;
		/* F: ????-1010 */

		F.F += F.C;
		/* F: ????-1011 */

		F.F = F.F << 4;
		/* F: 1011-0000 */
	}
	else
	{
		/*
		Example:

			F: 1011-0000

		This is basically done by shifting F.F over a
		number of places and setting the individual
		flags to whatever is in the right-most location
		*/

		F.Z = ((F.F >> 7) & 0x1);
		/*
			F.Z = 0000-0001
			F.Z = ????-???^
			F.Z = 0000-0001
		*/

		F.N = ((F.F >> 6) & 0x1);
		/*
			F.N = 0000-0010
			F.N = ????-???^
			F.N = 0000-0000
		*/

		F.H = ((F.F >> 5) & 0x1);
		/*
			F.H = 0000-0101
			F.H = ????-???^
			F.H = 0000-0001
		*/

		F.C = ((F.F >> 4) & 0x1);
		/*
			F.C = 0000-1011
			F.C = ????-???^
			F.C = 0000-0001
		*/
	}
}

/*
	Function to test whether a certain bit in a byte is set
	or reset.

	Bit - 0-7

	Return:
	0 - Reset
	1 - Set
*/
byte bitset(byte *reg, byte bit)
{
	byte temp, ret;

	/* Set temp to register */
	temp = *reg;

	/* Shift temp over to the bit in question */
	temp >>= bit;

	/* Set the return variable to the same as the bit in question */
	ret = temp & 0x01;

	return ret;
}

/*
	Set a particular bit in a byte
*/
void setbit(byte *reg, byte bit)
{
	/*
		NOTE: This can be GREATLY simplified by the following
		bitwise operation:

		*reg |= 1 << bit;

		TODO change to this?

		I have no idea why I'm writing this as a pseudo tutorial.
	*/

	byte temp, bitshift;

	/*
		Set temp to the register shifted over to the bit we want
		Ex.

		bit = 4
		Reg = 11101111
		temp = 1110;
	*/
	temp = *reg >> bit;
	/*
		Set the bit in question(now the LSB)
		temp = 1111;
	*/
	temp |= 0x01;
	/*
		Shift temp back over
		temp = 1111-0000
	*/
	temp <<= bit;

	/* If bit was zero, we're done */
	if(bit == 0)
	{
		*reg = temp;
		return;
	}

	/*
		Bitshift is how much the register needs to be shifted in order
		to leave only the bits to the right of the bit location in the
		register. Read below if this doesn't make sense.
	*/
	bitshift = 8 - bit;

	/*
		Now we're going to shift everything in the register at bit
		and left of it off of the byte so the register can be
		cleanly and easily added to temp.

		reg = 1111
	*/
	*reg <<= bitshift;
	/*
		reg = 0000-1111
	*/
	*reg >>= bitshift;

	/*
		Now merge the two:
		temp = 1111-0000 (from before)
		reg = 0000-1111 (from before)
		temp += reg:
		temp = 1111-1111
	*/
	temp += *reg;
	/*
		Set reg to the finalized temp
	*/
	*reg = temp;
}

/*
	The CPU relies on the flags register for many of the arithmetic
	operations, this function will clear the flag register state for
	new use.
*/
void CPU_clear_flags()
{
	F.Z = 0;
	F.N = 0;
	F.C = 0;
	F.H = 0;

	F.F = 0;
}

/*
	Decrement Stack Pointer(TM) and write a byte to it
*/
void SP_push(byte b)
{
	printf("DBG SP:ps B4DEC: %X; PC: %X; Data: %X\n", SP, PC, b);
	SP--;
	printf("DBG SP:ps AFDEC: %X; PC: %X; Data: %X\n", SP, PC, b);

	memory_writeb(SP, b);
}

/*
	Assign passed byte to the latest item on the stack;
	increment Stack Pointer
*/
void SP_pop(byte *reg)
{
	*reg = memory_readb(SP);
	printf("DBG SP:pop Data: %X\n", *reg);

	printf("DBG SP:pop B4INC: %X\n", SP, PC);
	SP++;
	printf("DBG SP:pop AFINC: %X\n", SP, PC);
}
