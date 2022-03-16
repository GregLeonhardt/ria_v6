/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef RECIPE_API_H
#define RECIPE_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external
 *  library components of the 'recipe' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define SIZE_AMOUNT             (   8 )
#define SIZE_UNIT               (  32 )
#define SIZE_INGREDIENT         ( 256 )
#define SIZE_PREPARATION        ( 256 )
#define SIZE_CATEGORY           (  32 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    recipe_format_e
{
    RECIPE_FORMAT_NONE          =   0,      //  RESERVED
    RECIPE_FORMAT_BOF           =   1,      //  Big Oven Format
    RECIPE_FORMAT_CP2           =   2,      //  Cooken Pro 2.0
    RECIPE_FORMAT_ERD           =   3,      //  Easy Recipe Deluxe
    RECIPE_FORMAT_GRF           =   4,      //  Generic Format #1     [[[[[
    RECIPE_FORMAT_GF2           =   5,      //  Generic Format #2     @@@@@
    RECIPE_FORMAT_MMF           =   6,      //  MealMaster
    RECIPE_FORMAT_MXP           =   7,      //  MasterCook eXport
    RECIPE_FORMAT_MX2           =   8,      //  MasterCook XML
    RECIPE_FORMAT_NYC           =   9,      //  Now You're Cooking!
    RECIPE_FORMAT_RXF           =  10,      //  Recipe eXchange Format (Native)
    RECIPE_FORMAT_TXT           =  99       //  Unformatted text data
};
//----------------------------------------------------------------------------
enum    categories_state_e
{
    CSS_IDLE                    =   0,
    CSS_STARTED                 =   1,
    CSS_COMPLETE                =   2
};
//----------------------------------------------------------------------------
enum    auip_state_e
{
    AUIPS_IDLE                  =   0,
    AUIPS_TEXT                  =   1,
    AUIPS_DASH                  =   2,
    AUIPS_AMIP                  =   3,
    AUIPS_COMPLETE              =   4
};
//----------------------------------------------------------------------------
enum    direction_state_e
{
    DSS_IDLE                    =   0,
    DSS_STARTED                 =   1,
    DSS_COMPLETE                =   2
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param recipe_t             Primary structure for a recipe
 *  @NOTE:  Changes to this structure require changes to recipe_kill( );    */
struct   recipe_t
{
    /**
     *  @param  rcb_p           Pointer to a Recipe Control Block           */
    struct  rcb_t           *   rcb_p;
    /**
     *  @param  name            The recipe name or title                    */
    char                    *   name_p;
    /**
     *  @param  recipe_id       A unique identifier for the recipe          */
    char                    *   recipe_id_p;
    /**
     *  @param  web_address     The web address the file came from          */
    char                    *   web_address_p;
    /**
     *  @param  source_format   Encoding format from the source recipe      */
    char                    *   source_format_p;
    /**
     *  @param  author          The person or company that created          */
    char                    *   author_p;
    /**
     *  @param  copyright       (c) XyZ                                     */
    char                    *   copyright_p;
    /**
     *  @param  edited_by       Who last edited or changed                  */
    char                    *   edited_by_p;
    /**
     *  @param  formatted by    Who originally formatted                    */
    char                    *   formatted_by_p;
    /**
     *  @param  import_from     Where the recipe was imported from          */
    char                    *   import_from_p;
    /**
     *  @param  serves          Number of people                            */
    char                    *   serves_p;
    /**
     *  @param  serving_size    How much each person is served              */
    char                    *   servings_size_p;
    /**
     *  @param  makes           How many of something the recipe will make  */
    char                    *   makes_p;
    /**
     *  @param  makes_unit      Unit of measurement for 'makes'             */
    char                    *   makes_unit_p;
    /**
     *  @param  time_prep       How long is should take to get ready        */
    char                    *   time_prep_p;
    /**
     *  @param  time_wait       After preparation and before cooking        */
    char                    *   time_wait_p;
    /**
     *  @param  time_cook       How long it should take to cook             */
    char                    *   time_cook_p;
    /**
     *  @param  time_rest       After cooked before serving                 */
    char                    *   time_rest_p;
    /**
     *  @param  time_total      All the times added up.                     */
    char                    *   time_total_p;
    /**
     *  @param  serves          A short description (1024 MAX.)             */
    char                    *   description_p;
    /**
     *  @param  based_on        Another recipe this one is based on         */
    char                    *   based_on_p;
    /**
     *  @param  serve_with      Something to serve with                     */
    char                    *   serve_with_p;
    /**
     *  @param  wine            Sone wines that would go well               */
    char                    *   wine_p;
    /**
     *  @param  rating          How good (0-9)                              */
    char                    *   rating_p;
    /**
     *  @param  skill           Skill needed to make (0-9)                  */
    char                    *   skill_p;
    /**
     *  @param  instructions    A single string of the "directions:         */
    char                    *   instructions_p;
    /**
     *  @param  source          Where the recipe came from                  */
    char                    *   source_p;
    /**
     *  @param  Appliance       List of appliances needed to make           */
    struct  list_base_t     *   appliance_p;
    /**
     *  @param  Cuisine         List of Cuisine this recipe fits            */
    struct  list_base_t     *   cuisine_p;
    /**
     *  @param  Occasion        List of occasions to use this recipe        */
    struct  list_base_t     *   occasion_p;
    /**
     *  @param  Course          List of courses to use this recipe          */
    struct  list_base_t     *   course_p;
    /**
     *  @param  Diet            List of diet(s) to use this recipe          */
    struct  list_base_t     *   diet_p;
    /**
     *  @param  Chapter         List of Chapters for this recipe            */
    struct  list_base_t     *   chapter_p;
    /**
     *  @param  ingredient      List of ingredients                         */
    struct  list_base_t     *   ingredient_p;
    /**
     *  @param  directions      List of directions to make                  */
    struct  list_base_t     *   directions_p;
    /**
     *  @param  notes           Additional notes or modification options    */
    struct  list_base_t     *   notes_p;
    //------------------------------------------------------------------------
    //  Used by the decoders
    //------------------------------------------------------------------------
    /**
     * @param blank_line        The previous input line has no data         */
    int                         blank_line;
    /**
     * @param first_auip        We have located the first AUIP line         */
    int                         first_auip;
    /**
     *  @param  categories_scan_state                                       */
    enum    categories_state_e  categories_scan_state;
    /**
     *  @param  auip_scan_state                                             */
    enum    auip_state_e        auip_scan_state;
    /**
     *  @param  direction_scan_state                                        */
    enum    direction_state_e   direction_scan_state;
};
//----------------------------------------------------------------------------
struct  auip_t
{
    char                        *   type_p;         //  I=Ingredient
                                                    //  T=Text
                                                    //  R=Recipe
                                                    //  S=Subtitle
    char                        *   amount_p;
    char                        *   unit_p;
    char                        *   ingredient_p;
    char                        *   preparation_p;
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Storage Allocation
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
struct   recipe_t   *
recipe_new(
    struct  rcb_t           *   rcb_p,
    enum    recipe_format_e     recipe_format
    );
//---------------------------------------------------------------------------
void
recipe_kill(
    struct   recipe_t       *   recipe_p
    );
//---------------------------------------------------------------------------
enum    recipe_format_e
recipe_is_start(
    char                    *   data_p
    );
int
//---------------------------------------------------------------------------
recipe_is_end(
    enum    recipe_format_e     recipe_format,
    char                    *   data_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    RECIPE_API_H
