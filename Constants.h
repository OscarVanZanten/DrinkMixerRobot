#ifndef Constants_H
#define Constants_H
#include <Arduino.h>

//////////PINS//////////
#define LCDPIN1 7
#define LCDPIN2 6
#define LCDPIN3 5
#define LCDPIN4 4
#define LCDPIN5 3
#define LCDPIN6 2

#define ROWPIN1 22
#define ROWPIN2 24
#define ROWPIN3 26
#define ROWPIN4 28
#define COLUMNPIN1 30
#define COLUMNPIN2 32
#define COLUMNPIN3 34

#define RELAY1 23
#define RELAY2 25
#define RELAY3 27
#define RELAY4 29
#define RELAY5 31
#define RELAY6 33
#define RELAY7 35
#define RELAY8 37
#define RELAY9 39

#define SDCSPIN 53

///////////KEYPAD//////////////////////////////
#define ROWSIZE 4
#define COLUMNSIZE 3
byte KEYPADROWS = ROWSIZE;
byte KEYPADCOLS = COLUMNSIZE;
byte rowPins[ROWSIZE] =
{
  ROWPIN1,
  ROWPIN2,
  ROWPIN3,
  ROWPIN4
};
byte colPins[COLUMNSIZE] =
{
  COLUMNPIN1,
  COLUMNPIN2,
  COLUMNPIN3
};
char keys[ROWSIZE][COLUMNSIZE] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

/////////////////////RECIPES///////////////////
const  byte VALVES_COUNT = 9;
const  byte VALVE_PINS[VALVES_COUNT] = {RELAY1, RELAY2, RELAY3, RELAY4, RELAY5, RELAY6, RELAY7, RELAY8, RELAY9};
const  short RECIPES_COUNT = 100;
const  short RECIPES_LENGTH = 32;

/////////////////////LCD///////////////////////
#define MENU_SELECT_RECIPE 0
#define MENU_SELECT_MENU 1
#define MENU_VALVE_SELECT 2
#define MENU_VALVE_EDIT 3
#define MENU_RECIPE_SUBMENU 4
#define MENU_RECIPE_DELETE 5
#define MENU_RECIPE_ADD 6

const short ERROR_DELAY = 1000;
const char* STARTING_MESSAGE = "Starting...";
const char* STANDARD_MESSAGE = "Select recipe:";
const char* INVALID_RECIPE_MESSAGE = "INVALID RECIPE!";
const char* PREPARING_MESSAGE = "Preparing.....";
const char* INVALID_SD_CARD_MESSAGE = "No SD card found";
const char* RESTART_MACHINE_MESSAGE = "Insert and reset";
const char* MENU_HEAD_MESSAGE = "Select option:";
const char* MENU_OPTION_MESSAGE = "1:Valve 2:Recipe";
const char* VALVE_SELECT_HEAD_MESSAGE = "Select valve:";
const char* VALVE_SELECT_INVALID_MESSAGE = "INVALID VALVE!";
const char* VALVE_EDIT_HEAD_MESSAGE = "Edit valve: ";
const char* RECIPE_HEAD_MESSAGE = "Edit:   #:Back";

///////////////////SD-CARD////////////////////
const char* VALVE_FILE_NAME = "valves.txt";
const char* RECIPE_FILE_NAME = "recipes.txt";

#endif
