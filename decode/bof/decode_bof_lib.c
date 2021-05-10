/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_bof' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <ctype.h>              //  Determine the type contained
#include <string.h>             //  Functions for managing strings
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include "global.h"             //  Global stuff for this application
#include "libtools_api.h"       //  My Tools Library
                                //*******************************************
#include "tcb_api.h"            //  API for all tcb_*               PUBLIC
#include "rcb_api.h"            //  API for all rcb_*               PUBLIC
#include "recipe_api.h"         //  API for all recipe_*            PUBLIC
#include "xlate_api.h"          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include "decode_api.h"         //  API for all decode_*            PUBLIC
                                //*******************************************
#include "decode_api.h"         //  API for all decode_*            PUBLIC
#include "decode_bof_lib.h"     //  API for all DECODE_BOF__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Definitions local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Structures local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Storage Allocation local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/


/****************************************************************************/
/**
 *  Test the string for a recipe START marker.
 *      FORMATS:
 *          1   |-= Exported from BigOven =-|

 *
 *  @param  data_p              Pointer to a a line of text to be scanned.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__start (
    char                        *   data_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe start tag
    bof_rc = false;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );


    /************************************************************************
     *  Anything that starts with "-= Exported from BigOven =-"
     ************************************************************************/

    //  Is the a BOF start tag ?
    if (    ( start_p != NULL )                           //  Data is present
         && ( start_p[ 0 ] != '>' )                       //  Not forwarded e-mail
         && ( strncmp( start_p, BOF_START, BOF_START_L ) == 0 ) )
    {
        //  YES:    Change the return code
        bof_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( bof_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |** This recipe can be pasted into BigOven without retyping.|
 *
 *  @param  data_p              Pointer to a a line of text to be scanned.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__pre_end(
    char                        *   data_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    bof_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Big-Oven BOF recipe ?
    if ( strncmp( tmp_data_p, BOF_END_1, BOF_END_1_L  ) == 0 )
    {
        //  YES:    Change the return code
        bof_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( bof_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |** Easy recipe software.  Try it free at: http://www.bigoven.com    **|
 *
 *  @param  data_p              Pointer to a a line of text to be scanned.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__end(
    char                        *   data_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    bof_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Big-Oven BOF recipe ?
    if (    ( strncmp( tmp_data_p, BOF_END_1, BOF_END_1_L  ) == 0 )
         || ( strncmp( tmp_data_p, BOF_END_2, BOF_END_2_L  ) == 0 )
         || ( strncmp( tmp_data_p, BOF_END_3, BOF_END_3_L  ) == 0 ) )
    {
        //  YES:    Change the return code
        bof_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( bof_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe title.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  title_p             Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a the recipe title is located and
 *                              processed else false.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a title
    bof_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( title_p ) != true )
    {
        title_p = text_skip_past_whitespace( title_p );

        //  Cleanup the recipe name before saving it.
        decode_name_cleanup( title_p, strlen( title_p ) );

        //  Save the recipe title (name)
        recipe_p->name_p = text_copy_to_new( title_p );

        log_write( MID_DEBUG_1, "decode_bof_lib.c", "Line: %d\n", __LINE__ );

        // Change the pass_fail flag to PASS
        bof_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( bof_rc );
}

/****************************************************************************/
/**
 *  Copy 'recipe by' information to the recipe structure.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return bof_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__recipe_by(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'recipe by' information.
    bof_rc = false;

    /************************************************************************
     *  Recipe By:
     ************************************************************************/

    //  Is there something already there ?
    if ( recipe_p->author_p == NULL )
    {
        //  YES:    Search the data buffer
        tmp_data_p = data_p + BOF_BY_L;

        //  Locate the first character in the buffer
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe title (name)
        recipe_p->author_p = text_copy_to_new( tmp_data_p );

        log_write( MID_DEBUG_1, "decode_bof_lib.c", "Line: %d\n", __LINE__ );
    }
    else
    {
        //  NO:     OOPS.
        log_write( MID_WARNING, "BOF_DECODE__",  "\tA second 'Recipe By:' "
                   "was detected.\n" );
    }

    //  Change the return code
    bof_rc = true;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( bof_rc );
}

/****************************************************************************/
/**
 *  Copy 'serving size' information to the recipe structure.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return bof_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__srv_size(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'serving size' information.
    bof_rc = false;

    /************************************************************************
     *  Serving Size  :
     ************************************************************************/

    //  Is there something already there ?
    if ( recipe_p->serves_p == NULL )
    {
        //  YES:    Jump past the search string
        tmp_data_p = data_p + BOF_SERVING_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe title (name)
        recipe_p->serves_p = text_copy_to_new( tmp_data_p );

        log_write( MID_DEBUG_1, "decode_bof_lib.c", "Line: %d\n", __LINE__ );
    }
    else
    {
        //  NO:     OOPS.
        log_write( MID_WARNING, "BOF_DECODE__",  "\tA second 'Serving Size:' "
                   "was detected.\n" );
    }



    //  Change the return code
    bof_rc = true;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( bof_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe categories.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  categories_p        Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a the recipe title is located and
 *                              processed else false.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__categories(
    struct  recipe_t            *   recipe_p,
    char                        *   categories_p
    )
{
    /**
     *  @param bof_rc           Return Code                                 */
    int                             bof_rc;
    /**
     *  @param  raw_chapter     A temporary holding buffer for a chapter    */
    char                            raw_category[ MAX_LINE_L ];
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;
    /**
     * @param found_category_p  Pointer to a temp data buffer               */
    char                        *   found_category_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    bof_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Many decodes got us here so there isn't a fixed size to skip.
    //  Find the semi colen
    tmp_data_p = strchr( categories_p, ':' );

    //  Should always be true but just in case; Did we find one ?
    if ( tmp_data_p != NULL )
    {
        //  YES:    Advance the pointer past it
        tmp_data_p += 1;
    }

    //  Skip past any leading whitespace.
    tmp_data_p = text_skip_past_whitespace( tmp_data_p );

    //  Should be pointing to the start of a chapter or
    //  the end of the line.
    while ( tmp_data_p[ 0 ] != '\0' )
    {
        //  Wipe the temporary chapter buffer clean
        memset( &(raw_category[ 0 ]),
                '\0',
                sizeof ( raw_category ) );

        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Copy the new chapter to the temp buffer
        for (  ;
              (    ( tmp_data_p[ 0 ] != ',' )
                && ( tmp_data_p[ 0 ] != '\0' ) );
              tmp_data_p ++ )
        {
            strncat( raw_category, tmp_data_p, 1 );
        }
        //  Skip past the ','
        tmp_data_p += 1;

        //  Translate the chapter
        found_category_p = xlate_chapter( raw_category );

        //  Do we have a chapter to save ?
        if ( found_category_p != NULL )
        {
            //  YES:    Save it
            decode_save_chapter( found_category_p, recipe_p );
        }
    }
    //  Change the pass_fail flag to PASS
    bof_rc = true;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( bof_rc );
}

/****************************************************************************/
/**
 *  Everything from here to the next blank line is AUIP.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  in_buffer_p         Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a new recipe is detected
 *                              else FRC_FAIL.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT an AUIP
    bof_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Process the first half (or the entire line)
        in_buffer_p = text_skip_past_whitespace( in_buffer_p );

        // Format the AUIP line
        decode_fmt_auip( recipe_p, in_buffer_p, RECIPE_FORMAT_BOF );

        //  Set the return code
        bof_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( bof_rc );
}

/****************************************************************************/
/**
 *  Everything from here to END-OF-RECIPE is 'DIRECTIONS'.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  in_buffer_p         Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a new recipe is detected
 *                              else FRC_FAIL.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_BOF__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    bof_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this the end-of-recipe tag ?
    if ( DECODE_BOF__end( in_buffer_p ) == false )
    {
        //  Format the AUIP line
        decode_add_instructions( recipe_p, in_buffer_p );

        //  This is a directions line of text
        bof_rc = true;
    }
    else
    {
        //  YES:    NOT a directions line
        bof_rc = false;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( bof_rc );
}

/****************************************************************************/
