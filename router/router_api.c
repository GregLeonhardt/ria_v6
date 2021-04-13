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
 *  library components of the 'router' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ROUTER          ( "ALLOCATEGORIESE STORAGE FOR ROUTER" )

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
#include "router_api.h"         //  API for all router_*            PUBLIC
#include "router_lib.h"         //  API for all ROUTER__*           PRIVATE
                                //*******************************************
#include "email_api.h"          //  API for all email_*             PUBLIC
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
router(
    void                    *   void_p
    )
{
    /**
     *  @param  tcb_p           Pointer to a Thread Control Block           */
    struct  tcb_t           *   tcb_p;
    /**
     *  @param  rcb_p           Pointer to a Recipe Control Block           */
    struct  rcb_t           *   rcb_p;

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
        /**
         * @param thread_id         Unique thread id number                 */
        int                         thread_id;
        /**
         *  @param  id              Thread ID with smallest queue           */
        int                         id;
        /**
         *  @param  queue_size      Queue size                              */
        int                         queue_size;

        /********************************************************************
         *  Get a new file to import
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

        //  Locate the destination queue
        switch( rcb_p->destination )
        {
            case    DST_INVALID:
            {
            }   break;
            //================================================================
            case    DST_IMPORT:
            {
                //  Set the base numbers
                id = 99999999;
                queue_size = 99999999;

                //  Locate the thread with the smallest queue depth
                for( thread_id = 0;
                     thread_id < THREAD_COUNT_IMPORT;
                     thread_id += 1 )
                {
                    int                     queue_count;

                    //  Get the queue count for this queue
                    queue_count = queue_get_count( import_tcb[ thread_id ]->queue_id );

                    //  Is it smaller than the current smallest ?
                    if ( queue_count < queue_size )
                    {
                        //  YES:    Use this queue
                        id = import_tcb[ thread_id ]->queue_id;
                        queue_size = queue_count;
                    }
                }

                //  Put it in one of the IMPORT queue's
                queue_put_payload( id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, "main",
                           "Q-%03d: Snd: FILE-ID: %s\n", id,
                           rcb_p->display_name );
            }   break;
            //================================================================
            case    DST_EMAIL:
            {
                //  Set the base numbers
                id = 99999999;
                queue_size = 99999999;

                //  Locate the thread with the smallest queue depth
                for( thread_id = 0;
                     thread_id < THREAD_COUNT_EMAIL;
                     thread_id += 1 )
                {
                    int                     queue_count;

                    //  Get the queue count for this queue
                    queue_count = queue_get_count( email_tcb[ thread_id ]->queue_id );

                    //  Is it smaller than the current smallest ?
                    if ( queue_count < queue_size )
                    {
                        //  YES:    Use this queue
                        id = email_tcb[ thread_id ]->queue_id;
                        queue_size = queue_count;
                    }
                }

                //  Put it in one of the EMAIL queue's
                queue_put_payload( id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, "main",
                           "Q-%03d: Snd: FILE-ID: %s\n", id,
                           rcb_p->display_name );
            }   break;
            //================================================================
            case    DST_DECODE:
            {
            }   break;
            //================================================================
            case    DST_ENCODE:
            {
            }   break;
            //================================================================
            case    DST_EXPORT:
            {
            }   break;
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