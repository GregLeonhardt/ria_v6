/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'mmf' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_MMF          ( "ALLOCATE STORAGE FOR MMF" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
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
#include "decode_mmf_api.h"     //  API for all decode_mmf_*        PUBLIC
#include "decode_mmf_lib.h"     //  API for all DECODE_MMF__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    mmf_decode_state_e
{
    MMF_DS_IDLE                 =   0,
    MMF_DS_TITLE                =   1,
    MMF_DS_CATEGORIES           =   2,
    MMF_DS_YIELD                =   3,
    MMF_DS_AUIP                 =   4,
    MMF_DS_DIRECTIONS           =   5,
    MMF_DS_END                  =   6
};
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
 *  Test the data for the start of a MasterCook MMF formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mmf_rc              Passed through from DECODE_MMF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
mmf_is_start(
    char                    *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                         mmf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    mmf_rc = MMF__is_start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mmf_rc );
}

/****************************************************************************/
/**
 *  Test the data for the end of a MasterCook MMF formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mmf_rc              Passed through from DECODE_MMF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
mmf_is_end(
    char                    *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                         mmf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    mmf_rc = MMF__is_end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mmf_rc );
}

/****************************************************************************/
/**
 *  Decode a MealMaster formatted recipe
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No information is returned.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_mmf(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    mmf_decode_state_e  mmf_state;
    /**
     * @param list_lock_key     File list key                               */
    int                         list_lock_key;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   list_data_p;

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
    mmf_state = MMF_DS_TITLE;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    rcb_p->recipe_p = recipe_new( rcb_p, RECIPE_FORMAT_MMF );

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

        //  Debug log output
        log_write( MID_DEBUG_0, rcb_p->tcb_p->thread_name,
                      "'%.60s'\n", list_data_p );

        //  Process the new data
        switch( mmf_state )
        {

            /****************************************************************
             *  Search for the recipe title
             ****************************************************************/

            case MMF_DS_TITLE:
            {
                //  Locate and process the recipe title
                if ( MMF__title( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    mmf_state = MMF_DS_CATEGORIES;
                }
            }   break;

            /****************************************************************
             *  Search for recipe categories
             ****************************************************************/

            case MMF_DS_CATEGORIES:
            {
                //  Locate and process the recipe title
                if ( MMF__categories( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change to Title Case
                    text_title_case( rcb_p->recipe_p->name, rcb_p->recipe_p->name );

                    //  Log the new title
                    log_write( MID_INFO, rcb_p->tcb_p->thread_name,
                               "'%s - %s'\n",
                               rcb_p->display_name,
                               rcb_p->recipe_p->name );

                    //  Change decode state
                    mmf_state = MMF_DS_YIELD;
                }
            }   break;

            /****************************************************************
             *  Search for recipe yield
             ****************************************************************/

            case MMF_DS_YIELD:
            {
                //  Locate and process the recipe title
                if ( MMF__yield( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change decode state
                    mmf_state = MMF_DS_AUIP;
                }
            }   break;

            /****************************************************************
             *  Process AUIP
             ****************************************************************/

            case MMF_DS_AUIP:
            {
                //  Locate and process the recipe title
                if ( MMF__auip( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change decode state
                    mmf_state = MMF_DS_DIRECTIONS;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case MMF_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                if ( MMF__directions( rcb_p->recipe_p, list_data_p ) == true )
                {
                    //  Change decode state
                    mmf_state = MMF_DS_END;
                }
            }   break;

            /****************************************************************
             *  End of recipe decode.
             ****************************************************************/

            case MMF_DS_END:
            {
                //  There may be some data in the directions processing buffer.
                //  This call will flush it out.
                MMF__directions( rcb_p->recipe_p, "   " );
            }   break;


            /****************************************************************
             *  Here to catch the unused case
             ****************************************************************/

            default:
            {
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