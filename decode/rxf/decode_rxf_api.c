/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_rxf' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_RXF          ( "ALLOCATE STORAGE FOR DECODE_RXF" )

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
#include "recipe_api.h"         //  API for all recipe_*            PUBLIC
                                //*******************************************
#include "decode_api.h"         //  API for all decode_*            PUBLIC
#include "decode_rxf_lib.h"     //  API for all DECODE_RXF__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Public API Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook RXF formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return rxf_rc              Passed through from DECODE_RXF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
rxf_is_start(
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    rxf_rc = DECODE_RXF__is_start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
}

/****************************************************************************/
/**
 *  Test the data for the end of a Generic Recipe Formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return rxf_rc              Passed through from DECODE_RXF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
rxf_is_end(
    char                        *   data_p
    )
{
    /**
     * @param rxf_rc            Return Code                                 */
    int                             rxf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    rxf_rc = DECODE_RXF__is_end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( rxf_rc );
}

/****************************************************************************/
/**
 *  Decode a Big-Oven formatted recipe
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     void
 *
 *  @note
 *
 ****************************************************************************/

void
decode_rxf(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    rxf_decode_state_e  rxf_state;
    /**
     * @param list_lock_key     File list key                               */
    int                         list_lock_key;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   list_data_p;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                    *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, rcb_p->tcb_p->thread_name,
                  "Entering with %6d lines of text\n",
                  list_query_count( rcb_p->import_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( rcb_p->import_list_p );

    //  Change state to looking for the recipe title.
    rxf_state = RXF_DS_START;

    /************************************************************************
     *  Create a blank recipe structure
     ************************************************************************/

    //  Allocate a new recipe data structure
    rcb_p->recipe_p = recipe_new( rcb_p, RECIPE_FORMAT_RXF );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( rcb_p->import_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( rcb_p->import_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( rcb_p->import_list_p, list_data_p, list_lock_key );

        /*********************************************************************
         *  Look for a segment change
         *********************************************************************/

        //  Locate the first character in the buffer
        tmp_data_p = text_skip_past_whitespace( list_data_p );

        //--------------------------------------------------------------------
        //  -----  Recipe Import Assist Start ----- ?
        if ( DECODE_RXF__is_start( tmp_data_p ) == true )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_START;
        }
        //--------------------------------------------------------------------
        //  Description: ?
        if ( DECODE_RXF__is_description( tmp_data_p ) == true )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_DESCRIPTION;
        }
        //--------------------------------------------------------------------
        //  ----- Recipe Data ----- ?
        if ( DECODE_RXF__is_recipe_data( tmp_data_p ) == true )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_RECIPE_DATA;
        }
#if 0
        //--------------------------------------------------------------------
        //  Recipe By: ?
        if ( strncmp( tmp_data_p, RXF_BY, RXF_BY_L  ) == 0 )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_BY;
        }
        //--------------------------------------------------------------------
        //  Serving Size: ?
        else
        if ( strncmp( tmp_data_p, RXF_SERVING, RXF_SERVING_L  ) == 0 )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_SERVING;
        }
        //--------------------------------------------------------------------
        //  Cuisine: ?
        else
        if ( strncmp( tmp_data_p, RXF_CUISINE, RXF_CUISINE_L  ) == 0 )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_CUISINE;
        }
        //--------------------------------------------------------------------
        //  Main Ingredient: ?
        else
        if ( strncmp( tmp_data_p, RXF_MAIN, RXF_MAIN_L  ) == 0 )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_MAIN;
        }
        //--------------------------------------------------------------------
        //  Categories: ?
        else
        if ( strncmp( tmp_data_p, RXF_CATEGORY, RXF_CATEGORY_L  ) == 0 )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_CATEGORY;
        }
        //--------------------------------------------------------------------
        //  -= Ingredients =- ?
        else
        if ( strncmp( tmp_data_p, RXF_INGREDIENTS, RXF_INGREDIENTS_L  ) == 0 )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_INGREDIENTS;
        }
        //--------------------------------------------------------------------
        //  -= Instructions =- ?
        else
        if ( strncmp( tmp_data_p, RXF_INSTRUCTIONS, RXF_INSTRUCTIONS_L  ) == 0 )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_INSTRUCTIONS;
        }
