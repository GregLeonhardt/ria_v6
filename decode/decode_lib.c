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
 *  library components of the 'import' library.
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
#include "import_api.h"         //  API for all import_*            PUBLIC
#include "email_api.h"          //  API for all email_*             PUBLIC
#include "xlate_api.h"          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include "decode_api.h"         //  API for all decode_*            PUBLIC
#include "decode_lib.h"         //  API for all DECODE__*           PRIVATE
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
#define PREP_1                  "--"
#define PREP_1_L                strlen( PREP_1 )
//----------------------------------------------------------------------------
#define PREP_2                  ";"
#define PREP_2_L                strlen( PREP_2 )
//----------------------------------------------------------------------------
#define PREP_3                  ":"
#define PREP_3_L                strlen( PREP_3 )
//----------------------------------------------------------------------------
#define PREP_4                  "-"
#define PREP_4_L                strlen( PREP_4 )
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
 *  Verify the format of the current recipe
 *
 *  @param  rcb_p               Pointer to a Recipe Control Block
 *
 *  @return                     TRUE for a valid recipe, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE__recipe_verify(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  decode_rc       Return code from this function              */
    int                         decode_rc;
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t          *   auip_p;
    /**
     *  @param  tmp_data_p      Pointer to a temporary data buffer          */
    char                    *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize variables
    decode_rc = true;

    /************************************************************************
     *  Recipe
     ************************************************************************/

    //  NAME (TITLE)
    if (    ( rcb_p->recipe_p->name_p           == NULL )
         || ( strlen( rcb_p->recipe_p->name_p ) <     4 )
         || ( strlen( rcb_p->recipe_p->name_p ) >   255 ) )
                    decode_rc = false;

    //  @note   This is nothing but a hack.  Some recipes are being decoded
    //          that wind up with a recipe name (title) of <BR>.  These
    //          recipes are obviously corrupted.  The fix needs to be a
    //          few steps before here but as I am clueless as to what or
    //          where the broken code is i am just excluding them from
    //          being entered into the dBase here.
    else
    if ( strncasecmp( rcb_p->recipe_p->name_p, "<BR>", 4 ) == 0 )
                    decode_rc = false;

    //  AUIP        (AMOUNT - UNIT - INGREDIENT - PREPARATION)
    //-----------------------------------------------------------------------
    if (    ( decode_rc == true )
         && ( list_query_count( rcb_p->recipe_p->ingredient_p ) == 0 ) )
    {
        //  AUIP is required for a valid recipe
        decode_rc = false;
    }
    if (    ( decode_rc == true )
         && ( list_query_count( rcb_p->recipe_p->ingredient_p ) > 0 ) )
    {
        for( auip_p = list_get_first( rcb_p->recipe_p->ingredient_p );
             auip_p != NULL;
             auip_p = list_get_next( rcb_p->recipe_p->ingredient_p, auip_p ) )
        {
            //  TYPE
            if (    (         auip_p->type_p   != NULL )
                 && ( strlen( auip_p->type_p ) !=    1 ) )
                            decode_rc = false;
            //  AMOUNT
            else
            if (    (         auip_p->amount_p        != NULL )
                 && (    ( strlen( auip_p->amount_p ) <   0 )
                      || ( strlen( auip_p->amount_p ) >  15 ) ) )
                            decode_rc = false;
            //  UNIT
            else
            if (    (         auip_p->unit_p        != NULL )
                 && (    ( strlen( auip_p->unit_p ) <   0 )
                      || ( strlen( auip_p->unit_p ) >  15 ) ) )
                            decode_rc = false;
            //  INGREDIENT
            else
            if (    (         auip_p->ingredient_p        != NULL )
                 && (    ( strlen( auip_p->ingredient_p ) <   0 )
                      || ( strlen( auip_p->ingredient_p ) >  99 ) ) )
                            decode_rc = false;
            //  PREPERATION
            else
            if (    (         auip_p->preparation_p        != NULL )
                 && (    ( strlen( auip_p->preparation_p ) <   0 )
                      || ( strlen( auip_p->preparation_p ) >  64 ) ) )
                            decode_rc = false;

            //  Was there an error ?
            if ( decode_rc == false )
            {
                //  YES:    Stop the verification cycle
                break;
            }
        }
    }

    //  DIRECTIONS
#if 0
    if (    ( decode_rc == true )
         && ( list_query_count( rcb_p->recipe_p->directions_p ) == 0 ) )
    {
        //  DIRECTIONS are required for a valid recipe
//      decode_rc = false;
        decode_rc = true;
    }
#endif
    if (    ( decode_rc == true )
         && ( list_query_count( rcb_p->recipe_p->directions_p ) > 0 ) )
    {
        /**
         *  @param  size        Total size of the directions                */
        int                     size;

        size = 0;

        for( tmp_data_p = list_get_first( rcb_p->recipe_p->directions_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->directions_p, tmp_data_p ) )
        {
            //  Is there something to write ?
            if ( text_is_blank_line( tmp_data_p ) != true )
            {
                //  YES:    Add the string length to the total length
                size += strlen( tmp_data_p );
            }
        }

        //  Is the directions length within the acceptable range ?
        if ( ( size <     0 ) || ( size >  2048 ) )
            decode_rc = false;
    }

    //  NOTES
    if (    ( decode_rc == true )
         && ( list_query_count( rcb_p->recipe_p->notes_p ) > 0 ) )
    {
        /**
         *  @param  size        Total size of the directions                */
        int                     size;

        size = 0;

        for( tmp_data_p = list_get_first( rcb_p->recipe_p->notes_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->notes_p, tmp_data_p ) )
        {
            //  Is there something to write ?
            if ( text_is_blank_line( tmp_data_p ) != true )
            {
                //  YES:    Add the string length to the total length
                size += strlen( tmp_data_p );
            }
        }

        //  Is the directions length within the acceptable range ?
        if ( ( size <     0 ) || ( size >  2048 ) )
            decode_rc = false;
    }

    /************************************************************************
     *  Recipe-Information
     ************************************************************************/

    //  AUTHOR
    if (    ( decode_rc == true )
         && (      rcb_p->recipe_p->author_p           != NULL )
         && (    ( strlen( rcb_p->recipe_p->author_p ) <     0 )
              || ( strlen( rcb_p->recipe_p->author_p ) >   255 ) ) )
                    decode_rc = false;

    //  DESCRIPTION
    if (    ( decode_rc == true )
         && (      rcb_p->recipe_p->description_p           != NULL )
         && (    ( strlen( rcb_p->recipe_p->description_p ) <     0 )
              || ( strlen( rcb_p->recipe_p->description_p ) >   255 ) ) )
                    decode_rc = false;

    //  WEB_ADDRESS
    if (    ( decode_rc == true )
         && (      rcb_p->recipe_p->web_address_p           != NULL )
         && (    ( strlen( rcb_p->recipe_p->web_address_p ) <     0 )
              || ( strlen( rcb_p->recipe_p->web_address_p ) >   255 ) ) )
                    decode_rc = false;

    //  COPYRIGHT
    if (    ( decode_rc == true )
         && (      rcb_p->recipe_p->copyright_p           != NULL )
         && (    ( strlen( rcb_p->recipe_p->copyright_p ) <     0 )
              || ( strlen( rcb_p->recipe_p->copyright_p ) >   255 ) ) )
                    decode_rc = false;

    //  EDITED_BY
    if (    ( decode_rc == true )
         && (      rcb_p->recipe_p->edited_by_p           != NULL )
         && (    ( strlen( rcb_p->recipe_p->edited_by_p ) <     0 )
              || ( strlen( rcb_p->recipe_p->edited_by_p ) >   255 ) ) )
                    decode_rc = false;

    //  FORMATTED_BY
    if (    ( decode_rc == true )
         && (      rcb_p->recipe_p->formatted_by_p           != NULL )
         && (    ( strlen( rcb_p->recipe_p->formatted_by_p ) <     0 )
              || ( strlen( rcb_p->recipe_p->formatted_by_p ) >   255 ) ) )
                    decode_rc = false;

    //  IMPORTED_FROM
    if (    ( decode_rc == true )
         && (      rcb_p->recipe_p->import_from_p           != NULL )
         && (    ( strlen( rcb_p->recipe_p->import_from_p ) <     0 )
              || ( strlen( rcb_p->recipe_p->import_from_p ) >   255 ) ) )
                    decode_rc = false;

    /************************************************************************
     *  Recipe-Source
     ************************************************************************/

    //  GROUP_FROM
    if (    ( decode_rc == true )
         && (    ( strlen( rcb_p->email_info_p->g_from ) <   0 )
              || ( strlen( rcb_p->email_info_p->g_from ) > 255 ) ) )
                    decode_rc = false;

    //  GROUP_SUBJECT
    if (    ( decode_rc == true )
         && (    ( strlen( rcb_p->email_info_p->g_subject ) <   0 )
              || ( strlen( rcb_p->email_info_p->g_subject ) > 255 ) ))
                    decode_rc = false;

    //  GROUP_DATETIME
    if (    ( decode_rc == true )
         && (    ( strlen( rcb_p->email_info_p->g_datetime ) <   0 )
              || ( strlen( rcb_p->email_info_p->g_datetime ) > 255 ) ) )
                    decode_rc = false;

    //  EMAIL_FROM
    if (    ( decode_rc == true )
         && (    ( strlen( rcb_p->email_info_p->e_from ) <   0 )
              || ( strlen( rcb_p->email_info_p->e_from ) > 255 ) ) )
                    decode_rc = false;

    //  EMAIL_SUBJECT
    if (    ( decode_rc == true )
         && (    ( strlen( rcb_p->email_info_p->e_subject ) <   0 )
              || ( strlen( rcb_p->email_info_p->e_subject ) > 255 ) ) )
                    decode_rc = false;

    //  EMAIL_DATETIME
    if (    ( decode_rc == true )
         && (    ( strlen( rcb_p->email_info_p->e_datetime ) <   0 )
              || ( strlen( rcb_p->email_info_p->e_datetime ) > 255 ) ) )
                    decode_rc = false;

#if 0
//----------------------------------------------------------------------------
struct   recipe_t
{
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
     *  @param  Chapter         List of Chapters for this recipe            */
    struct  list_base_t     *   chapter_p;
};
#endif

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( decode_rc );
}

