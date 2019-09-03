#include "chip.h"
#include "opcodes.h"
#include "cpu_helpers.h"
#include "games.h"

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

char keys[ROWS][COLS] = {
      {'1','2','3', 'C'},
      {'4','5','6', 'D'},
      {'7','8','9', 'E'},
      {'A','0','B', 'F'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


uint16_t tx, ty;
chip8 c8;
uint16_t instruction;

void setup()
{
  Serial.begin(2000000);
  Serial.println("RA8875 start");
  chipInit(&c8);
  
  // loadMemory(&c8, PONG, PONG_SIZE);
  loadMemory(&c8, PLANE, PLANE_SIZE);
  // loadMemory(&c8, MAZE, MAZE_SIZE);
  //loadMemory(&c8, WORM, WORM_SIZE);
  
  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)){
    Serial.println("RA8875 Not Found!");
    while (1);
  }
  
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);
  
}
// goes with drawSprite()
void write_to_screen(){
  
  for(int i = 0; i < SCREEN_H; i++){
    for(int j = 0; j < SCREEN_W; j++){
      if(c8.chipScreen.binaryDisplay[j][i] == 1){
         tft.fillRect(j*12, i*12, 12, 12, RA8875_WHITE);
      }
      else if(c8.chipScreen.binaryDisplay[j][i] == CHANGE_XOR){
         tft.fillRect(j*12, i*12, 12, 12, RA8875_BLACK);
      }
    }
  }
}
void loop()
{
  //write_to_screen();
  instruction = (c8.dataMemory[c8.programCounter] << 8) | c8.dataMemory[c8.programCounter + 1];
  switch(find(instruction)){
      case 1:
        clearScreen(&c8, instruction);
        write_to_screen();
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
        write_to_screen();
        // DXYN
        break;
      case 24:
        skipPress(&c8, instruction, keypad);
        // EX9E
        break;
      case 25:
        skipNotPress(&c8, instruction, keypad);
        // EXA1
        break;
      case 26:
        storeTimer(&c8, instruction);
        // FX07
        break;
      case 27:
        waitKeypress(&c8, instruction, keypad);
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
        exit(1);
    }
  delay(10);
  countTimer(&c8);
}
