#ifndef DrinkMaker_h
#define DrinkMaker_h

#include "Arduino.h"
#include "Valve.h"
#include "Recipe.h"

class DrinkMaker {
  private:
    Valve* valves;
    Recipe* recipes;

  public:
    DrinkMaker(Valve valves[], Recipe recipes[]);
    void MakeRecipe(short index);
};

DrinkMaker::DrinkMaker(Valve valves[], Recipe recipes[])
{
  this->valves = valves;
  this->recipes = recipes;
}

void DrinkMaker::MakeRecipe(short index)
{
  Recipe recipe = recipes[index];

  if (!recipe.enabled)
  {
    return;
  }

  for (short i = 0; i < recipe.recipeLength; i++) {
    short valveIndex = recipe.recipe[i];

    if (valveIndex == -1)
    {
      break;
    }

    valves[valveIndex].Activate();
  }
}

#endif
