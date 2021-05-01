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
 *  library components of the 'monitor' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_MONITOR          ( "ALLOCATE STORAGE FOR MONITOR" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <unistd.h>             //  UNIX standard library.
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
#include "monitor_api.h"        //  API for all monitor_*           PUBLIC
#include "monitor_lib.h"        //  API for all MONITOR__*          PRIVATE
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
monitor(
    void                    *   void_p
    )
{
    /**
     *  @param  tcb_p           Pointer to a Thread Control Block           */
    struct  tcb_t           *   tcb_p;
    /**
     *  @param                  Queue counters                              */
    int                         file_total;
    int                         import_total;
    int                         email_total;
    int                         decode_total;
    int                         encode_total;
    int                         export_total;
    /**
     *  @param  line_count      Number of lines since printing header       */
    static int                  line_count;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set the pointer
    tcb_p = void_p;

    //  Initialize the line counter
    line_count = 0;

    //  Progress report.
    log_write( MID_INFO, tcb_p->thread_name,
               "Initialization complete.\n" );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    while ( 1 )
    {

        /********************************************************************
         *  Reset the totals
         ********************************************************************/


        /********************************************************************
         *  Gather the queue depth data
         ********************************************************************/

        //  INPUT FILE
        file_total = list_query_count( file_list_p );

        //  IMPORT
        import_total = queue_get_count( import_tcb->queue_id );

        //  EMAIL
        email_total = queue_get_count( email_tcb->queue_id );

        //  DECODE
        decode_total = queue_get_count( decode_tcb->queue_id );

        //  ENCODE
        encode_total = queue_get_count( encode_tcb->queue_id );

        //  EXPORT
        export_total = queue_get_count( export_tcb->queue_id );

        /********************************************************************
         *  Write the data to the log file
         ********************************************************************/

        //  Print the header
        if ( ( line_count % 15 ) == 0 )
        {
            log_write( MID_LOGONLY, "monitor",
                       "\n" );
            log_write( MID_LOGONLY, "monitor",
                       "\t     FILES    IMPORT     EMAIL    DECODE    ENCODE    EXPORT\n" );
        }

        //  FILE LIST
        log_write( MID_LOGONLY, "monitor",
                   "\t%10d%10d%10d%10d%10d%10d\n",
                   file_total, import_total, email_total,
                   decode_total, encode_total, export_total );

        //  Output data every n seconds
        sleep( 5 );

        //  Another line is complete
        line_count += 1;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/