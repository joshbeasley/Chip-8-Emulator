#include "chip.h"
#include "cpu_helpers.h"
#include "opcodes.h"


int main(int argc, char **argv){

	chip8 c8;
	
	chipInit(&c8);
	loadMemory(&c8, argv);

/*	c8.dataRegister[0xB] = 0x3;
	c8.dataRegister[0xA] = 0xF;
	c8.dataRegister[0x0] = 0x1F;
	c8.dataRegister[0x1] = 0xF;
	c8.dataRegister[0x2] = 0xA;

	c8.iRegister = 0x501;
	storei(&c8, 0xF255);
	printf("%i %i %i\n", c8.dataMemory[0x501],c8.dataMemory[0x502],c8.dataMemory[0x503]);
	printf("%x\n", c8.iRegister);

	c8.iRegister = 0x501;
	loadi(&c8, 0xF265);
	printf("%i %i %i\n", c8.dataRegister[0x00], c8.dataRegister[0x01],c8.dataRegister[0x02]);
	printf("%x\n", c8.iRegister);



	setTimer(&c8, 0xF015);

*/

	SDL_Event event;
	uint16_t instruction;

	while(c8.on){
		instruction = (c8.dataMemory[c8.programCounter] << 8) | c8.dataMemory[c8.programCounter + 1];
		printf("|%x at %x|\n", instruction, c8.programCounter);
		Output_Cpu(c8);

//		printStack(c8.stack);
//		printf("HIT\n");
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT: 
				      	chipQuit(&c8);
				      	exit(0);
			
				default:
					break;
			}	
		}  
/*		setHex(&c8, 0xF129);
		drawSprite(&c8, 0xDAB5);
		SDL_Delay(1000);	
		setHex(&c8, 0xFB29);
		drawSprite(&c8, 0xD015);
		SDL_Delay(1000);	
		clearScreen(&c8, 0x00E0);
		SDL_Delay(1000);	
*/
		switch(find(instruction)){
			case 1:
				clearScreen(&c8, instruction);
				// 00E0
				break;
			case 2:
				returnSubroutine(&c8, instruction);
				// 00EE
				break;
			case 3:
				jumpAddress(&c8, instruction);
				// 1NNN
				break;
			case 4:
				goToSubroutine(&c8, instruction);
				// 2NNN
				break;
			case 5:
				skipEqual(&c8, instruction);
				// 3XNN
				break;
			case 6:
				skipNotEqual(&c8, instruction);
				// 4XNN
				break;
			case 7:
				skipRegEqual(&c8, instruction);
				// 5xy0
				break;
			case 8:
				storeNumber(&c8, instruction);
				// 6XNN
				break;
			case 9:
				addNumber(&c8, instruction);
				// 7XNN
				break;
			case 10:
				storeRegValue(&c8, instruction);
				// 8XY0
				break;
			case 11:
				logicalOR(&c8, instruction);
				// 8xy1
				break;
			case 12:
				logicalAND(&c8, instruction);
				// 8XY2
				break;
			case 13:
				logicalXOR(&c8, instruction);
				// 8XY3
				break;
			case 14:
				addRegister(&c8, instruction);
				// 8XY4
				break;
			case 15:
				subtract(&c8, instruction);
				// 8XY5
				break;
			case 16:
				rightShift(&c8, instruction);
				// 8XY6
				break;
			case 17:
				subtract(&c8, instruction);
				// 8XY7
				break;
			case 18:
				leftShift(&c8, instruction);
				// 8XYE
				break;
			case 19:
				skipRegNotEqual(&c8, instruction);
				// 9XY0
				break;
			case 20:
				storeMemoryAddress(&c8, instruction);
				// ANNN
				break;
			case 21:
				jumpAddress(&c8, instruction);
				// BNNN
				break;
			case 22:
				randomNum(&c8, instruction);
				// CXNN
				break;
			case 23:
				drawSprite(&c8, instruction);
				// DXYN
				break;
			case 24:
				skipPress(&c8, instruction);
				// EX9E
				break;
			case 25:
				skipNotPress(&c8, instruction);
				// EXA1
				break;
			case 26:
				storeTimer(&c8, instruction);
				// FX07
				break;
			case 27:
				waitKeypress(&c8, instruction);
				// FX0A
				break;
			case 28:
				setTimer(&c8, instruction);
				// FX15
				break;
			case 29:
				setSoundTimer(&c8, instruction);
				// FX18
				break;
			case 30:
				addRegI(&c8, instruction);
				// FX1E
				break;
			case 31:
				setHex(&c8, instruction);
				// FX29
				break;
			case 32:
				binDec(&c8, instruction);
				// Fx33
				break;
			case 33:
				storei(&c8, instruction);
				// FX55
				break;
			case 34:
				loadi(&c8, instruction);
				// FX65
				break;
			default:
				chipQuit(&c8);
				printf("invalid instruction\n");
				exit(1);
		}
/*		printf("V0 = %x ", c8.dataRegister[0x0]);
		printf("V1 = %x ", c8.dataRegister[0x1]);
		printf("V2 = %x ", c8.dataRegister[0x2]);
		printf("V3 = %x ", c8.dataRegister[0x3]);
		printf("V4 = %x ", c8.dataRegister[0x4]);
		printf("V5 = %x ", c8.dataRegister[0x5]);
		printf("V6 = %x ", c8.dataRegister[0x6]);
		printf("V7 = %x ", c8.dataRegister[0x7]);
		printf("V8 = %x ", c8.dataRegister[0x8]);
		printf("V9 = %x ", c8.dataRegister[0x9]);
		printf("Va = %x ", c8.dataRegister[0xA]);
		printf("Vb = %x ", c8.dataRegister[0xB]);
		printf("Vc = %x ", c8.dataRegister[0xC]);
		printf("Vd = %x ", c8.dataRegister[0xD]);
		printf("Ve = %x ", c8.dataRegister[0xE]);
		printf("Vf = %x\n", c8.dataRegister[0xF]);
		printf("I = %x\n", c8.iRegister);

*/

		SDL_Delay(20);
		countTimer(&c8);
	}
}
