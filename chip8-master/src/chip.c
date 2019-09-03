#include "chip.h"
#include "cpu_helpers.h"

uint8_t builtInSprites[81] = { 0xF0,0x90, 0x90,0x90, 0xF0,	//0 starts at 0x000
		    0x20,0x60, 0x20,0x20, 0x70,	//1 starts at 0x005
		    0xF0,0x10, 0xF0,0x80, 0xF0,	//2 starts at 0x00A
		    0xF0,0x10, 0xF0,0x10, 0xF0,	//3 starts at 0x00F
		    0x90,0x90, 0xF0,0x10, 0x10,	//4 starts at 0x014
		    0xF0,0x80, 0xF0,0x10, 0xF0,	//5 starts at 0x019
		    0xF0,0x80, 0xF0,0x90, 0xF0,	//6 starts at 0x01E
		    0xF0,0x10, 0x20,0x40, 0x40,	//7 starts at 0x023
		    0xF0,0x90, 0xF0,0x90, 0xF0,	//8 starts at 0x028
		    0xF0,0x90, 0xF0,0x10, 0xF0,	//9 starts at 0x02D
		    0xF0,0x90, 0xF0,0x90, 0x90,	//A starts at 0x032
		    0xE0,0x90, 0xE0,0x90, 0xE0,	//B starts at 0x037
		    0xF0,0x80, 0x80,0x80, 0xF0,	//C starts at 0x03C
		    0xE0,0x90, 0x90,0x90, 0xE0,	//D starts at 0x041
		    0xF0,0x80, 0xF0,0x80, 0xF0,	//E starts at 0x046
		    0xF0,0x80, 0xF0,0x80, 0x80,	//F starts at 0x04C
		    '\0'};

void chipInit(chip8 *c8){
	
	c8 -> programCounter = START_LOCATION;			//starting memory
	c8 -> iRegister = 0x0;
	c8 -> delayTimer = 0x0;
	c8 -> soundTimer = 0x0;
	c8 -> stack = stack_init();
	memset(c8 -> dataRegister, 0x00, REG_LOCATIONS * sizeof(uint8_t));
	memset(c8 -> dataMemory, 0x00, MEM_SIZE * sizeof(uint8_t));
	memset(c8 -> chipScreen.binaryDisplay,BLACK ,SCREEN_W * SCREEN_H * sizeof(c8 -> chipScreen.binaryDisplay[0][0]));
	c8 -> on = 1;

	loadSprites(builtInSprites, c8 -> dataMemory);

	if((SDL_Init(SDL_INIT_EVERYTHING)) != 0 ){
		printf("Unsuccessful initialization\n");
		exit(1);
	}
	else{
		printf("successful initialization\n");
	}
	
	c8 -> chipScreen.name = "chip8_chipScreen";
	c8 -> chipScreen.window = NULL;
	c8 -> chipScreen.renderer = NULL;

	c8 -> chipScreen.window = SDL_CreateWindow(c8 -> chipScreen.name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_W * SCALE,
			SCREEN_H * SCALE,
			0);
	if(c8 -> chipScreen.window == NULL){
		printf("Can't create window\n");
	}

	c8 -> chipScreen.renderer = SDL_CreateRenderer(c8 -> chipScreen.window,
			-1, SDL_RENDERER_ACCELERATED);

	setScreen(&(c8 -> chipScreen));		//set screen to all black
	
	SDL_RenderPresent(c8 -> chipScreen.renderer);
	

}

void chipQuit(chip8 *c8){
	SDL_DestroyRenderer(c8 -> chipScreen.renderer);
	SDL_DestroyWindow(c8 -> chipScreen.window);
	c8 -> on = 0;
	c8 -> chipScreen.renderer = NULL;
	c8 -> chipScreen.window = NULL;
	SDL_Quit();
	exit(1);
	printf("chip8 quit\n");
}





