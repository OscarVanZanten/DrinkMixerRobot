#ifndef DrinkMaker_h
#define DrinkMaker_h

#include "Arduino.h"
#include "Valve.h"

class DrinkMaker {
private:
 Valve* valves;
 short* recipes;
 short recipeLength;
 
public:
 DrinkMaker(Valve valves[], short recipeLength, short recipes[]);
 void MakeRecipe(short index);
};

DrinkMaker::DrinkMaker(Valve valves[], short recipeLength, short recipes[])
{
  this->valves = valves;
  this->recipes = recipes;
  this->recipeLength = recipeLength;
}

void DrinkMaker::MakeRecipe(short index)
{
  for (short i = 0; i < this->recipeLength; i++) {
    short valveIndex = this->recipes[index * this->recipeLength + i];
    
    if(valveIndex == -1)
    {
      break;
    }

    valves[valveIndex].Activate();
  }
}

#endif
