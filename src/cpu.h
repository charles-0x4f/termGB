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

/* CPU.h */

#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "memory.h"

/* REGISTERS */
/* Declare the 8-bit CPU registers */
byte A;
byte B;
byte C;
byte D;
byte E;
byte H;
byte L;

/* Flag register, implemented as struct */
struct fREG {
	/* Zero bit, set if math op results in 0/CP is true */
	byte Z;

	/* Subtract flag bit, set if subtraction performed in last
		math operation */
	byte N;

	/* Half cary bit, set if carry occured from lower nibble in
		last math operation */
	byte H;

	/* Carry bit, set if carry occured from last math op/
		register A is smaller during CP */
	byte C;

	/*
		The flag(F) register is a byte that is comprised
		of the previous flags in the upper half and zeros
		in the lower: ZNHC-0000; this byte will be used
		to store the compiled F register
	*/
	byte F;
} F;

/* Declare the 16-bit CPU registers */
word SP; /* Stack pointer */
word PC; /* Program counter */

/* +++++ END REGISTERS +++++ */



/* +++++++ EMULATION-RELATED VARIABLES +++++++ */

/* Hold the number of cycles an instruction takes to execute */
int cycles;
/* Used as a total number of cycles for use in timing */
int total_cycles;
/* Hold the maximum number of cycles between Vsyncs */
int max_cycles;
/* Master interupt enable switch */
byte ie;
/*
	The enable/disable instructions are silly gooses and
	don't actually set or reset until after the instruction
	following them. This variable will be used to emulate
	this function. I would like this comment to be just a
	little bit longer. This should satisfy my want.
*/
byte interrupt_step;
/*
	Note whether interrupts will be disabled (0) or enabled (1)
	after interrupt step is zeroed
*/
byte interrupt_direction;



/* ++++ FUNCTIONS ++++ */

void CPU_reset();
int CPU(word);
int CPU_EXTENDED(word);

/* ++++ JUMP ++++ */
byte CPU_jump(byte, word, byte);

/* ++++ LOADS ++++ */

/* 8-BIT LOADS */
void CPU_load_immediate(byte*);
void CPU_load_register(byte*, byte*);
void CPU_load_address(byte*, word, byte);

/* 16-BIT LOADS */
void CPU_load_immediate16(byte*, byte*);
void CPU_load_sp_16(byte*, byte*);
void CPU_load_16_sp(byte*, byte*);

/* ++++ ALU ++++ */

/* 8-BIT ALU */
void CPU_add_8(byte*, byte);
void CPU_subtract_8(byte*, byte);
void CPU_and_8(byte*);
void CPU_or_8(byte*);
void CPU_xor_8(byte*);
void CPU_compare_8(byte*);
void CPU_incdec_8(byte*, byte);

/* 16-BIT ALU */
void CPU_add_16(byte*, byte*, byte*, byte*);
void CPU_add_sp_n(byte);
void CPU_incdec_16(byte*, byte*, byte);

/* ++++ END ALU ++++ */

/* CALLS, RESTARTS, AND RETURNS */
void CPU_call();
void CPU_restart(byte);
void CPU_return();

/* ROTATES AND SHIFTS */
void CPU_rotate(byte*, byte);
void CPU_rotate_through(byte*, byte);

/* EXTENDED */
void CPU_bit(byte, byte*);
void CPU_set(byte, byte*);
void CPU_res(byte, byte*);

/* MISC */
void CPU_decimal_adjust();
void CPU_swap(byte*);
void CPU_compliment(byte*);

/* ++++++ INTERRUPT RELATED FUNCTIONS ++++++ */
void CPU_interrupt_switch();
void CPU_check_interrupts();
void CPU_service_interrupt(byte);
void CPU_request_interrupt(byte);

/* ++++ CPU HELPER FUNCTIONS ++++ */
word convert_to16m(word, word);
void convert_to_pair(byte*, byte*, word*);
void convert_to_bytes(byte*, byte*, word*);
word swap_byte_order(word*);
void compiler_F(byte);
byte bitset(byte*, byte);
void setbit(byte*, byte);

void CPU_clear_flags();

void SP_push(byte);
void SP_pop(byte*);

#endif
