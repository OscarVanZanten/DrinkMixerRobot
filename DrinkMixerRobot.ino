#include <Keypad.h>
#include <LiquidCrystal.h>
#include "Valve.h"
#include "DrinkMaker.h"
using byte = signed byte;

//////////PINS//////////
#define LCDPIN1 12
#define LCDPIN2 11
#define LCDPIN3 5
#define LCDPIN4 4
#define LCDPIN5 3
#define LCDPIN6 2

#define ROWPIN1 23
#define ROWPIN2 25
#define ROWPIN3 27
#define ROWPIN4 29
#define COLUMNPIN1 31
#define COLUMNPIN2 33
#define COLUMNPIN3 35

#define RELAY1 37
#define RELAY2 39
#define RELAY3 41
#define RELAY4 43
#define RELAY5 45
#define RELAY6 47
#define RELAY7 49
#define RELAY8 51
#define RELAY9 53
////////////////////////

///////////KEYPAD//////////////////////////////
const byte ROWS = 4;
const byte COLS = 3;
byte rowPins[ROWS] =
{
  ROWPIN1,
  ROWPIN2,
  ROWPIN3,
  ROWPIN4
};
byte colPins[COLS] =
{
  COLUMNPIN1,
  COLUMNPIN2,
  COLUMNPIN3
};
char keys[ROWS][COLS] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int currentSelectedRecipe = 0;

///////////////////////////////////////////////

/////////////////////RECIPES///////////////////
const byte VALVES = 9;
Valve valves[VALVES] =
{
  Valve(RELAY1, 100),
  Valve(RELAY2, 100),
  Valve(RELAY3, 500),
  Valve(RELAY4, 100),
  Valve(RELAY5, 100),
  Valve(RELAY6, 100),
  Valve(RELAY7, 100),
  Valve(RELAY8, 100),
  Valve(RELAY9, 100)
};

const short RECIPES_LENGTH = 4;
const short RECIPES_COUNT = 10;
short recipes[RECIPES_LENGTH * RECIPES_COUNT] = {
  0, 2, 1, 3,
  1, 3, 2, 3,
  3, 2, 3, 1,
  3, 3, 3, 1,
  0, 2, -1, 0,
  0, 2, 1, 3,
  1, 3, 2, 3,
  3, 2, 3, 1,
  3, 3, 3, 1,
  0, 2, -1, 0,
};
DrinkMaker maker = DrinkMaker(valves, RECIPES_LENGTH, recipes);
///////////////////////////////////////////////


/////////////////////LCD///////////////////////

LiquidCrystal lcd(LCDPIN1, LCDPIN2, LCDPIN3, LCDPIN4, LCDPIN5, LCDPIN6);
const short errorDelay = 1000;
const char* errorMessage = "INVALID RECIPE!";
const char* standardMessage = "Select recipe:";
///////////////////////////////////////////////

void flashError()
{
  lcd.noBlink();
  lcd.clear();
  lcd.print(errorMessage);
  delay(errorDelay);
}

void showMenu(int number)
{
  lcd.clear();
  lcd.print(standardMessage);
  lcd.setCursor(0, 1);
  if (number > 0) {
    lcd.print(number);
  }
  lcd.blink();
}

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  showMenu(0);
}


void loop()
{

  char key = kpd.getKey();

  if (key) // Check for a valid key.
  {
    switch (key)
    {
      case '#':
        if (currentSelectedRecipe - 1 >= RECIPES_COUNT ||  currentSelectedRecipe == 0)
        {
          flashError();
        } else
        {
          maker.MakeRecipe(currentSelectedRecipe - 1);
        }
        currentSelectedRecipe = 0;
        showMenu(currentSelectedRecipe);
        break;
      case '*':
        currentSelectedRecipe = 0;
        showMenu(currentSelectedRecipe);
        break;
      default:
        int number = (key - '0');
        currentSelectedRecipe *= 10;
        currentSelectedRecipe += number;

        if (currentSelectedRecipe - 1 >= RECIPES_COUNT)
        {
          currentSelectedRecipe = 0;
          flashError();
        }

        showMenu(currentSelectedRecipe);

        break;
    }
  }
}
