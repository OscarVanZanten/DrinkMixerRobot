#ifndef Storage_h
#define Storage_h

#include "Valve.h"
#include "Recipe.h"
#include "LCD.h"
#include <SD.h>

class Storage {
   private:
    LCD* lcd;
    File valveFile;
    File recipeFile;
    Valve* valves;
    Recipe* recipes;
    
   public:
    bool loadedSuccesfull = true;
     
   public:
    void setupSDCard();
    void loadValves();
    void loadRecipes();
    Storage(LCD* lcd, Valve valves[9], Recipe recipes[256]);
};

Storage::Storage(LCD* lcd, Valve valves[9], Recipe recipes[256])
{
  this->lcd = lcd; 
  this->valves = valves;
  this->recipes = recipes;
}

void Storage::setupSDCard() {
  if (!SD.begin())
  {
    Serial.println("SD card initialization failed");
    lcd->showSDCardError();
    loadedSuccesfull = false;
    return;
  }

  if (!SD.exists(VALVE_FILE_NAME))
  {
    valveFile = SD.open(VALVE_FILE_NAME, FILE_WRITE);
    valveFile.println("100");
    valveFile.println("300");
    valveFile.println("500");
    valveFile.println("200");
    valveFile.close();
  }
  if (!SD.exists(RECIPE_FILE_NAME))
  {
    recipeFile = SD.open(RECIPE_FILE_NAME, FILE_WRITE);
    recipeFile.println("1,3,2,1,4,3,1,2,1");
    recipeFile.println("1,3,2,3");
    recipeFile.println("3,1,1,2,3");
    recipeFile.close();
  }
}

void Storage::loadValves()
{
  valveFile = SD.open(VALVE_FILE_NAME);
  int valveDelay = 0;
  int valveIndex = 0;

  if (valveFile) {
    while (valveFile.available()) {
      char c = valveFile.read();
      int number = (c - '0');

      if (c == '\n') {
        Serial.println(valveDelay);

        if (valveIndex < VALVES_COUNT) {
          valves[valveIndex] = Valve(VALVE_PINS[valveIndex], valveDelay);
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

void Storage::loadRecipes()
{
  recipeFile = SD.open(RECIPE_FILE_NAME);
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

#endif
