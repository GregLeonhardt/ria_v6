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
 *  Initialize the Translations tables.
 *
 *  @param  void                No parameters are passed in.
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
     * @param list_data_p       Pointer to the read data                    */
    char                    *   list_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                         list_lock_key;

    //  @ToDo:  Move to rcb_p

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


        //  @ToDo:  This is only here to avoid compile warnings
        if (    ( email_flag    ==     true )
             && ( content_type  ==  CT_NONE )
             && ( encoding_type == CTE_NONE ) )
        {
            //  YES:
            printf( "Compile warning hack\n" );
        }






        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/