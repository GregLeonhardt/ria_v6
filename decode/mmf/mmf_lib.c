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
                                //*******************************************
#include "mmf_api.h"            //  API for all mmf_*               PUBLIC
#include "mmf_lib.h"            //  API for all MMF__*              PRIVATE
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
              && ( strlen( tmp_data_p ) == MMF_END_4_L ) ) )
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