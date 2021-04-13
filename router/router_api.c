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

#define ALLOC_ROUTER          ( "ALLOCATE STORAGE FOR ROUTER" )

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
         *  @param  id              Thread ID with smallest queue           */
        int                         id;

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
        switch( rcb_p->dst_thread )
        {
            case    DST_INVALID:
            {
            }   break;
            //================================================================
            case    DST_IMPORT:
            {
                id = ROUTER__choose( import_tcb, THREAD_COUNT_IMPORT );

                //  Put it in one of the EMAIL queue's
                queue_put_payload( id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, "main",
                           "Q-%03d: Snd: FILE-ID: %s\n", id,
                           rcb_p->display_name );
            }   break;
            //================================================================
            case    DST_EMAIL:
            {
                id = ROUTER__choose( email_tcb, THREAD_COUNT_EMAIL );

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
                id = ROUTER__choose( decode_tcb, THREAD_COUNT_DECODE );

                //  Put it in one of the EMAIL queue's
                queue_put_payload( id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, "main",
                           "Q-%03d: Snd: FILE-ID: %s\n", id,
                           rcb_p->display_name );
            }   break;
            //================================================================
            case    DST_ENCODE:
            {
#if 0
                id = ROUTER__choose( encode_tcb, THREAD_COUNT_ENCODE );

                //  Put it in one of the EMAIL queue's
                queue_put_payload( id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, "main",
                           "Q-%03d: Snd: FILE-ID: %s\n", id,
                           rcb_p->display_name );
#endif
            }   break;
            //================================================================
            case    DST_EXPORT:
            {
#if 0
                id = ROUTER__choose( export_tcb, THREAD_COUNT_EXPORT );

                //  Put it in one of the EMAIL queue's
                queue_put_payload( id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, "main",
                           "Q-%03d: Snd: FILE-ID: %s\n", id,
                           rcb_p->display_name );
#endif
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