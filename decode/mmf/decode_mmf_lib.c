/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'mmf' library.
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
#include "decode_mmf_api.h"     //  API for all decode_mmf_*        PUBLIC
#include "decode_mmf_lib.h"     //  API for all DECODE_MMF__*       PRIVATE
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
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |MMMMM|
 *          2   |-----|
 *          3   |- -----|
 *      NOTE:
 *          When new end of recipe markers are added here, they should also
 *          be added to rd_input_file:rd_preprocess( )
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
MMF__is_end(
    char                    *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                         mmf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                    *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    mmf_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Meal-Master MMF recipe ?
    if (    (    ( strncasecmp( tmp_data_p, MMF_END_1,  MMF_END_1_L  ) == 0 )
              && ( strlen( tmp_data_p ) == MMF_END_1_L ) )
         || (    ( strncasecmp( tmp_data_p, MMF_END_2,  MMF_END_2_L  ) == 0 )
              && ( strlen( tmp_data_p ) == MMF_END_2_L ) )
         || (    ( strncasecmp( tmp_data_p, MMF_END_3,  MMF_END_3_L  ) == 0 )
              && ( strlen( tmp_data_p ) == MMF_END_3_L ) )
         || (    ( strncasecmp( tmp_data_p, MMF_END_4,  MMF_END_4_L  ) == 0 )
              && ( strlen( tmp_data_p ) == MMF_END_4_L ) )
         || (    ( strncasecmp( tmp_data_p, MMF_END_5,  MMF_END_5_L  ) == 0 )
              && ( strlen( tmp_data_p ) == MMF_END_5_L ) ) )
    {
        //  YES:    Change the return code
        mmf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( mmf_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe START marker.
 *      FORMATS:
 *          1   |Recipe via Meal-Master|
 *          2   |----- Now You're Cooking!|
 *          3   |MMMMM----- Now You're Cooking|
 *          4   |- ----- Recipe via Meal-Master|
 *          5   |MMMM----- Recipe via Meal-Master|
 *          6   |--------- Recipe via Meal-Master|
 *          7   |MMMMM----- Recipe via Meal-Master|
 *          8   |---------- Recipe via Meal-Master|
 *          9   |:MMMMM----- Recipe via Meal-Master|
 *         10   |----------- Recipe via Meal-Master|
 *         11   |----------- Recipe via Meal-Master|
 *         12   |-- MMMMM----- Recipe via Meal-Master|
 *         13   |------------- Recipe Extracted from Meal-Master|

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
MMF__is_start (
    char                    *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                         mmf_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                    *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    mmf_rc = false;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );


    /************************************************************************
     *  Look for a generic recipe start message for Meal-Master format
     ************************************************************************/

    //  Is the a CP2 start tag ?
    if (    ( start_p != NULL )                           //  Data is present
         && ( start_p[ 0 ] != '>' ) )                     //  Not forwarded e-mail
    {
        //  Does the string start with one of out line prefixes?
        if (    ( strncmp( start_p, MMF_PREFIX_1, MMF_PREFIX_1_L ) == 0 )    //  -----
             || ( strncmp( start_p, MMF_PREFIX_2, MMF_PREFIX_2_L ) == 0 )    //  MMMMM
             || ( strncmp( start_p, MMF_PREFIX_3, MMF_PREFIX_3_L ) == 0 ) )  //  - -----
        {
            //  YES:    Does it contain contain a software tag ?
            if (    ( strstr( start_p, MMF_MMF_1 ) != NULL )     //  Meal-Master
                 || ( strstr( start_p, MMF_NYC_1 ) != NULL ) )   //  Now You're Cooking!
            {
                //  YES:    Change the return code
                mmf_rc = true;
            }
        }

        /********************************************************************
         *  Look for other more specialized start messages
         ********************************************************************/

        //  Skip this test if a previous test was TRUE
        if ( mmf_rc == false )
        {
            //  Is this the start of a Meal-Master MMF recipe ?
            if (    ( strncmp( start_p, MMF_START_1,  MMF_START_1_L  ) == 0 )
                 || ( strncmp( start_p, MMF_START_2,  MMF_START_2_L  ) == 0 )
                 || ( strncmp( start_p, MMF_START_3,  MMF_START_3_L  ) == 0 )
                 || ( strncmp( start_p, MMF_START_4,  MMF_START_4_L  ) == 0 )
                 || ( strncmp( start_p, MMF_START_5,  MMF_START_5_L  ) == 0 )
                 || ( strncmp( start_p, MMF_START_6,  MMF_START_6_L  ) == 0 )
                 || ( strncmp( start_p, MMF_START_7,  MMF_START_7_L  ) == 0 )
                 || ( strncmp( start_p, MMF_START_8,  MMF_START_8_L  ) == 0 ) )
            {
                //  YES:    Change the return code
                mmf_rc = true;
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( mmf_rc );
}

/****************************************************************************/
/**
 *  Test for an AUIP split marker.
 *      FORMATS:
 *          1   |MMMMM|
 *          2   |-----|
 *          3   |- -----|
 *      NOTE:
 *          When new end of recipe markers are added here, they should also
 *          be added to rd_input_file:rd_preprocess( )
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
MMF__split(
    char                        *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mmf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    mmf_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid AUIP split string
     ************************************************************************/

    //  Is this the start of a Meal-Master MMF recipe ?
    if (    (    ( strncmp( tmp_data_p, MMF_END_1,  MMF_END_1_L  ) == 0 )
              && ( strlen( tmp_data_p ) > MMF_END_1_L ) )
         || (    ( strncmp( tmp_data_p, MMF_END_2,  MMF_END_2_L  ) == 0 )
              && ( strlen( tmp_data_p ) > MMF_END_2_L ) )
         || (    ( strncmp( tmp_data_p, MMF_END_3,  MMF_END_3_L  ) == 0 )
              && ( strlen( tmp_data_p ) > MMF_END_3_L ) ) )
    {
        //  YES:    Change the return code
        mmf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( mmf_rc );
}

/****************************************************************************/
/**
 *  Format an AUIP split line
 *
 *  @param  data_p              Pointer to a a line of text to be scanned.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

char *
MMF__fmt_split(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;
    /**
     * @param data_l            Size of the known data buffer               */
    int                             data_l;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Get the size of the data buffer
    data_l = strlen( data_p );

    //  Locate the first character in the buffer
    data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Remove leading 'MMMMM'
     ************************************************************************/

    //  Is the first character in the buffer a 'M' ?
    while ( data_p[ 0 ] == 'M' )
    {
        //  YES:    Remove it
        text_remove( data_p, 0, 1 );
    }

    /************************************************************************
     *  Remove leading non-alpha-numeric characters
     ************************************************************************/

    //  Is the first character in the buffer an alpha or numeric character ?
    while ( isalnum( data_p[ 0 ] ) == 0 )
    {
        //  NO:     Remove it
        text_remove( data_p, 0, 1 );

        //  Is there anything still in the buffer ?
        if ( text_is_blank_line( data_p ) == true )
        {
            //  NO:     We're done here
            break;
        }
    }

    /************************************************************************
     *  Skip over the text
     ************************************************************************/

    //  Is there anything still in the buffer ?
    if ( text_is_blank_line( data_p ) != true )
    {
        //  YES:    Get a temporary pointer to the string
        tmp_data_p = data_p;

        //  Is the first character in the buffer a 'M' ?
        while (    (    ( isalnum( tmp_data_p[ 0 ] ) != 0 )
                     || ( isspace( tmp_data_p[ 0 ] ) != 0 ) )
                || (    ( isalnum( tmp_data_p[ 0 ] ) == 0 )
                     && ( isalnum( tmp_data_p[ 1 ] ) != 0 ) ) )
        {
            //  YES:    Skip over it
            tmp_data_p += 1;
        }
    }

    /************************************************************************
     *  Remove all trailing characters
     ************************************************************************/

    //  Is there anything still in the buffer ?
    if ( text_is_blank_line( data_p ) != true )
    {
        //  YES:    Is the last character in the buffer ?
        while ( tmp_data_p[ 0 ] != '\0' )
        {
            //  YES:    Remove it
            text_remove( tmp_data_p, 0, 1 );
        }
    }

    /************************************************************************
     *  Format the string
     ************************************************************************/

    //  Is there anything still in the buffer ?
    if ( text_is_blank_line( data_p ) != true )
    {
        // YES: Put a '-' in the beginning of the string
        text_insert( data_p, data_l, 0, (char*)"** " );

        //  Append a colon to the end of the string
        if ( strlen( data_p ) < ( data_l + 3 ) )
        {
            tmp_data_p[ 3 ] = ' ';
            tmp_data_p[ 4 ] = '*';
            tmp_data_p[ 5 ] = '*';
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( data_p );
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
MMF__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mmf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( title_p ) != true )
    {
        title_p = text_skip_past_whitespace( title_p );

        //  Look for a TITLE tag in the line
        if ( strncmp( title_p, "Title:", 6 ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            title_p += 6;

            title_p = text_skip_past_whitespace( title_p );

            //  Cleanup the recipe name before saving it.
            recipe_name_cleanup( title_p, strlen( title_p ) );

            //  Save the recipe title (name)
            recipe_p->name = text_copy_to_new( title_p );

            log_write( MID_DEBUG_1, "mmf_lib.c", "Line: %d\n", __LINE__ );

            // Change the pass_fail flag to PASS
            mmf_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( mmf_rc );
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
MMF__categories(
    struct  recipe_t            *   recipe_p,
    char                        *   categories_p
    )
{
    /**
     *  @param mmf_rc           Return Code                                 */
    int                             mmf_rc;
    /**
     *  @param  raw_chapter     A temporary holding buffer for a chapter    */
    char                            raw_chapter[ MAX_LINE_L ];
    /**
     *  @param  tmp_p           A temporary data buffer pointer             */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    mmf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( categories_p ) != true )
    {
        categories_p = text_skip_past_whitespace( categories_p );

        //  Look for a TITLE tag in the line
        if ( strncmp( categories_p, "Categories:", 11 ) == 0 )
        {
            //  This is the title!  Move the pointer past the categories tag.
            categories_p += 11;

            //  Should be pointing to the start of a chapter or
            //  the end of the line.
            while ( categories_p[ 0 ] != '\0' )
            {
                //  Wipe the temporary chapter buffer clean
                memset( &raw_chapter,
                        '\0',
                        sizeof ( raw_chapter ) );

                categories_p = text_skip_past_whitespace( categories_p );

                //  Copy the new chapter to the temp buffer
                for (  ;
                      (    ( categories_p[ 0 ] != ',' )
                        && ( categories_p[ 0 ] != '\0' ) );
                      categories_p ++ )
                {
                    strncat( raw_chapter, categories_p, 1 );
                }
                //  Skip past the ','
                categories_p += 1;

                //  Translate the chapter
                tmp_p = xlate_chapter( raw_chapter );

                //  Do we have a chapter to save ?
                if ( tmp_p != NULL )
                {
                    //  YES:    Save it
                    decode_save_chapter( tmp_p, recipe_p );
                }
            }
            //  Change the pass_fail flag to PASS
            mmf_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( mmf_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe yield.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  yield_p             Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a the recipe title is located and
 *                              processed else false.
 *
 *  @note
 *
 ****************************************************************************/

int
MMF__yield(
    struct  recipe_t            *   recipe_p,
    char                        *   yield_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mmf_rc;
    // Local buffer for AMOUNT
    char                            local_amount[ MAX_LINE_L ];

    // Local buffer for UNIT
    char                            local_unit[ MAX_LINE_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    // Wipe clean the local buffers
    memset( local_amount,
            '\0',
            sizeof ( local_amount ) );
    memset( local_unit,
            '\0',
            sizeof ( local_unit ) );

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( yield_p ) != true )
    {
        yield_p = text_skip_past_whitespace( yield_p );

        //  Test this line for Meal-Master Servings
        if (    ( strncmp( yield_p, "Servings:", 9 ) == 0 )
             || ( strncmp( yield_p, "Yield:",    6 ) == 0 ) )
        {
            //  Skip past the marker
            for (;
                  ( ( isspace( yield_p[ 0 ] ) == 0 )
                    && ( yield_p[ 0 ] != ':' ) );
                  yield_p++ )
            {
            }
            yield_p = text_skip_past_whitespace( ++yield_p );

            //  The first part should be an amount
            for (;
                  ( ( isspace( yield_p[ 0 ] ) == 0 )
                    && ( yield_p[ 0 ] != '\0' ) );
                  yield_p++ )
            {
                strncat( local_amount, &yield_p[ 0 ], 1 );
            }
            //  Skip over spaces and or tabs between the
            //  amount and unit fields
            yield_p = text_skip_past_whitespace( yield_p );

            //  Copy the units field
            for (;
                  yield_p[ 0 ] != '\0';
                  yield_p++ )
            {
                strncat( local_unit, &yield_p[ 0 ], 1 );
            }
            //  Change the pass_fail flag to PASS
            mmf_rc = true;
        }
    }
    //  Before we go any farther.  Make sure there was something
    //  in the input data.
    if ( strlen( local_amount ) > 0 )
    {
        //  Time to figure out if this is a serves or MAKES field.
        if ( strlen( local_unit ) > 0 )
        {
            char                    tmp_unit[ MAX_LINE_L ];
            char                *   tmp_unit_p;

            //  To compare this field, I have to make it UPPER case
            //  Do this is a temp buffer
            memset( tmp_unit, '\0', sizeof ( tmp_unit ) );
            for ( tmp_unit_p = local_unit;
                  tmp_unit_p[ 0 ] != '\0';
                  tmp_unit_p++ )
            {
                strncat( tmp_unit, tmp_unit_p, 1 );
                tmp_unit[ strlen( tmp_unit ) - 1 ] =
                    toupper( tmp_unit[ strlen( tmp_unit ) - 1 ] );
            }
            //  Is the unit SERVINGS ?
            if ( strncmp( tmp_unit, "SERVINGS", 8 ) == 0 )
            {
                //  YES:    Erase it
                tmp_unit[ 0 ] = '\0';
            }
            //  Is the units field empty
            if ( strlen( tmp_unit ) == 0 )
            {
                //  YES:    This is a serves amount, not a MAKES amount
                recipe_p->serves = text_copy_to_new( local_amount );

                log_write( MID_DEBUG_1, "mmf_lib.c", "Line: %d\n", __LINE__ );
            }
            else
            {
                //  NO:     This is a serves amount, not a MAKES amount
                recipe_p->makes      = text_copy_to_new( local_amount );

                log_write( MID_DEBUG_1, "mmf_lib.c", "Line: %d\n", __LINE__ );

                recipe_p->makes_unit = text_copy_to_new( tmp_unit );

                log_write( MID_DEBUG_1, "mmf_lib.c", "Line: %d\n", __LINE__ );
            }
        }
        else
        {
            //  This is a serves amount, not a MAKES amount
            recipe_p->serves = text_copy_to_new( local_amount );

            log_write( MID_DEBUG_1, "mmf_lib.c", "Line: %d\n", __LINE__ );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( mmf_rc );
}

/****************************************************************************/
/**
 *  Tests for the end of a Meal-Master Formatted recipe.
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
MMF__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mmf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    mmf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Process the first half (or the entire line)
        in_buffer_p = text_skip_past_whitespace( in_buffer_p );

        //  Did we run past the end of AUIP ?
        if (    ( recipe_p->blank_line         == true )
             && ( MMF__split( in_buffer_p ) != true )
             && ( recipe_p->first_auip         == true ) )
        {
            //  YES:    This must be part of the directions
            MMF__directions( recipe_p, in_buffer_p );

            //  End of AUIP
            mmf_rc = true;
        }
        else
        {
            //  Change the flag because we have data.
            recipe_p->blank_line = false;
            recipe_p->first_auip = true;

            //  Is this an AUIP split line ?
            if ( MMF__split( in_buffer_p ) == true )
            {
                //  Format the split line
                in_buffer_p = MMF__fmt_split( in_buffer_p );
            }

            // Format the AUIP line
            recipe_fmt_auip( recipe_p, in_buffer_p, RECIPE_FORMAT_MMF );
        }
    }
    else
    {
        //  So we can detect the previous line was blank.
        recipe_p->blank_line = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( mmf_rc );
}

/****************************************************************************/
/**
 *  Tests for the end of a Meal-Master Formatted recipe.
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
MMF__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mmf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    mmf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this the end of the recipe ?
    if ( MMF__is_end( in_buffer_p ) == true )
    {
        //  YES:    Change the return code
        mmf_rc = true;
    }
    else
    {
        //  Format the Directions line
        decode_add_instructions( recipe_p, in_buffer_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mmf_rc );
}
/****************************************************************************/