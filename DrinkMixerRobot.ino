#include "Constants.h"
#include <SD.h>
#include <SPI.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "Storage.h"
#include "Valve.h"
#include "DrinkMaker.h"
#include "Recipe.h"
#include "LCD.h"
using byte = signed byte;

///////////KEYPAD//////////////////////////////
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int currentSelectedRecipe = 0;

/////////////////////RECIPES///////////////////
Valve valves[VALVES_COUNT];
Recipe recipes[ RECIPES_COUNT];
DrinkMaker maker = DrinkMaker(valves, recipes);

/////////////////////LCD///////////////////////
LiquidCrystal lcdDisplay(LCDPIN1, LCDPIN2, LCDPIN3, LCDPIN4, LCDPIN5, LCDPIN6);
LCD lcd(&lcdDisplay, ERROR_DELAY, STARTING_MESSAGE, STANDARD_MESSAGE, INVALID_RECIPE_MESSAGE, PREPARING_MESSAGE, INVALID_SD_CARD_MESSAGE, RESTART_MACHINE_MESSAGE);

///////////////////SD-CARD////////////////////
Storage storage = Storage(&lcd, valves, recipes);

void setup() {
  Serial.begin(9600);
  pinMode(SDCSPIN, OUTPUT);
  
  lcdDisplay.begin(16, 2);
  lcd.showStarting();

  storage.setupSDCard();
  storage.loadValves();
  storage.loadRecipes();

  lcd.showMenu(0);
}


void loop()
{
  if (!storage.loadedSuccesfull)
  {
    return;
  }

  char key = kpd.getKey();

  if (key) // Check for a valid key.
  {
    switch (key)
    {
      case '#':
        if (currentSelectedRecipe - 1 >= RECIPES_COUNT ||  currentSelectedRecipe == 0)
        {
          lcd.flashError();
        } else
        {
          lcd.showPreparing();
          maker.MakeRecipe(currentSelectedRecipe - 1);
        }
        currentSelectedRecipe = 0;
        lcd.showMenu(currentSelectedRecipe);
        break;
      case '*':
        currentSelectedRecipe = 0;
        lcd.showMenu(currentSelectedRecipe);
        break;
      default:
        int number = (key - '0');
        currentSelectedRecipe *= 10;
        currentSelectedRecipe += number;

        if (currentSelectedRecipe - 1 >= RECIPES_COUNT)
        {
          currentSelectedRecipe = 0;
          lcd.flashError();
        }

        lcd.showMenu(currentSelectedRecipe);

        break;
    }
  }
}
