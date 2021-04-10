#   Generic @

@@@@@                                               RECIPE START
title                                               RECIPE TITLE
|                                                   VERTICAL BAR
description                                         FREE FORMAT
|                                                   VERTICAL BAR
ingredients                                         ONE or MORE
|                                                   VERTICAL BAR
directions                                          FREE FORMAT
_____   (or @@@@@)                                  RECIPE END

### DESCRIPTION:

@@@@@
    A unique character sequence that identifies the following
    as a Generically Formatted recipe.
title
    The recipe name.
|
    (MUST BE A VERTICAL BAR '|'.)
description
            Say something about the recipe.  (Free form.)
|
    (MUST BE A VERTICAL BAR '|'.)
ingredients
    Formatted as 'Amount', 'Unit of Measurement', 'Ingredient'
    and 'Preparation'.  Spaces separate the first three columns
    from each other.  A coma {,), or double dash (--) may be used
    to separate the "Ingredient' from 'Preparation'.
|
    (MUST BE A VERTICAL BAR '|'.)
directions
    This is essentially free formatted text.
_____
    A unique character sequence that identifies the end of a
    Generically Formatted recipe. (NOTE: May optionally be
    the same character sequence as the start '@@@@@'.)

### EXAMPLE:

@@@@@
Squash Casserole
|
NOTE: Say something nice about the recipe.
NOTE: Notice there is a blank line here.  That's OK.
NOTE: It is also OK if there isn't any text.  In that case there 
      would still be two vertical bars but no text between them.
|
2 pounds yellow squash
2 medium tomatoes, sliced
Salt and pepper, to taste
2 medium onions, sliced
1/2 cup Parmesan Cheese
1/4 cup Margarine, (1/2 stick)
|
Preheat oven to 350 degrees F. Peel and slice squash into
rounds about 1/2" thick. Place about 1/3 of the sliced squash
in the bottom of a baking dish, add a 1/3 of the tomatoes, salt
and pepper, 1/3 of the onions and 1/3 of the cheese. Dot with
1/3 of the margarine. Repeat with additional two layers. Cover
with foil and bake in preheated oven for 30 minutes. Remove the
foil and bake for an additional 10 minutes.

### NOTES:

*   Many descriptions of this format floating around the internet do NOT
require the set of closing brackets that end the recipe.  I have done my
best at following this 'standard' there is a significant problem with it.
When they are omitted, there is no way of knowing that any text following
the last recipe is part of the recipe directions or just something the
publisher added as a comment, note or any other random text.