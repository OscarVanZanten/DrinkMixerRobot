#include <SD.h>
#include <SPI.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "Valve.h"
#include "DrinkMaker.h"
#include "Recipe.h"
using byte = signed byte;

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

const short RECIPES_COUNT = 3;
short recipe1[5] = {0, 3, 1, 2, 4};
short recipe2[3] = {0, 2, 3};
short recipe3[3] = {3, 2, 1};

Recipe recipes[ RECIPES_COUNT] = {
  Recipe(recipe1, 5),
  Recipe(recipe2, 3),
  Recipe(recipe3, 3)
};

DrinkMaker maker = DrinkMaker(valves, recipes);
///////////////////////////////////////////////


/////////////////////LCD///////////////////////

LiquidCrystal lcd(LCDPIN1, LCDPIN2, LCDPIN3, LCDPIN4, LCDPIN5, LCDPIN6);
const short errorDelay = 1000;
const char* standardMessage = "Select recipe:";
const char* invalidRecipeMessage = "INVALID RECIPE!";
const char* preparingMessage = "Preparing.....";
const char* invalidSDCardErrorMessage = "No SD card found";
const char* restartMachineMessage = "Insert and reset";
///////////////////////////////////////////////


///////////////////SD-CARD////////////////////
const char* valveFileName = "valves.txt";
const char* recipeFileName = "recipes.txt";
File valveFile;
File recipeFile;
bool loadedSuccesfull = true;
//////////////////////////////////////////////

void flashError()
{
  lcd.noBlink();
  lcd.clear();
  lcd.print(invalidRecipeMessage);
  delay(errorDelay);
}

void showPreparing()
{
  lcd.noBlink();
  lcd.clear();
  lcd.print(preparingMessage);
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

void showSDCardError()
{
  lcd.clear();
  lcd.print(invalidSDCardErrorMessage);
  lcd.setCursor(0, 1);
  lcd.print(restartMachineMessage);
  lcd.noBlink();
}

void setupSDCard() {
  if (!SD.begin())
  {
    Serial.println("SD card initialization failed");
    showSDCardError();
    loadedSuccesfull = false;
    return;
  }

  if (!SD.exists(valveFileName))
  {
    valveFile = SD.open(valveFileName, FILE_WRITE);
    valveFile.println("0: 100");
    valveFile.println("1: 300");
    valveFile.println("2: 500");
    valveFile.println("3: 200");
    valveFile.close();
  }
  if (!SD.exists(recipeFileName))
  {
    recipeFile = SD.open(recipeFileName, FILE_WRITE);
    recipeFile.println("0: 1,3,2,1,4,3,1,2,1");
    recipeFile.println("1: 1,3,2,3");
    recipeFile.println("2: 3,1,1,2,3");
    recipeFile.close();
  }
}

void loadRecipes()
{
  valveFile = SD.open(valveFileName);
  recipeFile = SD.open(recipeFileName);

  if (valveFile) {
    while (valveFile.available()) {
      Serial.write(valveFile.read());
    }
    valveFile.close();
  }

  if (recipeFile) {
    while (recipeFile.available()) {
      Serial.write(recipeFile.read());
    }
    recipeFile.close();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(SDCSPIN, OUTPUT);
  lcd.begin(16, 2);

  setupSDCard();
  loadRecipes();

  showMenu(0);
}


void loop()
{
  if (!loadedSuccesfull)
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
          flashError();
        } else
        {
          showPreparing();
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
