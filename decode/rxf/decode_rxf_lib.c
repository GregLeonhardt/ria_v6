/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_rxf' library.
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
#include "decode_rxf_lib.h"     //  API for all DECODE_RXF__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    data_type_e
{
    RXF_DT_VOID                 =   0,
    RXF_DT_CUISINE              =   1,
    RXF_DT_OCCASION             =   2,
    RXF_DT_COURSE               =   3,
    RXF_DT_DIET                 =   4,
    RXF_DT_APPLIANCE            =   5,
    RXF_DT_CHAPTER              =   6,
    RXF_DT_COPYRIGHT            =   7,
    RXF_DT_SKILL_LEVEL          =   8,
    RXF_DT_RATING               =   9,
    RXF_DT_END                  =  99
};
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
/**
 * @param notes_parsing_flag        Set TRUE when parsing the notes segment */
static
int                                 notes_parsing_flag;
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/


/****************************************************************************/
/**
 *  Parse the 'YIELD" field for quantity and units
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  yield_p             Pointer to a a line of text to be scanned.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

static
int
DECODE_RXF__parse_yield(
    struct  recipe_t            *   recipe_p,
    char                        *   yield_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;
    /**
     *  @param  local_amount    Local buffer for AMOUNT                     */
    char                            local_amount[ MAX_LINE_L ];
    /**
     *  @param  local_unit      Local buffer for UNIT                       */
    char                            local_unit[ MAX_LINE_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume success
    rxf_rc = false;

    // Wipe clean the local buffers
    memset( local_amount,
            '\0',
            sizeof ( local_amount ) );
    memset( local_unit,
            '\0',
            sizeof ( local_unit ) );

    /************************************************************************
     *  The input format should be number units
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( yield_p ) != true )
    {
        yield_p = text_skip_past_whitespace( yield_p );

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
        rxf_rc = true;
    }

    //  Before we go any farther.  Make sure there was something
    //  in the input data.
    if ( strlen( local_amount ) > 0 )
    {
        //  YES:    Something here so save it.
        recipe_p->makes_p      = text_copy_to_new( local_amount );

        recipe_p->makes_unit_p = text_copy_to_new( local_unit );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
}

/****************************************************************************/
/**
 *  Parse the recipe categories.
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

static
int
DECODE_RXF__parse_data(
    struct  recipe_t            *   recipe_p,
    char                        *   data_p,
    enum    data_type_e             data_type
    )
{
    /**
     *  @param rxf_rc           Return Code                                 */
    int                             rxf_rc;
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
    rxf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( data_p ) != true )
    {
        data_p = text_skip_past_whitespace( data_p );

        //  Should be pointing to the start of a chapter or
        //  the end of the line.
        while ( data_p[ 0 ] != '\0' )
        {
            //  Wipe the temporary chapter buffer clean
            memset( &raw_chapter,
                    '\0',
                    sizeof ( raw_chapter ) );

            data_p = text_skip_past_whitespace( data_p );

            //  Copy the new chapter to the temp buffer
            for (  ;
                  (    ( data_p[ 0 ] != ',' )
                    && ( data_p[ 0 ] != '\0' ) );
                  data_p ++ )
            {
                strncat( raw_chapter, data_p, 1 );
            }
//log_write( MID_INFO, "DECODE_RXF__", "TMP: %s\n", raw_chapter );
            //  Skip past the ','
            data_p += 1;

            //  Translate the chapter
            tmp_p = xlate_chapter( raw_chapter );
//log_write( MID_INFO, "DECODE_RXF__", "%s = TRANSLATE( %s );\n", tmp_p, raw_chapter );

            //  Do we have a chapter to save ?
            if ( tmp_p != NULL )
            {
                //  YES:    Save it
                switch( data_type )
                {
                    case    RXF_DT_CUISINE:
                    {
//log_write( MID_INFO, "DECODE_RXF__", "RXF_DT_CUISINE:\t%s\n", raw_chapter );
                        decode_append( recipe_p->cuisine_p, raw_chapter );
                    }   break;
                    //
                    case    RXF_DT_OCCASION:
                    {
//log_write( MID_INFO, "DECODE_RXF__", "RXF_DT_OCCASION:\t%s\n", raw_chapter );
                        decode_append( recipe_p->occasion_p, raw_chapter );
                    }   break;
                    //
                    case    RXF_DT_COURSE:
                    {
//log_write( MID_INFO, "DECODE_RXF__", "RXF_DT_COURSE:\t\t%s\n", raw_chapter );
                        decode_append( recipe_p->course_p, raw_chapter );
                    }   break;
                    //
                    case    RXF_DT_DIET:
                    {
//log_write( MID_INFO, "DECODE_RXF__", "RXF_DT_DIET:\t\t%s\n", raw_chapter );
                        decode_append( recipe_p->diet_p, raw_chapter );
                    }   break;
                    //
                    case    RXF_DT_APPLIANCE:
                    {
//log_write( MID_INFO, "DECODE_RXF__", "RXF_DT_APPLIANCE:\t%s\n", raw_chapter );
                        decode_append( recipe_p->appliance_p, raw_chapter );
                    }   break;
                    //
                    default :
                    {
                    }
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
}

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
DECODE_RXF__is_start (
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe start tag
    rxf_rc = false;

    /************************************************************************
     *  Anything that starts with "-= Exported from BigOven =-"
     ************************************************************************/

    //  Is the a RXF start tag ?
    if (    ( data_p != NULL )                        //  Data is present
         && ( data_p[ 0 ] != '>' )                    //  Not forwarded e-mail
         && ( strncmp( data_p, RXF_START, RXF_START_L ) == 0 ) )
    {
        //  YES:    Change the return code
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |-----  Recipe Import Assist End -----|
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
DECODE_RXF__is_end(
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    rxf_rc = false;

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Big-Oven RXF recipe ?
    if ( strncmp( data_p, RXF_END, RXF_END_L  ) == 0 )
    {
        //  YES:    Change the return code
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   Description:
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
DECODE_RXF__is_description(
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    rxf_rc = false;

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Big-Oven RXF recipe ?
    if ( strncmp( data_p, RXF_DESCRIPTION, RXF_DESCRIPTION_L  ) == 0 )
    {
        //  YES:    Change the return code
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
}

/****************************************************************************/
/**
 *  Test the string for the start of the recipe data section.
 *      FORMATS:
 *          1   ----- Recipe Data -----
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
DECODE_RXF__is_recipe_data(
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    rxf_rc = false;

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Big-Oven RXF recipe ?
    if ( strncmp( data_p, RXF_RECIPE_DATA, RXF_RECIPE_DATA_L  ) == 0 )
    {
        //  YES:    Change the return code
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
}

/****************************************************************************/
/**
 *  Test the string for the start of the source information section.
 *      FORMATS:
 *          1   ----- Source Info -----
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
DECODE_RXF__is_source_info(
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    rxf_rc = false;

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Big-Oven RXF recipe ?
    if ( strncmp( data_p, RXF_SOURCE_INFO, RXF_SOURCE_INFO_L  ) == 0 )
    {
        //  YES:    Change the return code
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
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
DECODE_RXF__do_title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a title
    rxf_rc = false;

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

        // Change the pass_fail flag to PASS
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( rxf_rc );
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
DECODE_RXF__do_description(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DESCRIPTION
    rxf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this the end-of-recipe tag ?
    if ( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Format the AUIP line
//      decode_add_description( recipe_p, in_buffer_p );

        //  This is a directions line of text
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
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
DECODE_RXF__do_auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT an AUIP
    rxf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Process the first half (or the entire line)
        in_buffer_p = text_skip_past_whitespace( in_buffer_p );

        // Format the AUIP line
        decode_fmt_auip( recipe_p, in_buffer_p, RECIPE_FORMAT_RXF );

        //  Set the return code
        rxf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
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
DECODE_RXF__do_directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    rxf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Format the DIRECTIONS line
    decode_add_instructions( recipe_p, in_buffer_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
}

/****************************************************************************/
/**
 *  Process everything is the recipe data segment
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
DECODE_RXF__do_recipe_data(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    rxf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip past any leading whitespace.
    in_buffer_p = text_skip_past_whitespace( in_buffer_p );

//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "DATA: '%s'\n", in_buffer_p );
    //  AUTHOR:
    if ( strncmp( in_buffer_p, RXF_RECIPE_AUTHOR, RXF_RECIPE_AUTHOR_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_AUTHOR_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe author
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->author_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_AUTHOR -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  SERVES:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_SERVES, RXF_RECIPE_SERVES_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_SERVES_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the serves quantity
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->serves_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_AUTHOR -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  TIME_PREP:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_T_PREP, RXF_RECIPE_T_PREP_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_T_PREP_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the preperation time
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->time_prep_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_SERVES -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  TIME_COOK:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_T_COOK, RXF_RECIPE_T_COOK_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_T_COOK_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the cooking time
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->time_cook_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_T_COOK -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  TIME_WAIT:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_T_WAIT, RXF_RECIPE_T_WAIT_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_T_WAIT_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the waiting time
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->time_wait_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_T_WAIT -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  TIME_REST:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_T_REST, RXF_RECIPE_T_REST_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_T_REST_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the resting time
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->time_rest_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_T_REST -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  YIELD:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_YIELD, RXF_RECIPE_YIELD_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_YIELD_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the yield
        if ( strlen( tmp_data_p ) >= 1 )
            DECODE_RXF__parse_yield( recipe_p, tmp_data_p );

//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_YIELD -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  SOURCE:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_SOURCE, RXF_RECIPE_SOURCE_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_SOURCE_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe source
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->source_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_SOURCE -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  NOTES:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_NOTES, RXF_RECIPE_NOTES_L  ) == 0 )
    {
        //  YES:    Set the notes processing flag
        notes_parsing_flag = true;
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_SOURCE = TRUE\n" );
    }
    //  NOTES:
    else
    if ( notes_parsing_flag == true  )
    {
        //  YES:    Add the text to the notes.
        decode_fmt_notes( recipe_p, in_buffer_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_NOTES -- %s\n", tmp_data_p );

        // Is this the end of the notes ?
        if ( strlen( in_buffer_p ) == 0 )
        {
            //  YES:    Set the flag to false.
            notes_parsing_flag = false;
        }

    }
    //------------------------------------------------------------------------
    //  CUISINE:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_CUISINE, RXF_RECIPE_CUISINE_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_CUISINE_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the cuisine data
        if ( strlen( tmp_data_p ) >= 1 )
            DECODE_RXF__parse_data( recipe_p, tmp_data_p, RXF_DT_CUISINE );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_CUISINE -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  OCCASION:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_OCCASION, RXF_RECIPE_OCCASION_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_OCCASION_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the occasion data
        if ( strlen( tmp_data_p ) >= 1 )
            DECODE_RXF__parse_data( recipe_p, tmp_data_p, RXF_DT_OCCASION );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_OCCASION -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  COURSE:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_COURSE, RXF_RECIPE_COURSE_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_COURSE_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the course data
        if ( strlen( tmp_data_p ) >= 1 )
            DECODE_RXF__parse_data( recipe_p, tmp_data_p, RXF_DT_COURSE );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_COURSE -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  DIET:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_DIET, RXF_RECIPE_DIET_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_DIET_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the diet data
        if ( strlen( tmp_data_p ) >= 1 )
            DECODE_RXF__parse_data( recipe_p, tmp_data_p, RXF_DT_DIET );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_DIET -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  APPLIANCE:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_APPLIANCE, RXF_RECIPE_APPLIANCE_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_APPLIANCE_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the appliance data
        if ( strlen( tmp_data_p ) >= 1 )
            DECODE_RXF__parse_data( recipe_p, tmp_data_p, RXF_DT_APPLIANCE );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_APPLIANCE -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  CHAPTER:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_CHAPTER, RXF_RECIPE_CHAPTER_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_CHAPTER_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the chapter data
        if ( strlen( tmp_data_p ) >= 1 )
            DECODE_RXF__parse_data( recipe_p, tmp_data_p, RXF_DT_CHAPTER );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_CHAPTER -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  COPYRIGHT:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_COPYRIGHT, RXF_RECIPE_COPYRIGHT_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_COPYRIGHT_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the copyright information
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->copyright_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_COPYRIGHT -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  SKILL LEVEL:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_SKILL_LEVEL, RXF_RECIPE_SKILL_LEVEL_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_SKILL_LEVEL_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the skill level data
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->skill_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_SKILL_LEVEL -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  RATING:
    else
    if ( strncmp( in_buffer_p, RXF_RECIPE_RATING, RXF_RECIPE_RATING_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_RATING_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the rating data
        if ( strlen( tmp_data_p ) >= 1 )
            recipe_p->rating_p = text_copy_to_new( tmp_data_p );
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_RATING -- %s\n", tmp_data_p );
    }
    //------------------------------------------------------------------------
    //  NONE OF THE ABOVE.
    else
    {
        //  We are not parsing the notes field yet.
        notes_parsing_flag = false;
//log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "DEFAULT: notes_parsing_flag = false\n" );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
}

/****************************************************************************/
/**
 *  Process everything is the recipe data segment
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *  @param  in_buffer_p         Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a new recipe is detected
 *                              else FRC_FAIL.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_RXF__do_source_info(
    struct  rcb_t               *   rcb_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    rxf_rc = false;

    //  Initialize variables
    tmp_data_p = NULL;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip past any leading whitespace.
    in_buffer_p = text_skip_past_whitespace( in_buffer_p );

log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "DATA: '%s'\n", in_buffer_p );

    //------------------------------------------------------------------------
    //  START or END:
    if ( strncmp( in_buffer_p, "----- Source ", 13  ) == 0 )
    {
        //  YES:    Just ignore these tags
        {
        }
    }
#if 0   //  Use the original RECIPE-ID in case the AUIP have been changed
    //------------------------------------------------------------------------
    //  RECIPE-ID:
    if ( strncmp( in_buffer_p, RXF_RECIPE_ID, RXF_RECIPE_ID_L  ) == 0 )
    {
        //  YES:    Jump past the search string
        tmp_data_p = in_buffer_p + RXF_RECIPE_ID_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the rating data
        if ( strlen( tmp_data_p ) >= 1 )
        {
            if ( rcb_p->recipe_p->recipe_id_p != NULL )
            {
                //  Remove the old Recipe-ID
                mem_free( rcb_p->recipe_p->recipe_id_p );
                rcb_p->recipe_p->recipe_id_p = NULL;
            }
            rcb_p->recipe_p->recipe_id_p = text_copy_to_new( tmp_data_p );
        }
log_write( MID_INFO, "DECODE_RXF__do_recipe_data", "RXF_RECIPE_ID -- %s\n", rcb_p->recipe_p->recipe_id_p );
    }
#endif
    //------------------------------------------------------------------------
    //  NONE OF THE ABOVE.
    else
    {
        if ( strlen( in_buffer_p ) >= 1 )
            log_write( MID_INFO, "DECODE_RXF__do_source_info", "UNKNOWN -- '%s'\n", in_buffer_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
}


#if 0
/****************************************************************************/
/**
 *  Copy 'recipe by' information to the recipe structure.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return rxf_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_RXF__recipe_by(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'recipe by' information.
    rxf_rc = false;

    /************************************************************************
     *  Recipe By:
     ************************************************************************/

    //  Is there something already there ?
    if ( recipe_p->author_p == NULL )
    {
        //  YES:    Search the data buffer
        tmp_data_p = data_p + RXF_BY_L;

        //  Locate the first character in the buffer
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe title (name)
        recipe_p->author_p = text_copy_to_new( tmp_data_p );
    }
    else
    {
        //  NO:     OOPS.
        log_write( MID_WARNING, "RXF_DECODE__",  "\tA second 'Recipe By:' "
                   "was detected.\n" );
    }

    //  Change the return code
    rxf_rc = true;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
}

/****************************************************************************/
/**
 *  Copy 'serving size' information to the recipe structure.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return rxf_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_RXF__srv_size(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'serving size' information.
    rxf_rc = false;

    /************************************************************************
     *  Serving Size  :
     ************************************************************************/

    //  Is there something already there ?
    if ( recipe_p->serves_p == NULL )
    {
        //  YES:    Jump past the search string
        tmp_data_p = data_p + RXF_SERVING_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe title (name)
        recipe_p->serves_p = text_copy_to_new( tmp_data_p );
    }
    else
    {
        //  NO:     OOPS.
        log_write( MID_WARNING, "RXF_DECODE__",  "\tA second 'Serving Size:' "
                   "was detected.\n" );
    }



    //  Change the return code
    rxf_rc = true;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
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
DECODE_RXF__categories(
    struct  recipe_t            *   recipe_p,
    char                        *   categories_p
    )
{
    /**
     *  @param rxf_rc           Return Code                                 */
    int                             rxf_rc;
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
    rxf_rc = false;

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
    rxf_rc = true;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( rxf_rc );
}
#endif

/****************************************************************************/
