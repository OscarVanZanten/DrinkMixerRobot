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
const byte VALVES_COUNT = 9;
byte valvePins[VALVES_COUNT] = {RELAY1, RELAY2, RELAY3, RELAY4, RELAY5, RELAY6, RELAY7, RELAY8, RELAY9};
Valve valves[VALVES_COUNT];

const short RECIPES_COUNT = 256;
const short RECIPES_LENGTH = 32;
Recipe recipes[ RECIPES_COUNT];

DrinkMaker maker = DrinkMaker(valves, recipes);
///////////////////////////////////////////////


/////////////////////LCD///////////////////////
LiquidCrystal lcd(LCDPIN1, LCDPIN2, LCDPIN3, LCDPIN4, LCDPIN5, LCDPIN6);

const short errorDelay = 1000;

const char* startingMessage = "Starting...";
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

void showStarting()
{
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print(startingMessage);
  lcd.blink();
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
  lcd.setCursor(0, 0);
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
    valveFile.println("100");
    valveFile.println("300");
    valveFile.println("500");
    valveFile.println("200");
    valveFile.close();
  }
  if (!SD.exists(recipeFileName))
  {
    recipeFile = SD.open(recipeFileName, FILE_WRITE);
    recipeFile.println("1,3,2,1,4,3,1,2,1");
    recipeFile.println("1,3,2,3");
    recipeFile.println("3,1,1,2,3");
    recipeFile.close();
  }
}

void loadValves()
{
  valveFile = SD.open(valveFileName);
  int valveDelay = 0;
  int valveIndex = 0;

  if (valveFile) {
    while (valveFile.available()) {
      char c = valveFile.read();
      int number = (c - '0');

      if (c == '\n') {
        Serial.println(valveDelay);

        if (valveIndex < VALVES_COUNT) {
          valves[valveIndex] = Valve(valvePins[valveIndex], valveDelay);
          valveDelay = 0;
          valveIndex++;
        } else
        {
          break;
        }
      }
      else if ( number  <= 9 || number >= 0)
      {
        valveDelay *= 10;
        valveDelay += number;
      }

    }
    valveFile.close();
  }
}

void loadRecipes()
{
  recipeFile = SD.open(recipeFileName);
  short recipeCount = 0;
  short* recipe =  new short[RECIPES_LENGTH];
  short recipeLength = 0;
  short valveSelected = 0;

  if (recipeFile) {
    while (recipeFile.available()) {
      char c = recipeFile.read();
      Serial.write(c);
      int number = (c - '0');

      if (c == '\n')
      {
        recipe[recipeLength] = valveSelected;
        Recipe r = Recipe(recipe, recipeLength + 1);
        recipes[recipeCount] = r;
        recipeCount++;
        recipe =  new short[RECIPES_LENGTH];
        valveSelected = 0;
        recipeLength = 0;

      }
      else if (c == ',')
      {
        recipe[recipeLength] = valveSelected;
        recipeLength++;
        valveSelected = 0;
      }
      else if ( number  <= 9 || number >= 0)
      {
        valveSelected *= 10;
        valveSelected += number;
      }
    }
    recipeFile.close();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(SDCSPIN, OUTPUT);

  lcd.begin(16, 2);

  showStarting();

  setupSDCard();
  loadValves();
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
