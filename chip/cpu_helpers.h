#include "chip.h"

#ifndef CPU_HELPERS_H
#define CPU_HELPERS_H
void updateLocations(struct subroutineStack*, int);

void Output_Cpu(chip8);

struct subroutineStack *push(struct subroutineStack*, uint16_t);

struct subroutineStack *pop(struct subroutineStack*);

uint16_t peek(struct subroutineStack *);

struct subroutineStack *stack_init(void);

void printStack(struct subroutineStack *);

void pcIncr(chip8 *);

void loadMemory(chip8 *, const uint8_t *, int);

void getBits(uint8_t *, uint8_t);

void writeBits(uint8_t *, uint8_t , uint8_t , chip8 *);

void loadSprites(uint8_t *, uint8_t *);

void countTimer(chip8 *);		//for counting down the delay timer

uint8_t find(uint16_t);
#endif
