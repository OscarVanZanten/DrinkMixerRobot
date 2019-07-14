#ifndef Recipe_h
#define Recipe_h

class Recipe {
   public:
    short* recipe;
    short recipeLength;
    bool enabled;
     
   public:
    Recipe();
    Recipe(short* recipe, short recipeLength);
};

Recipe::Recipe()
{
  this->enabled = false;
}

Recipe::Recipe(short* recipe, short recipeLength)
{
  this->recipe = recipe;
  this->recipeLength = recipeLength;
  this->enabled = true;
}

#endif
