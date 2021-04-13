/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef RECIPE_LIB_H
#define RECIPE_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal
 *  library components of the 'recipe' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_RECIPE
   #define RECIPE_EXT
#else
   #define RECIPE_EXT         extern
#endif

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
 * Library Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
char  *
RECIPE__fmt_amount(
    char                    *   in_auip_p,
    char                    *   amount_p,
    int                         out_buf_size
    );
//----------------------------------------------------------------------------
char  *
RECIPE__fmt_unit(
    char                    *   in_auip_p,
    char                    *   unit_p,
    int                         out_buf_size
    );
//----------------------------------------------------------------------------
char  *
RECIPE__fmt_ingredient(
    char                    *   in_ingredient_p,
    char                    *   ingredient_p,
    int                         mmf_length,
    int                         out_buf_size
    );
//----------------------------------------------------------------------------
char  *
RECIPE__fmt_preparation(
    char                    *   in_preparation_p,
    char                    *   preparation_p,
    int                         mmf_length,
    int                         out_buf_size
    );
//----------------------------------------------------------------------------
void
RECIPE__new_auip(
    struct   recipe_t       *   recipe_p,
    char                    *   amount_p,
    char                    *   unit_p,
    char                    *   ingredient_p,
    char                    *   preparation_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    RECIPE_LIB_H