/****************************************************************************/
/**
 *  Convert a text AM or PM to a integer.
 *
 *  @param  dow_p               A pointer to an input string containing
 *                              text string.
 *
 *  @return                     is successful 1 or 2, else 0.
 *
 *  @note
 *      RC 1 = AM
 *      RC 2 = PM
 *
 ****************************************************************************/

int
DECODE__am_or_pm(
    char                    *   dow_p
    )
{
    /**
     *  @param  month           The numeric month                           */
    int                         dow;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize variables
    dow = 0;

    /************************************************************************
     *  Function Body
     ************************************************************************/

         if ( strncasecmp( "AM",    dow_p, 2 ) == 0 ) dow =  1;
    else if ( strncasecmp( "PM",    dow_p, 2 ) == 0 ) dow =  2;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dow );
}

/****************************************************************************/
/**
 *  Convert a text Day-Of-Week string into a integer.
 *
 *  @param  dow_p               A pointer to an input string containing
 *                              text string.
 *
 *  @return                     is successful 1 -> 7, else 0.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE__day_of_week(
    char                    *   dow_p
    )
{
    /**
     *  @param  month           The numeric month                           */
    int                         dow;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize variables
    dow = 0;

    /************************************************************************
     *  Function Body
     ************************************************************************/

         if ( strncasecmp( "Sun",        dow_p, 3 ) == 0 ) dow =  1;
    else if ( strncasecmp( "Mon",        dow_p, 3 ) == 0 ) dow =  2;
    else if ( strncasecmp( "Tue",        dow_p, 3 ) == 0 ) dow =  3;
    else if ( strncasecmp( "Wed",        dow_p, 3 ) == 0 ) dow =  4;
    else if ( strncasecmp( "Thu",        dow_p, 3 ) == 0 ) dow =  5;
    else if ( strncasecmp( "Fri",        dow_p, 3 ) == 0 ) dow =  6;
    else if ( strncasecmp( "Sat",        dow_p, 3 ) == 0 ) dow =  7;
    else if ( strncasecmp( "Sunday",     dow_p, 6 ) == 0 ) dow =  1;
    else if ( strncasecmp( "Monday",     dow_p, 6 ) == 0 ) dow =  2;
    else if ( strncasecmp( "Tuesday",    dow_p, 7 ) == 0 ) dow =  3;
    else if ( strncasecmp( "Wednesday",  dow_p, 9 ) == 0 ) dow =  4;
    else if ( strncasecmp( "Thursday",   dow_p, 8 ) == 0 ) dow =  5;
    else if ( strncasecmp( "Friday",     dow_p, 6 ) == 0 ) dow =  6;
    else if ( strncasecmp( "Saturday",   dow_p, 8 ) == 0 ) dow =  7;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dow );
}

