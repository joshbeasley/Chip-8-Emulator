#include "chip.h"
void updateLocations(struct subroutineStack *, int);
struct subroutineStack *stack_init(void){
	struct subroutineStack *stack = (struct subroutineStack *)malloc(sizeof(struct subroutineStack));
	if(stack == NULL){
		Serial.println("stack is null");
		exit(0);
	}
	stack -> size = 0;
	stack -> head = NULL;
	return stack;
}

struct subroutineStack *push(struct subroutineStack *stack, uint16_t data){
	struct node *newEntry;
	if((newEntry = (struct node *)malloc(sizeof(struct node))) == NULL){
		exit(0);
	}
	newEntry -> next = stack -> head;
	newEntry -> address = data;
	(stack -> size)++;
	stack -> head = newEntry;
	
	return stack;	
}

struct subroutineStack *pop(struct subroutineStack *stack){
	if(stack -> size == 0){
		 fprintf( stderr, "Subroutine exit call when stack is empty");
		 exit(1);
	}
	struct node* temp = stack -> head;
	stack -> head = stack -> head -> next;
	free(temp);
	(stack -> size)--;

	return stack;

}
uint16_t peek(struct subroutineStack *stack){
	if(stack -> size == 0){
		 exit(1);
	}
	return (stack -> head) -> address;
}

void printStack(struct subroutineStack *stack){
	struct node* stack_ptr = stack -> head;
	if(stack -> size == 0){
	}
	else{
		int i;
		for(i = stack -> size; i > 0; i--){
			Serial.print("stack info: ");
			Serial.println(stack_ptr -> address);
			stack_ptr = stack_ptr -> next;
		}
	}
}
void pcIncr(chip8 *c8){
	c8 -> programCounter += 2;
	if(c8 -> programCounter < 0x200){	//if an attempt is made to access memory not reserved for the interpreter, quit.
		exit(1);
	}
	if(c8 -> programCounter >= 0xFFF){	//if an attempt is made to access memory not inbounds, quit.
		exit(0);
	}

}

void loadMemory(chip8 *c8, const uint8_t *game, int gamesize){
	int j = START_LOCATION;
	
	for(int i = 0; i < gamesize; i++, j++){
		uint8_t myByte = pgm_read_byte_near(game + i);
		c8 -> dataMemory[j] = myByte;
		//Serial.println(game[i]);
		Serial.println(c8 -> dataMemory[j], HEX);
	}
	c8 -> dataMemory[j] = '\0';
}

void getBits(uint8_t *bits, uint8_t data){
	int i;
	int j;
	for(i = 7, j = 0; i >= 0; i--, j++){
		bits[i] = (data >> j) & 0x1;
	}
	bits[8] = '\0';
}

void writeBits(uint8_t *bits, uint8_t x, uint8_t y, chip8 *c8){
	int i;
	uint8_t flag = 0x00;
	for(i = 0; i < 8; i++){
		if(c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_H] == bits[i]){
			c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_H] = BLACK;		//if both are white pixels or both are black pixel, the pixel will change to black: XOR mode
			if(bits[i] == WHITE){
				c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_H] = CHANGE_XOR;
				flag = 0x01;
			}
		}
		else if(c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_H] == CHANGE_XOR){
			c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_H] = bits[i];
		}
		else{
			c8 -> chipScreen.binaryDisplay[(x + i) % SCREEN_W][y % SCREEN_H] = WHITE; 		//if there are different colors, then the pixel will change to WHITE
		}
	}	
	c8 -> dataRegister[0xF] = flag;
}

void loadSprites(uint8_t *sprites, uint8_t *memory){
	uint16_t start;

	for(start = 0x000; sprites[start] != '\0'; start++){
		memory[start] = sprites[start];
	}


}

void countTimer(chip8 *c8){		//for counting down the delay timer
	if(c8 -> delayTimer != 0x00){
		c8 -> delayTimer--;
	}
}

uint8_t find(uint16_t opcode){

	switch( (opcode >> 12) & 0xF){
		case 0x0:
			switch(opcode & 0xF){
				case 0x0:
					if(((opcode >> 4) & 0xF) == 0){
						return 0;
					}
					else{
						return 1;
					}
					break;
				case 0xE:
					return 2;
					break;
				default:
					break;
			}
			break;
		case 0x1:
			return 3;	
			break;
		case 0x2:
			return 4;
			break;
		case 0x3:
			return 5;	
			break;
		case 0x4:
			return 6;	
			break;
		case 0x5:
			return 7;
			break;
		case 0x6:
			return 8;
			break;
		case 0x7:
			return 9;	
			break;
		case 0x8:
			switch(opcode & 0xF){
				case 0x0:
					return 10;
					break;
				case 0x1:
					return 11;
					break;
				case 0x2:
					return 12;
					break;
				case 0x3:
					return 13;
					break;
				case 0x4:
					return 14;
					break;
				case 0x5:
					return 15;
					break;
				case 0x6:
					return 16;
					break;
				case 0x7:
					return 17;
					break;
				case 0xE:
					return 18;
					break;
				default:
					break;
			}

			break;
		case 0x9:
			return 19;
			break;
		case 0xA:
			return 20;	
			break;
		case 0xB:
			return 21;	
			break;
		case 0xC:
			return 22;	
			break;
		case 0xD:
			return 23;	
			break;
		case 0xE:
			switch(opcode & 0xF){
				case 0xE:
					return 24;	
					break;
				case 0x1:
					return 25;
					break;
				default:
					break;
			}
			break;
		case 0xF:
			switch((opcode >> 4) & 0xF){
				case 0x0:
					switch(opcode & 0xF){
						case 0x7:
							return 26;
							break;
						case 0xA:
							return 27;
							break;
						default:
							break;
					}
					break;
				case 0x1:
					switch(opcode & 0xF){
						case 0x5:
							return 28;
							break;
						case 0x8:
							return 29;
							break;
						case 0xE:
							return 30;
							break;
						default:
							break;
					}
					break;
				case 0x2:
					return 31;
					break;
				case 0x3:
					return 32;
					break;
				case 0x5:
					return 33;	
					break;
				case 0x6:
					return 34;
					break;

				default:
					break;
			}

			break;
		default:
			return 0;
			break;
	}
	
}
