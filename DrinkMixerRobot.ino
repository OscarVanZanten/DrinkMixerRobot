#include "Constants.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "Storage.h"
#include "Valve.h"
#include "DrinkMaker.h"
#include "Recipe.h"
#include "LCD.h"

///////////KEYPAD//////////////////////////////
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, KEYPADROWS, KEYPADCOLS);
int selectedIndex = 0;

/////////////////////RECIPES///////////////////
Valve valves[VALVES_COUNT];
Recipe recipes[RECIPES_COUNT];
DrinkMaker maker = DrinkMaker(valves, recipes);

/////////////////////LCD///////////////////////
byte currentMenu = MENU_SELECT_RECIPE;
LiquidCrystal lcdDisplay(LCDPIN1, LCDPIN2, LCDPIN3, LCDPIN4, LCDPIN5, LCDPIN6);
LCD lcd(&lcdDisplay, ERROR_DELAY);

///////////////////SD-CARD////////////////////
Storage storage = Storage(&lcd, valves, recipes);

void setup()
{
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

  switch (currentMenu)
  {
  case MENU_SELECT_RECIPE:
    selectRecipe();
    break;
  case MENU_SELECT_MENU:
    selectMenu();
    break;
  case MENU_VALVE_SELECT:
    valveSelect();
    break;
  case MENU_VALVE_EDIT:
    valveEdit();
    break;
  case MENU_RECIPE_SUBMENU:
    break;
  case MENU_RECIPE_DELETE:
    break;
  case MENU_RECIPE_ADD:
    break;
  }
}

void selectRecipe()
{
  char key = kpd.getKey();

  if (key) // Check for a valid key.
  {
    switch (key)
    {
    case '#':
      if (selectedIndex - 1 >= RECIPES_COUNT || selectedIndex == 0)
      {
        lcd.flashError(0);
      }
      else
      {
        lcd.showPreparing();
        maker.MakeRecipe(selectedIndex - 1);
      }
      selectedIndex = 0;
      lcd.showMenu(selectedIndex);
      break;
    case '*':
      if (selectedIndex == 0)
      {
        lcd.showOptionMenu();
        currentMenu = MENU_SELECT_MENU;
        return;
      }
      selectedIndex = 0;
      lcd.showMenu(selectedIndex);
      break;
    default:
      int number = (key - '0');
      selectedIndex *= 10;
      selectedIndex += number;

      if (selectedIndex - 1 >= RECIPES_COUNT)
      {
        selectedIndex = 0;
        lcd.flashError(0);
      }

      lcd.showMenu(selectedIndex);
      break;
    }
  }
}

void selectMenu()
{
  char key = kpd.getKey();

  if (key) // Check for a valid key.
  {
    switch (key)
    {
    case '*':
      currentMenu = MENU_SELECT_RECIPE;
      lcd.showMenu(0);
      break;
    case '1':
      currentMenu = MENU_VALVE_SELECT;
      selectedIndex = 0;
      lcd.showValveMenu(selectedIndex);
      break;
    case '2':
      currentMenu = MENU_RECIPE_SUBMENU;
      lcd.showRecipeMenu();
      break;
    }
  }
}

void valveSelect()
{
  char key = kpd.getKey();

  if (key) // Check for a valid key.
  {
    switch (key)
    {
      case '#':
      if (selectedIndex - 1 >= VALVES_COUNT || selectedIndex == 0)
      {
        lcd.flashError(1);
      }
      else
      {
        currentMenu = MENU_VALVE_EDIT;
        lcd.showEditValveMenu(selectedIndex, -1, valves);
      }
      break;
    case '*':
      if (selectedIndex == 0)
      {
        lcd.showOptionMenu();
        currentMenu = MENU_SELECT_MENU;
        return;
      }
      selectedIndex = 0;
      lcd.showValveMenu(selectedIndex);
      break;
    default:
      int number = (key - '0');
      selectedIndex *= 10;
      selectedIndex += number;

      if (selectedIndex - 1 >= VALVES_COUNT)
      {
        selectedIndex = 0;
        lcd.flashError(1);
      }

      lcd.showValveMenu(selectedIndex);
      break;
    }
  }
}

void valveEdit()
{
  Serial.println("editing valve");
}

void recipeMenu()
{
}