/****************************************************************************/
/**
 *  Convert a text Month string into a integer.
 *
 *  @param  month_p             A pointer to an input string containing
 *                              text string.
 *
 *  @return                     is successful 1 -> 12, else 0.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE__month(
    char                    *   month_p
    )
{
    /**
     *  @param  month           The numeric month                           */
    int                         month;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize variables
    month = 0;

    /************************************************************************
     *  Function Body
     ************************************************************************/

         if ( strncasecmp( "Jan",        month_p, 3 ) == 0 ) month =  1;
    else if ( strncasecmp( "Feb",        month_p, 3 ) == 0 ) month =  2;
    else if ( strncasecmp( "Mar",        month_p, 3 ) == 0 ) month =  3;
    else if ( strncasecmp( "Apr",        month_p, 3 ) == 0 ) month =  4;
    else if ( strncasecmp( "May",        month_p, 3 ) == 0 ) month =  5;
    else if ( strncasecmp( "Jun",        month_p, 3 ) == 0 ) month =  6;
    else if ( strncasecmp( "Jul",        month_p, 3 ) == 0 ) month =  7;
    else if ( strncasecmp( "Aug",        month_p, 3 ) == 0 ) month =  8;
    else if ( strncasecmp( "Sep",        month_p, 3 ) == 0 ) month =  9;
    else if ( strncasecmp( "Oct",        month_p, 3 ) == 0 ) month = 10;
    else if ( strncasecmp( "Nov",        month_p, 3 ) == 0 ) month = 11;
    else if ( strncasecmp( "Dec",        month_p, 3 ) == 0 ) month = 12;
    else if ( strncasecmp( "January",    month_p, 7 ) == 0 ) month =  1;
    else if ( strncasecmp( "February",   month_p, 8 ) == 0 ) month =  2;
    else if ( strncasecmp( "March",      month_p, 5 ) == 0 ) month =  3;
    else if ( strncasecmp( "April",      month_p, 5 ) == 0 ) month =  4;
    else if ( strncasecmp( "May",        month_p, 3 ) == 0 ) month =  5;
    else if ( strncasecmp( "June",       month_p, 4 ) == 0 ) month =  6;
    else if ( strncasecmp( "July",       month_p, 4 ) == 0 ) month =  7;
    else if ( strncasecmp( "August",     month_p, 6 ) == 0 ) month =  8;
    else if ( strncasecmp( "September",  month_p, 9 ) == 0 ) month =  9;
    else if ( strncasecmp( "October",    month_p, 7 ) == 0 ) month = 10;
    else if ( strncasecmp( "November",   month_p, 8 ) == 0 ) month = 11;
    else if ( strncasecmp( "December",   month_p, 8 ) == 0 ) month = 12;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( month );
}

