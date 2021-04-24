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
 *  library components of the 'email' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_EMAIL          ( "ALLOCATE STORAGE FOR EMAIL" )

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
#include "email_api.h"          //  API for all email_*             PUBLIC
#include "email_lib.h"          //  API for all EMAIL__*            PRIVATE
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
 *  Test the text line to see if it contains an e-Mail group break.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

int
email_is_group_break(
    char                        *   data_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( EMAIL__is_group_break( data_p ) );
}

/****************************************************************************/
/**
 *  Initialize the Translations tables.
 *
 *  @param  void            *   Pointer
 *
 *  @return void                Upon successful completion TRUE is returned
 *                              else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

void
email(
    void                    *   void_p
    )
{
    /**
     *  @param  tcb_p           Pointer to a Thread Control Block           */
    struct  tcb_t           *   tcb_p;
    /**
     *  @param  rcb_p           Pointer to a Recipe Control Block           */
    struct  rcb_t           *   rcb_p;
    /**
     *  @param  new_rcb_p       Pointer to a Recipe Control Block           */
    struct  rcb_t           *   new_rcb_p;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   list_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                         list_lock_key;
    /**
     * @param email_flag        A mark on the wall if wr are doing an e-Mail*/
    int                         email_flag;
    /**
     * @param content_type      e-Mail content type                         */
    enum    content_type_e      content_type;
    /**
     * @param encoding_type     e-Mail encoding type                        */
    enum    encoding_type_e     encoding_type;
    /**
     * @param boundary_type     e-Mail boundary type                        */
//  enum    boundary_type_e     boundary_type;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set the pointer
    tcb_p = void_p;

    //  Progress report.
    log_write( MID_INFO, tcb_p->thread_name,
               "Initialization complete.\n" );

    //  Change execution state to "WAIT"ing for work.
    tcb_p->thread_state = TS_WAIT;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    while ( 1 )
    {

        /********************************************************************
         *  Get a new file
         ********************************************************************/

        //  Get the current File-ID.
        rcb_p = queue_get_payload( tcb_p->queue_id );

        //  Progress report.
        log_write( MID_LOGONLY, tcb_p->thread_name,
                   "Q-%03d: Rcv: FILE-ID: %s\n",
                   tcb_p->queue_id, rcb_p->display_name );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

        /********************************************************************
         *  FUNCTIONAL CODE FOR THIS THREAD GOES HERE
         ********************************************************************/

        //  Lock the list for fast(er) access
        list_lock_key = list_user_lock( rcb_p->import_list_p );

        //  Is this the start of a new e-Mail ?
        list_data_p = list_fget_first( rcb_p->import_list_p, list_lock_key );

        //  Is there anything to scan ?
        if ( list_data_p == NULL )
        {
            //  Release the lock on the level 3 list
            list_user_unlock( rcb_p->import_list_p, list_lock_key );

            //  Start over again.
            continue;
        }

        if ( EMAIL__is_start( list_data_p ) == true )
        {
            //  YES:    Set a flag so we can track it.
            email_flag = true;

            //  Reset the content types.
            content_type   = CT_NONE;
            encoding_type  = CTE_NONE;

            log_write( MID_DEBUG_0, tcb_p->thread_name,
                          "Start      %p - '%.80s'\n", list_data_p, list_data_p );

            //  Done with this text line
            list_fdelete( rcb_p->import_list_p, list_data_p, list_lock_key );
            mem_free( list_data_p );
        }

        //  Scan the list
        for( list_data_p = list_fget_first( rcb_p->import_list_p, list_lock_key );
             list_data_p != NULL;
             list_data_p = list_fget_next( rcb_p->import_list_p, list_data_p, list_lock_key ) )
        {
            /**
             * @param tmp_c_type        e-Mail content type                 */
            enum    content_type_e          tmp_c_type;
            /**
             * @param tmp_e_type        e-Mail encoding type                */
            enum    encoding_type_e         tmp_e_type;

            //  Remove the data from the level 1 list
            list_fdelete( rcb_p->import_list_p, list_data_p, list_lock_key );

            tmp_c_type = EMAIL__find_content( tcb_p, list_data_p );
            tmp_e_type = EMAIL__find_encoding( tcb_p, list_data_p );

            //  Did we find the start of a recipe ?
            if ( rcb_p->recipe_format == RECIPE_FORMAT_NONE )
            {
                //  NO:     Maybe this is a recipe start.
                rcb_p->recipe_format = recipe_is_start( list_data_p );

                //  Is this the start of a recipe ?
                if ( rcb_p->recipe_format != RECIPE_FORMAT_NONE )
                {
                    //  YES:    Prepare for the new recipe
                    new_rcb_p = rcb_new( tcb_p, rcb_p, rcb_p->recipe_format );
                }
                else
                {
                    //  This is trash data so throw it away
                    mem_free( list_data_p );
                }
            }
            else
            {
                //  YES:    Add this data buffer to the recipe list.
                list_put_last( new_rcb_p->import_list_p, list_data_p );

                //  Is this the end of the recipe
                if ( recipe_is_end( rcb_p->recipe_format, list_data_p ) )
                {
                    //  YES:    Set the packet destination
                    new_rcb_p->dst_thread = DST_DECODE;

                    //  Put it in one of the ROUTER queue
                    queue_put_payload( router_queue_id, new_rcb_p  );

                    //  Clear the current recipe format
                    rcb_p->recipe_format = RECIPE_FORMAT_NONE;

                    //  Clear the new RCB pointer
                    new_rcb_p = NULL;
                }
            }



            //  @ToDo: 3 This is only here to avoid compile warnings
            if (    ( email_flag    ==     true )
                 && ( tmp_c_type    ==  CT_NONE )
                 && ( tmp_e_type    == CTE_NONE )
                 && ( content_type  ==  CT_NONE )
                 && ( encoding_type == CTE_NONE ) )
            {
                //  YES:
                tmp_c_type = content_type;
            }

        }

        //  Release the lock on the level 3 list
        list_user_unlock( rcb_p->import_list_p, list_lock_key );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/