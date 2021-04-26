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
         *  @param  import_id       Last used import queue id.              */
        static  int                 import_id = 0;
        /**
         *  @param  email_id        Last used email  queue id.              */
        static  int                 email_id = 0;
        /**
         *  @param  decode_id       Last used decode queue id.              */
        static  int                 decode_id = 0;
        /**
         *  @param  encode_id       Last used encode queue id.              */
        static  int                 encode_id = 0;
        /**
         *  @param  export_id       Last used export queue id.              */
        static  int                 export_id = 0;

        /********************************************************************
         *  Get a new file to import
         ********************************************************************/

        //  Get the current File-ID.
        rcb_p = queue_get_payload( tcb_p->queue_id );

        //  Progress report.
        log_write( MID_LOGONLY, tcb_p->thread_name,
                   "Q-%03d: Rcv: FILE-ID: %s\n",
                   tcb_p->queue_id, rcb_p->file_path );

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
                //  Update the RCB_p with the current TCB_p
                rcb_p->tcb_p = import_tcb[ import_id ];

                //  Put it in one of the EMAIL queue's
                queue_put_payload( import_tcb[ import_id ]->queue_id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, tcb_p->thread_name,
                           "Q-%03d: Snd: FILE-ID: %s\n",
                           import_tcb[ import_id ]->queue_id,
                           rcb_p->file_path );

                if ( ++import_id >= THREAD_COUNT_IMPORT )
                    import_id = 0;
            }   break;
            //================================================================
            case    DST_EMAIL:
            {
                //  Update the RCB_p with the current TCB_p
                rcb_p->tcb_p = email_tcb[ email_id ];

                //  Put it in one of the EMAIL queue's
                queue_put_payload( email_tcb[ email_id ]->queue_id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, tcb_p->thread_name,
                           "Q-%03d: Snd: FILE-ID: %s\n",
                           email_tcb[ email_id ]->queue_id,
                           rcb_p->file_path );

                if ( ++email_id >= THREAD_COUNT_EMAIL )
                    email_id = 0;
            }   break;
            //================================================================
            case    DST_DECODE:
            {
                //  Update the RCB_p with the current TCB_p
                rcb_p->tcb_p = decode_tcb[ decode_id ];

                //  Put it in one of the EMAIL queue's
                queue_put_payload( decode_tcb[ decode_id ]->queue_id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, tcb_p->thread_name,
                           "Q-%03d: Snd: FILE-ID: %s\n",
                           decode_tcb[ decode_id ]->queue_id,
                           rcb_p->file_path );

                if ( ++decode_id >= THREAD_COUNT_DECODE )
                    decode_id = 0;
            }   break;
            //================================================================
            case    DST_ENCODE:
            {
                //  Update the RCB_p with the current TCB_p
                rcb_p->tcb_p = encode_tcb[ encode_id ];

                //  Put it in one of the EMAIL queue's
                queue_put_payload( encode_tcb[ encode_id ]->queue_id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, tcb_p->thread_name,
                           "Q-%03d: Snd: FILE-ID: %s\n",
                           encode_tcb[ encode_id ]->queue_id,
                           rcb_p->file_path );

                if ( ++encode_id >= THREAD_COUNT_ENCODE )
                    encode_id = 0;
            }   break;
            //================================================================
            case    DST_EXPORT:
            {
                //  Update the RCB_p with the current TCB_p
                rcb_p->tcb_p = export_tcb[ export_id ];

                //  Put it in one of the EMAIL queue's
                queue_put_payload( export_tcb[ export_id ]->queue_id, rcb_p  );

                //  Progress report.
                log_write( MID_LOGONLY, tcb_p->thread_name,
                           "Q-%03d: Snd: FILE-ID: %s\n",
                           export_tcb[ export_id ]->queue_id,
                           rcb_p->file_path );

                if ( ++export_id >= THREAD_COUNT_EXPORT )
                    export_id = 0;
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