/****************************************************************************/
/**
 *  Format an UNIT field.
 *
 *  @param  in_auip_p           A pointer to an input line containing
 *                              an Unit Of Measurement
 *  @param  unit_p              A pointer to a holding buffer for the
 *                              unit field.
 *  @param  out_buf_size        Size (in bytes) of the output buffer.
 *
 *  @return                     A pointer into the input buffer past the
 *                              Unit Of Measurement field.
 *
 *  @note
 *
 ****************************************************************************/

char  *
DECODE__fmt_unit(
    char                    *   in_auip_p,
    char                    *   unit_p,
    int                         out_buf_size
    )
{
    /**
     *  @param  xlate_unit_p    A pointer to the translated units.          */
    char                    *   xlate_unit_p;
    /**
     *  @param  unit            A local buffer for the unit of measurement  */
    char                        tmp_unit[ out_buf_size + 1 ];
    /**
     *  @param  ndx             Reference index                             */
    int                         ndx;
    /**
     *  @param  in_unit_p       Local pointer to input data                 */
    char                    *   in_unit_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Skip over leading spaces and or tabs
    in_unit_p = text_skip_past_whitespace( in_auip_p );

    //  Initialize the translated units buffer
    memset( tmp_unit, '\0', sizeof( tmp_unit) );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Does this line ONLY contain 'preperations' ?
    if ( DECODE__is_preperations( in_unit_p ) == false )
    {
        //  NO:     Start moving the amount to it's temporary buffer
        for( ndx = 0;
             ndx < ( out_buf_size );
             ndx += 1 )
        {
            //  Is this an alpha [a-zA-Z] character ?
            if ( isalpha( in_unit_p[ ndx ] ) != 0 )
            {
                //  YES:    Copy it to the temporary unit buffer
                tmp_unit[ ndx ] = in_unit_p[ ndx ];
            }
            //  Is this a period ['.']
            else
            if ( in_unit_p[ ndx ] == '.' )
            {
                //  YES:    Throw it away
                ndx += 1;
            }
            else
            {
                //  Nothing else here for the unit field
                break;
            }
        }

        //  Attempt to translate the unit of measurement
        xlate_unit_p = xlate_units( tmp_unit );

        //  Was a translated unit of measurement located ?
        if ( xlate_unit_p != NULL )
        {
            //  YES:    Copy the translation to the output buffer
            strncpy( unit_p, xlate_unit_p, out_buf_size );

            //  Strip off any trailing spaces.
            text_strip_whitespace( unit_p );

            //  Set the return pointer to after the unit field
            in_unit_p = &( in_unit_p[ ndx ] );

            //---------------------------------------------------------------
            //  Some recipe formats (CookenPro 2) add a '(s)' following the
            //  unit of measurement.  We will now skip past it [if it is there].
            //---------------------------------------------------------------

            //  Does the character string '(s)' exist ?
            if (    ( in_unit_p[ 0 ] == '(' )
                 && ( in_unit_p[ 1 ] == 's' )
                 && ( in_unit_p[ 2 ] == ')' ) )
            {
                //  YES:    Skip past it.
                in_unit_p += 3;

                //  And any white space following it.
                text_strip_whitespace( in_unit_p );
            }
        }
        else
        {
            //  NO:     Return the entry pointer
            in_unit_p = in_auip_p;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Return the pointer that is set past the unit field
    return ( in_unit_p );
}

/****************************************************************************/
/**
 *  Test the line to see it it begins with a preparation method.
 *
 *  @param  data_p              Pointer to an input line.
 *
 *  @return                     TRUE if this is preparations only else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE__is_preperations(
    char                    *   data_p
    )
{
    /**
     *  @param  recipe_rc       Function return code                        */
    int                         recipe_rc;
    /**
     *  @param  in_amount_p     Local pointer to the source data            */
    char                    *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT a preparation ONLY line
    recipe_rc = false;

    //  Skip over leading spaces and or tabs
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Test the input line
    if (    ( strncmp( tmp_data_p, PREP_1, PREP_1_L ) == 0 )
         || ( strncmp( tmp_data_p, PREP_2, PREP_2_L ) == 0 )
         || ( strncmp( tmp_data_p, PREP_3, PREP_3_L ) == 0 )
         || ( strncmp( tmp_data_p, PREP_4, PREP_4_L ) == 0 ) )
    {
        //  YES:    Change the return code.
        recipe_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( recipe_rc );
}

/****************************************************************************/
/**
 *  Format an INGREDIENT field.
 *
 *  @param  in_auip_p           A pointer to an input line containing
 *                              an ingredient
 *  @param  ingredient_p        A pointer to a holding buffer for the
 *                              ingredient field.
 *  @param  mmf_length          Unprocessed length of a MM ingredient line.
 *  @param  out_buf_size        Size (in bytes) of the output buffer.
 *
 *  @return                     A pointer into the input buffer past the
 *                              ingredient field.
 *
 *  @note
 *
 ****************************************************************************/

char  *
DECODE__fmt_ingredient(
    char                    *   in_auip_p,
    char                    *   ingredient_p,
    int                         mmf_length,
    int                         out_buf_size
    )
{
    /**
     *  @param  in_unit_p       Local pointer to input data                 */
    char                    *   in_ingredient_p;
    /**
     *  @param  ndx             Reference index                             */
    int                         ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Skip over leading spaces and or tabs
    in_ingredient_p = text_skip_past_whitespace( in_auip_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Does this line ONLY contain 'preperations' ?
    if ( DECODE__is_preperations( in_ingredient_p ) == false )
    {
        //  NO:     Start moving the amount to it's temporary buffer
        for( ndx = 0;
             ndx < ( out_buf_size );
             ndx += 1 )
        {
            //  Is this something that marks the start of preparations ?
            if (    ( in_ingredient_p[ ndx ] != ';'  )
                 && ( in_ingredient_p[ ndx ] != ':'  )
                 && ( in_ingredient_p[ ndx ] != ','  )
                 && ( in_ingredient_p[ ndx ] != '('  )
                 && ( in_ingredient_p[ ndx ] != '{'  )
                 && ( in_ingredient_p[ ndx ] != '['  )
                 && ( in_ingredient_p[ ndx ] != '\t' )
                 && ( in_ingredient_p[ ndx ] != '\0' ) )
            {
                //  NO:     Is this a hyphenated word ?
                if (    (          in_ingredient_p[ ndx     ]   == '-' )
                     && ( isalpha( in_ingredient_p[ ndx + 1 ] ) !=  0  ) )
                {
                    //  YES:    Continue copying to the ingredients
                    ingredient_p[ ndx ] = in_ingredient_p[ ndx ];
                }
                //  MAYBE:  Is this a hyphen by itself ?
                else
                if ( in_ingredient_p[ ndx ] != '-' )
                {
                    //  NO:     Copy the current character to the output buffer
                ingredient_p[ ndx ] = in_ingredient_p[ ndx ];
                }
                else
                {
                    //  YES:    It is a hyphen so stop the copy
                    break;
                }
            }
            else
            {
                //  YES:    Done with the ingredient field.
                break;
            }
        }

        //  Did we locate an ingredient field ?
        if ( strlen( ingredient_p ) == 0 )
        {
            //  NO:     Return the entry pointer
            in_ingredient_p = in_auip_p;
        }
        else
        {
            //  YES:    Set the return address to the end of the ingredient field
            in_ingredient_p = &(in_ingredient_p[ ndx ]);

            //  Remove trailing spaces from the ingredient
            text_strip_whitespace( ingredient_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return the pointer that is set past the ingredient field
    return ( in_ingredient_p );
}

/****************************************************************************/
/**
 *  Format an PREPARATION field.
 *
 *  @param  in_auip_p           A pointer to an input line
 *  @param  preparation_p       A pointer to a holding buffer for the
 *                              preparation field.
 *  @param  mmf_length          Unprocessed length of a MM AUIP line.
 *  @param  out_buf_size        Size (in bytes) of the output buffer.
 *
 *  @return                     A pointer into the input buffer past the
 *                              preparation field.
 *
 *  @note
 *
 ****************************************************************************/

char  *
DECODE__fmt_preparation(
    char                    *   in_auip_p,
    char                    *   preparation_p,
    int                         mmf_length,
    int                         out_buf_size
    )
{
    /**
     *  @param  tmp_prep_p      Local pointer to input data                 */
    char                    *   tmp_prep_p;
    /**
     *  @param  ndx             A value used during truncation of dashes    */
    int                         ndx;
    /**
     *  @param  eol             Number of characters in the current line    */
    int                         eol;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Skip over leading spaces and or tabs
    tmp_prep_p = text_skip_past_whitespace( in_auip_p );

    /************************************************************************
     *  Reformat a section split  (--- SOMETHING ---)
     ************************************************************************/

    //  Is the string long enough for this to be a marker ?
    if ( strlen( tmp_prep_p ) > 9 )
    {
        //  YES:    Set the line length
        eol = ( strlen( tmp_prep_p ) - 1 );
        ndx = 0;

        //  NO:     Is this a comment line ?
        if (    ( tmp_prep_p[ ndx     ] == '-' )
             && ( tmp_prep_p[ ndx + 1 ] == '-' )
             && ( tmp_prep_p[ ndx + 2 ] == '-' )
             && ( tmp_prep_p[ eol     ] == '-' )
             && ( tmp_prep_p[ eol - 1 ] == '-' )
             && ( tmp_prep_p[ eol - 2 ] == '-' ) )
        {
            //  YES:    Is there anything other then dashes [-] ?
            for( ndx = 0;
                 tmp_prep_p[ ndx ] == '-';
                 ndx += 1 )
            {
                //  Nothing to do until we find something other then a dash.
            }

            //  Did we run to the End-of-Line ?
            if ( tmp_prep_p[ ndx ] != '\0' )
            {
                //  NO:     Change leading [--- ] to spaces [** ].
                for( ndx = 0;
                     (    ( tmp_prep_p[ ndx ] == '-' )
                       || ( tmp_prep_p[ ndx ] == ' ' ) );
                     ndx += 1 )
                {
                    tmp_prep_p[ ndx ] = ' ';
                }
                tmp_prep_p[ ndx - 2 ] = '*';
                tmp_prep_p[ ndx - 3 ] = '*';

                //  Change leading [--- ] to spaces [ **].
                for( ndx = ( strlen( tmp_prep_p ) - 1 );
                     (    ( tmp_prep_p[ ndx ] == '-' )
                       || ( tmp_prep_p[ ndx ] == ' ' ) );
                     ndx -= 1 )
                {
                    tmp_prep_p[ ndx ] = ' ';
                }
                tmp_prep_p[ ndx + 2 ] = '*';
                tmp_prep_p[ ndx + 3 ] = '*';

                //  Skip over leading and trailing spaces and or tabs
                tmp_prep_p = text_skip_past_whitespace( tmp_prep_p );
                text_strip_whitespace( tmp_prep_p );

                //  Make the text all UPPERCASE
                text_to_uppercase( tmp_prep_p );
            }
        }
    }

    /************************************************************************
     *  Skip past a leading double dash.
     ************************************************************************/

    //  Is the first character a coma
    if (    ( tmp_prep_p[ 0 ] == '-' )
         && ( tmp_prep_p[ 1 ] == '-' ) )
    {
        //  YES:    Jump over it
        tmp_prep_p += 2;

        //  And any spaces that may be here
        tmp_prep_p = text_skip_past_whitespace( tmp_prep_p );
    }

    /************************************************************************
     *  Skip past a leading coma.
     ************************************************************************/

    //  Is the first character a coma
    if (    ( tmp_prep_p[ 0 ] == ',' )
         || ( tmp_prep_p[ 0 ] == ':' ) )
    {
        //  YES:    Jump over it
        tmp_prep_p += 1;

        //  And any spaces that may be here
        tmp_prep_p = text_skip_past_whitespace( tmp_prep_p );
    }

    /************************************************************************
     *  Reformat AUIP - preparation format
     ************************************************************************/

    //  Start moving the preparations data to it's transfer buffer
    for( ndx = 0;
         ndx < ( out_buf_size );
         ndx += 1 )
    {
        //  Are we at the end of this line ?
        if ( tmp_prep_p[ ndx ] != '\0' )
        {
            //  Look for a second ' -- '
            if (    ( tmp_prep_p[ ndx     ] == ' ' )
                 && ( tmp_prep_p[ ndx + 1 ] == '-' )
                 && ( tmp_prep_p[ ndx + 2 ] == '-' )
                 && ( tmp_prep_p[ ndx + 3 ] == ' ' ) )
            {
                //  YES:    Replace it with a semi-colon.
                tmp_prep_p[ ndx ] = ';';
                text_remove( tmp_prep_p, ( ndx + 1 ), 2 );
            }
            //  Copy this character to the output buffer
            preparation_p[ ndx ] = tmp_prep_p[ ndx ];
        }
        else
        {
            //  YES:    Move the pointer
            tmp_prep_p = &(tmp_prep_p[ ndx ] );

            //  Nothing else to copy over
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return the pointer that is set past the preparation field
    return ( tmp_prep_p );
}

/****************************************************************************/
/**
 *  Add new ingredient information to the recipe
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  amount_p            Pointer to the amount field
 *  @param  unit_p              Pointer to the unit field
 *  @param  ingredient_p        Pointer to the ingredient field
 *  @param  preparation_p       Pointer to the preparation field
 *
 *  @return                     A pointer into the input buffer past the
 *                              preparation field.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__new_auip(
    struct   recipe_t       *   recipe_p,
    char                    *   amount_p,
    char                    *   unit_p,
    char                    *   ingredient_p,
    char                    *   preparation_p
    )
{
    /**
     *  @param  auip_p          Pointer to allocated AUIP structure         */
    struct  auip_t          *   auip_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Allocate storage and copy data
     ************************************************************************/

    //  Allocate a new ingredient structure
    auip_p = mem_malloc( sizeof( struct auip_t ) );

    log_write( MID_DEBUG_1, "recipe_lib.c", "Line: %d\n", __LINE__ );

    //  AMOUNT
    if ( strlen( amount_p ) > 0 )
    {
        //  Copy source information to the new buffer
        auip_p->amount_p = text_copy_to_new( amount_p );

        log_write( MID_DEBUG_1, "recipe_lib.c", "Line: %d\n", __LINE__ );
    }

    //  UNIT
    if ( strlen( unit_p ) > 0 )
    {
        //  Copy source information to the new buffer
        auip_p->unit_p = text_copy_to_new( unit_p );

        log_write( MID_DEBUG_1, "recipe_lib.c", "Line: %d\n", __LINE__ );
    }

    //  INGREDIENT
    if ( strlen( ingredient_p ) > 0 )
    {
        //  Copy source information to the new buffer
        auip_p->ingredient_p = text_copy_to_new( ingredient_p );

        log_write( MID_DEBUG_1, "recipe_lib.c", "Line: %d\n", __LINE__ );
    }

    //  PREPERATION
    if ( strlen( preparation_p ) > 0 )
    {
        //  Copy source information to the new buffer
        auip_p->preparation_p = text_copy_to_new( preparation_p );

        log_write( MID_DEBUG_1, "recipe_lib.c", "Line: %d\n", __LINE__ );
    }

    /************************************************************************
     *  Append the information to the recipe
     ************************************************************************/

    //  Append the new ingredient to the list
    list_put_last( recipe_p->ingredient_p, auip_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Format an AMOUNT field.
 *
 *  @param  in_auip_p       Pointer to an input line containing an Amount.
 *  @param  amount_p        Pointer to a holding buffer for the amount field.
 *  @param  out_buf_size    Size (in bytes) of the output buffer.
 *
 *  @return                 Pointer into the input buffer past the amount field.
 *
 *  @note
 *
 ****************************************************************************/

char  *
DECODE__fmt_amount(
    char                    *   in_auip_p,
    char                    *   amount_p,
    int                         out_buf_size
    )
{
    /**
     *  @param  in_amount_p     Local pointer to the source data            */
    char                    *   in_amount_p;
    /**
     *  @param  ndx             Positional index into a buffer              */
    int                         ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Skip over leading spaces and or tabs
    in_amount_p = text_skip_past_whitespace( in_auip_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Does this line ONLY contain 'preperations' ?
    if ( DECODE__is_preperations( in_amount_p ) == false )
    {
        //  NO:     Start moving the amount to it's temporary buffer
        for( ndx = 0;
             ndx < ( out_buf_size );
             ndx += 1 )
        {
            //  Did someone put a dash between whole number and fraction ?
            if ( in_amount_p[ ndx ] == '-' )
            {
                //  YES:    Change the character to a space [' ']
                in_amount_p[ ndx ] = ' ';
            }

            //  Is this character a:
            //      numeric [0-9]
            //      space   [' ']
            //      period  ['.']
            //      slash   ['/']   character ?
            if (    ( isdigit( in_amount_p[ ndx ] ) !=  0  )
                 || (          in_amount_p[ ndx ]   == ' ' )
                 || (          in_amount_p[ ndx ]   == '.' )
                 || (          in_amount_p[ ndx ]   == '/' ) )
            {
                //  YES:    Copy the character to the output buffer
                amount_p[ ndx ] = in_amount_p[ ndx ];

                //  Two spaces back to back ?
                if (    ( in_amount_p[ ndx     ]   == ' ' )
                     && ( in_amount_p[ ndx + 1 ]   == ' ' ) )
                {
                    //  Time to leave
                    break;
                }
            }
            else
            {
                //  NO:     Get out of here
                break;
            }
        }

        //  If the next character is alpha the line is compressed
        //  and is probably something like 12Cups.  In this case
        //  back up one position and make it a space.
        if (    ( isalpha( in_amount_p[ ndx ] ) != 0 )
             && (                       ndx     != 0 ) )
        {
            in_amount_p[ --ndx ] = ' ';
        }

        //  Did we locate an amount field ?
        if ( strlen( amount_p ) != 0 )
        {
            //  YES:    Set the return address to the end of the amount field
            in_amount_p = &(in_amount_p[ ndx ]);

            //  Remove trailing spaces from the amount
            text_strip_whitespace( amount_p );
        }
        else
        {
            //  NO:     Return the entry pointer
            in_amount_p = in_auip_p;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return the pointer that is set past the amount field
    return ( in_amount_p );
}
/****************************************************************************/