#endif
        //--------------------------------------------------------------------
        //  -----  Recipe Import Assist End ----- ?
        else
        if ( DECODE_RXF__is_end( tmp_data_p ) == true )
        {
            //  YES:    Change the decode state
            rxf_state = RXF_DS_NEXT_SEGMENT;
        }
        //--------------------------------------------------------------------

        //  Process the new data
        switch( rxf_state )
        {

            /****************************************************************
             *  Search for the Start of a recipe
             ****************************************************************/

            case RXF_DS_START:
            {
                //  Change recipe Decode State
                rxf_state = RXF_DS_TITLE;

            }   break;

            /****************************************************************
             *  Process the Recipe Title
             ****************************************************************/

            case RXF_DS_TITLE:
            {
                //  Locate and process the recipe title
                if ( DECODE_RXF__do_title( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change to Title Case
                    text_title_case( rcb_p->recipe_p->name_p, rcb_p->recipe_p->name_p );

                    //  Change recipe Decode State
                    rxf_state = RXF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case RXF_DS_DESCRIPTION:
            {
                //  Locate and process the recipe title
                if ( DECODE_RXF__do_description( rcb_p->recipe_p, list_data_p ) != true )
                {
                    //  Change recipe Decode State
                    rxf_state = RXF_DS_AUIP;

                    //  There may be some data in the description processing buffer.
                    //  This call will flush it out.
                    DECODE_RXF__do_description( rcb_p->recipe_p, "   " );
                }
            }   break;

            /****************************************************************
             *  Process AUIP
             ****************************************************************/

            case RXF_DS_AUIP:
            {
                //  Locate and process the recipe title
                if ( DECODE_RXF__do_auip( rcb_p->recipe_p, list_data_p ) != true )
                {
                    //  Change recipe Decode State
                    rxf_state = RXF_DS_DIRECTIONS;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case RXF_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                DECODE_RXF__do_directions( rcb_p->recipe_p, list_data_p );

            }   break;

            /****************************************************************
             *  Process Recipe-Data
             ****************************************************************/

            case RXF_DS_RECIPE_DATA:
            {
                //  Locate and process the recipe title
                DECODE_RXF__do_recipe_data( rcb_p->recipe_p, list_data_p );

            }   break;
#if 0

            /****************************************************************
             *  Process Recipe By:
             ****************************************************************/

            case RXF_DS_BY:
            {
                //  Locate and process the recipe title
                if ( DECODE_RXF__recipe_by( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change recipe Decode State
                    rxf_state = RXF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  Process Servings:
             ****************************************************************/

            case RXF_DS_SERVING:
            {
                //  Locate and process the recipe title
                if ( DECODE_RXF__srv_size( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change recipe Decode State
                    rxf_state = RXF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  Process the Cuisine:
             *              Main Ingredient:
             *              Categories: information
             ****************************************************************/

            case RXF_DS_CUISINE:
            case RXF_DS_MAIN:
            case RXF_DS_CATEGORY:
            {
                //  Locate and process the recipe title
                if ( DECODE_RXF__categories( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change recipe Decode State
                    rxf_state = RXF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  -= Instructions =-
             ****************************************************************/

            case RXF_DS_INSTRUCTIONS:
            {
                //  Change recipe Decode State
                rxf_state = RXF_DS_DIRECTIONS;

            }   break;

            /****************************************************************
             *  Process End-of-Recipe
             ****************************************************************/

            case RXF_DS_END:
            {
                //  There may be some data in the directions processing buffer.
                //  This call will flush it out.
                DECODE_RXF__directions( rcb_p->recipe_p, "   " );

            }   break;
#endif

            /****************************************************************
             *  End of recipe decode.
             ****************************************************************/

            default:
            {
                //  Shouldn't be here
            }
        }

        //  Free the buffer
        mem_free( list_data_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the lock on the level 3 list
    list_user_unlock( rcb_p->import_list_p, list_lock_key );

    //  DONE!
}
/****************************************************************************/