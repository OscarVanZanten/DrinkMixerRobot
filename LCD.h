#ifndef LCD_h
#define LCD_h
#include <Arduino.h>
#include "Valve.h"

class LCD
{
private:
  LiquidCrystal *lcd;
  short errorDelay;

public:
  void flashError(int mode);
  void showStarting();
  void showPreparing();
  void showMenu(int number);
  void showOptionMenu();
  void showValveMenu(int value);
  void showRecipeMenu();
  void showSDCardError();
  void showEditValveMenu(int valve, int value, Valve valves[VALVES_COUNT]);
  LCD(LiquidCrystal *lcd, short errorDelay);
};

LCD::LCD(LiquidCrystal *lcd, short errorDelay)
{
  this->lcd = lcd;
  this->errorDelay = errorDelay;
}

void LCD::flashError(int mode)
{
  lcd->noBlink();
  lcd->clear();
  switch (mode)
  {
  case 0:
    lcd->print(INVALID_RECIPE_MESSAGE);
    break;
    
  case 1:
    lcd->print(VALVE_SELECT_INVALID_MESSAGE);
    break;
  }
  lcd->print(INVALID_RECIPE_MESSAGE);
  delay(errorDelay);
}

void LCD::showStarting()
{
  lcd->setCursor(0, 0);
  lcd->clear();
  lcd->print(STARTING_MESSAGE);
  lcd->blink();
}

void LCD::showPreparing()
{
  lcd->noBlink();
  lcd->clear();
  lcd->print(PREPARING_MESSAGE);
}

void LCD::showMenu(int number)
{
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(STANDARD_MESSAGE);
  lcd->setCursor(0, 1);
  if (number > 0)
  {
    lcd->print(number);
  }
  lcd->blink();
}

void LCD::showOptionMenu()
{
  lcd->noBlink();
  lcd->clear();
  lcd->print(MENU_HEAD_MESSAGE);
  lcd->setCursor(0, 1);
  lcd->print(MENU_OPTION_MESSAGE);
}

void LCD::showValveMenu(int value)
{
  lcd->clear();
  lcd->print(VALVE_SELECT_HEAD_MESSAGE);
  lcd->setCursor(0, 1);
  if (value > 0)
  {
    lcd->print(value);
  }
  lcd->blink();
}

void LCD::showEditValveMenu(int valve, int value, Valve valves[VALVES_COUNT])
{
  lcd->clear();
  lcd->print(VALVE_EDIT_HEAD_MESSAGE);
  lcd->print(valve);
  lcd->setCursor(0, 1);
  if (value > 0)
  {
    lcd->print(value);
  } else 
  {
     lcd->print(valves[valve-1].timing);
  }
  lcd->blink();
}

void LCD::showRecipeMenu()
{
  lcd->noBlink();
  lcd->clear();
  lcd->print(RECIPE_HEAD_MESSAGE);
  lcd->setCursor(0, 1);
  //lcd->print(MENU_OPTION_MESSAGE);
}

void LCD::showSDCardError()
{
  lcd->clear();
  lcd->print(INVALID_SD_CARD_MESSAGE);
  lcd->setCursor(0, 1);
  lcd->print(RESTART_MACHINE_MESSAGE);
  ///lcd->noBlink();
}

#endif
