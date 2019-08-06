#ifndef LCD_h
#define LCD_h
#include <Arduino.h>

class LCD {
  private:
    LiquidCrystal* lcd;
    short errorDelay;
    const char* startingMessage ;
    const char* standardMessage;
    const char* invalidRecipeMessage ;
    const char* preparingMessage ;
    const char* invalidSDCardErrorMessage;
    const char* restartMachineMessage ;

  public:
    void flashError();
    void showStarting();
    void showPreparing();
    void showMenu(int number);
    //void showOptionMenu();
    void showSDCardError();
    LCD(LiquidCrystal* lcd, short errorDelay, const char* startingMessage, const char* standardMessage, const char* invalidRecipeMessage, const char* preparingMessage, const char* invalidSDCardErrorMessage, const char* restartMachineMessage);
};


LCD::LCD(LiquidCrystal* lcd, short errorDelay, const char* startingMessage, const char* standardMessage, const char* invalidRecipeMessage, const char* preparingMessage, const char* invalidSDCardErrorMessage, const char* restartMachineMessage)
{
  this->lcd = lcd;
  this->errorDelay = errorDelay;
  this->startingMessage = startingMessage;
  this->standardMessage = standardMessage;
  this->invalidRecipeMessage = invalidRecipeMessage;
  this->preparingMessage = preparingMessage;
  this->invalidSDCardErrorMessage = invalidSDCardErrorMessage;
  this->restartMachineMessage = restartMachineMessage;
}

void LCD::flashError()
{
  lcd->noBlink();
  lcd->clear();
  lcd->print(invalidRecipeMessage);
  delay(errorDelay);
}

void LCD::showStarting()
{
  lcd->setCursor(0, 0);
  lcd->clear();
  lcd->print(startingMessage);
  lcd->blink();
}

void LCD::showPreparing()
{
  lcd->noBlink();
  lcd->clear();
  lcd->print(preparingMessage);
}

void LCD::showMenu(int number)
{
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(standardMessage);
  lcd->setCursor(0, 1);
  if (number > 0) {
    lcd->print(number);
  }
  lcd->blink();
}

void LCD::showSDCardError()
{
  lcd->clear();
  lcd->print(invalidSDCardErrorMessage);
  lcd->setCursor(0, 1);
  lcd->print(restartMachineMessage);
  lcd->noBlink();
}

#endif
