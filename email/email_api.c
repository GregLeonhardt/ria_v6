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

#define ALLOC_EMAIL             ( "ALLOCATE STORAGE FOR EMAIL" )

#define STUB                    ( 0 )

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
#if ! STUB
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
     * @param email_start_flag  TRUE = e-mail processing                    */
    int                         email_start_flag;
    /**
     * @param tmp_data_p        Temporary data pointer                      */
    char                    *   tmp_data_p;
    /**
     *  @param  recipe_format   Format code for this recipe                 */
    enum    recipe_format_e     tmp_format;
#endif

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

        //  This thread is now the owner of the RCB
        rcb_p->tcb_p = tcb_p;

        //  Progress report.
        log_write( MID_LOGONLY, tcb_p->thread_name,
                   "SIZE: %10d - FILE-ID: %s\n",
                   list_query_count( rcb_p->import_list_p ),
                   rcb_p->file_path );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

#if ! STUB

        //  Clear the new RCB pointer
        new_rcb_p = NULL;

        /********************************************************************
         *  FUNCTIONAL CODE FOR THIS THREAD GOES HERE
         ********************************************************************/

        //  Lock the list for fast(er) access
        list_lock_key = list_user_lock( rcb_p->import_list_p );

        //  Scan the list
        for( list_data_p = list_fget_first( rcb_p->import_list_p, list_lock_key );
             list_data_p != NULL;
             list_data_p = list_fget_next( rcb_p->import_list_p, list_data_p, list_lock_key ) )
        {
            //  Remove the data from the level 1 list
            list_fdelete( rcb_p->import_list_p, list_data_p, list_lock_key );

            //  Test for the start of an e-Mail thread
            if (    ( rcb_p->recipe_format == RECIPE_FORMAT_NONE )
                 && ( EMAIL__is_start( list_data_p ) == true ) )
            {
                email_start_flag = true;
            }

            //  Are we processing an e-Mail message ?
            if (    ( email_start_flag     ==               true )
                 && ( rcb_p->recipe_format == RECIPE_FORMAT_NONE ) )
            {
                //  "NEWSGROUPS:"
                tmp_data_p = EMAIL__find_newsgroup( list_data_p );
                if ( tmp_data_p != NULL )
                {
                    //  YES:    Save the information
                    memset( rcb_p->email_info_p->g_from, '\0', FROM_L );
                    if ( strlen( tmp_data_p ) < FROM_L )
                        memcpy( rcb_p->email_info_p->g_from,
                                tmp_data_p, strlen( tmp_data_p ) );
                    else
                        memcpy( rcb_p->email_info_p->g_from,
                                tmp_data_p, FROM_L - 1 );
                }
                //  "SUBJECT:"
                tmp_data_p = EMAIL__find_subject( list_data_p );
                if ( tmp_data_p != NULL )
                {
                    //  YES:    Save the information
                    memset( rcb_p->email_info_p->e_subject, '\0', SUBJECT_L );
                    if ( strlen( tmp_data_p ) < FROM_L )
                        memcpy( rcb_p->email_info_p->e_subject,
                                tmp_data_p, strlen( tmp_data_p ) );
                    else
                        memcpy( rcb_p->email_info_p->e_subject,
                                tmp_data_p, SUBJECT_L - 1 );
                }
                //  "FROM:"
                tmp_data_p = EMAIL__find_from( list_data_p );
                if ( tmp_data_p != NULL )
                {
                    //  YES:    Save the information
                    memset( rcb_p->email_info_p->e_from, '\0', FROM_L );
                    if ( strlen( tmp_data_p ) < FROM_L )
                        memcpy( rcb_p->email_info_p->e_from,
                                tmp_data_p, strlen( tmp_data_p ) );
                    else
                        memcpy( rcb_p->email_info_p->e_from,
                                tmp_data_p, FROM_L - 1 );
                }
                //  "DATE:"
                tmp_data_p = EMAIL__find_datetime( list_data_p );
                if ( tmp_data_p != NULL )
                {
                    //  YES:    Save the information
                    memset( rcb_p->email_info_p->e_datetime, '\0', DATETIME_L );
                    if ( strlen( tmp_data_p ) < FROM_L )
                        memcpy( rcb_p->email_info_p->e_datetime,
                                tmp_data_p, strlen( tmp_data_p ) );
                    else
                        memcpy( rcb_p->email_info_p->e_datetime,
                                tmp_data_p, DATETIME_L - 1 );
                }
            }

            //  Is there an active recipe split ?
            if ( rcb_p->recipe_format == RECIPE_FORMAT_NONE )
            {
                //  NO:     Test foe the start of a new recipe.
                rcb_p->recipe_format = recipe_is_start( list_data_p );

                //  Is this the start of a new recipe ?
                if ( rcb_p->recipe_format != RECIPE_FORMAT_NONE )
                {
                    //  YES:    Clone the RCB
                    new_rcb_p = rcb_new( rcb_p );

                    //  Add this data buffer to the new recipe list.
                    list_put_last( new_rcb_p->import_list_p, list_data_p );
                }
                else
                {
                    //  This is trash data so throw it away
                    mem_free( list_data_p );
                }
            }
            else
            {
                //  Just in case this is the start of a new recipe
                tmp_format = recipe_is_start( list_data_p );

                //  Is this the end of the recipe
                if (    ( recipe_is_end( rcb_p->recipe_format, list_data_p ) == true )
                     || ( tmp_format                           != RECIPE_FORMAT_NONE )
                     || ( EMAIL__is_group_break( list_data_p )               == true ) )
                {
                    //  YES"    Put it in one of the DECODE queue
                    queue_put_payload( decode_tcb->queue_id, new_rcb_p  );

                    //  Clear the current recipe format
                    rcb_p->recipe_format = RECIPE_FORMAT_NONE;

                    //  Clear the new RCB pointer
                    new_rcb_p = NULL;
                }


                //  Is this the start of a recipe ?
                if ( tmp_format != RECIPE_FORMAT_NONE )
                {
                   //  YES:    Set the new recipe format
                    rcb_p->recipe_format = tmp_format;

                    //  Clone the RCB
                    new_rcb_p = rcb_new( rcb_p );
                }

                //  Is there a RCB to stote the data ?
                if ( new_rcb_p != NULL )
                {
                    //  YES:    Add this data buffer to the current recipe list.
                    list_put_last( new_rcb_p->import_list_p, list_data_p );
                }
            }

        }

        //  End-Of-File with an active recipe ?
        if ( new_rcb_p != NULL )
        {
            //  YES:    Put it in one of the DECODE queue
            queue_put_payload( decode_tcb->queue_id, new_rcb_p  );

            //  Clear the new RCB pointer
            new_rcb_p = NULL;
        }

        //  Release the lock on the level 3 list
        list_user_unlock( rcb_p->import_list_p, list_lock_key );

#endif

        //  Kill the Recipe Control Block
        rcb_kill( rcb_p );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/