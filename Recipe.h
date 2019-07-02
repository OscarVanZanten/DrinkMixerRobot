#ifndef Recipe_h
#define Recipe_h

class Recipe {
   public:
    short* recipe;
    short recipeLength;
   public:
    Recipe(short* recipe, short recipeLength);
};

Recipe::Recipe(short* recipe, short recipeLength)
{
  this->recipe = recipe;
  this->recipeLength = recipeLength;
}

#endif
