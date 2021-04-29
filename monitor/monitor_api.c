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
 *  @ToDo: 0 Add EXPORT.
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
     * @param thread_id         Unique thread id number                     */
    int                         thread_id;
    /**
     *  @param                  Queue counters                              */
    int                         file_count;
    int                         router_count;
    int                         import_count[ THREAD_COUNT_IMPORT ];
    int                         import_total;
    int                         email_count[  THREAD_COUNT_EMAIL  ];
    int                         email_total;
    int                         decode_count[ THREAD_COUNT_DECODE ];
    int                         decode_total;
    int                         encode_count[ THREAD_COUNT_ENCODE ];
    int                         encode_total;
    /**
     *  @param  out_data        Output data buffer                          */
    char                        out_data[ 256 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set the pointer
    tcb_p = void_p;

    //  Progress report.
    log_write( MID_INFO, tcb_p->thread_name,
               "Initialization complete.\n" );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    while ( 1 )
    {
        /**
         *  @param  tmp_buffer      Temproary data buffer                   */
        char                        tmp_buffer[ 10 ];

        /********************************************************************
         *  Reset the totals
         ********************************************************************/

        //  Initialize all the total counts to zero
        import_total = 0;
        email_total  = 0;
        decode_total = 0;
        encode_total = 0;

        /********************************************************************
         *  Gather the queue depth data
         ********************************************************************/

        //  INPUT FILE
        file_count = list_query_count( file_list_p );

        //  ROUTER
        router_count = queue_get_count( router_tcb->queue_id );

        //  IMPORT
        for( thread_id = 0; thread_id < THREAD_COUNT_IMPORT; thread_id += 1 )
        {
            import_count[ thread_id ] = queue_get_count( import_tcb[ thread_id ]->queue_id );
            import_total += import_count[ thread_id ];
        }

        //  EMAIL
        for( thread_id = 0; thread_id < THREAD_COUNT_EMAIL; thread_id += 1 )
        {
            email_count[ thread_id ] = queue_get_count( email_tcb[ thread_id ]->queue_id );
            email_total += email_count[ thread_id ];
        }

        //  DECODE
        for( thread_id = 0; thread_id < THREAD_COUNT_DECODE; thread_id += 1 )
        {
            decode_count[ thread_id ] = queue_get_count( decode_tcb[ thread_id ]->queue_id );
            decode_total += decode_count[ thread_id ];
        }

        //  ENCODE
        for( thread_id = 0; thread_id < THREAD_COUNT_ENCODE; thread_id += 1 )
        {
            encode_count[ thread_id ] = queue_get_count( encode_tcb[ thread_id ]->queue_id );
            encode_total += encode_count[ thread_id ];
        }

        /********************************************************************
         *  Write the data to the log file
         ********************************************************************/

        //  Marker
        log_write( MID_LOGONLY, "monitor",
                   "\t---------------------\n" );

        //  FILE LIST
        log_write( MID_LOGONLY, "monitor",
                   "\tFILE:   [%6d] - %6d\n", file_count, file_count );

        //  ROUTER
        log_write( MID_LOGONLY, "monitor",
                   "\tROUTER: [%6d] - %6d\n", router_count, router_count );

        //  IMPORT
        memset( out_data, '\0', sizeof( out_data ) );
        snprintf( out_data, ( sizeof( out_data ) - strlen( out_data ) ),
                  "IMPORT: [%6d]", import_total );
        for( thread_id = 0; thread_id < THREAD_COUNT_IMPORT; thread_id += 1 )
        {
            snprintf( tmp_buffer, sizeof(tmp_buffer ),
                      " - %6d", import_count[ thread_id ] );
            strncat( out_data, tmp_buffer, ( sizeof( out_data ) - strlen( out_data ) ) );
        }
        log_write( MID_LOGONLY, "monitor", "\t%s\n", out_data );

        //  EMAIL
        memset( out_data, '\0', sizeof( out_data ) );
        snprintf( out_data, ( sizeof( out_data ) - strlen( out_data ) ),
                  "EMAIL:  [%6d]", email_total );
        for( thread_id = 0; thread_id < THREAD_COUNT_EMAIL; thread_id += 1 )
        {
            snprintf( tmp_buffer, sizeof(tmp_buffer ),
                      " - %6d", email_count[ thread_id ] );
            strncat( out_data, tmp_buffer, ( sizeof( out_data ) - strlen( out_data ) ) );
        }
        log_write( MID_LOGONLY, "monitor", "\t%s\n", out_data );

        //  DECODE
        memset( out_data, '\0', sizeof( out_data ) );
        snprintf( out_data, ( sizeof( out_data ) - strlen( out_data ) ),
                  "DECODE: [%6d]", decode_total );
        for( thread_id = 0; thread_id < THREAD_COUNT_DECODE; thread_id += 1 )
        {
            snprintf( tmp_buffer, sizeof(tmp_buffer ),
                      " - %6d", decode_count[ thread_id ] );
            strncat( out_data, tmp_buffer, ( sizeof( out_data ) - strlen( out_data ) ) );
        }
        log_write( MID_LOGONLY, "monitor", "\t%s\n", out_data );

        //  ENCODE
        memset( out_data, '\0', sizeof( out_data ) );
        snprintf( out_data, ( sizeof( out_data ) - strlen( out_data ) ),
                  "ENCODE: [%6d]", encode_total );
        for( thread_id = 0; thread_id < THREAD_COUNT_ENCODE; thread_id += 1 )
        {
            snprintf( tmp_buffer, sizeof(tmp_buffer ),
                      " - %6d", encode_count[ thread_id ] );
            strncat( out_data, tmp_buffer, ( sizeof( out_data ) - strlen( out_data ) ) );
        }
        log_write( MID_LOGONLY, "monitor", "\t%s\n", out_data );

        //  Output data every n seconds
        sleep( 5 );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/