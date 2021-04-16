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
 *  library components of the 'import' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_IMPORT          ( "ALLOCATE STORAGE FOR IMPORT" )

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
#include "import_api.h"         //  API for all import_*            PUBLIC
#include "import_lib.h"         //  API for all IMPORT__*           PRIVATE
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
 *  @param  tcb_p               Pointer to a thread control block
 *
 *  @return void                No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
import(
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
     *  @param  file_name       Full file name (with directory)             */
    char                        file_name[ FILE_NAME_L * 3 ];
    /**
     *  @param  read_data_p     Read data buffer                            */
    char                    *   read_data_p;
    /**
     *  @param  read_data_l     Read data buffer                            */
    size_t                      read_data_l;

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
         *  Get a new file to import
         ********************************************************************/

        //  Get the current File-ID.
        rcb_p = queue_get_payload( tcb_p->queue_id );

        //  Progress report.
        log_write( MID_INFO, tcb_p->thread_name,
                   "Q-%03d: Rcv: FILE-ID: %s\n",
                   tcb_p->queue_id, rcb_p->display_name );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

        //  Build the full path/file name
        snprintf( file_name, sizeof( file_name ),
                  "%s/%s",
                  rcb_p->file_info_p->dir_name,
                  rcb_p->file_info_p->file_name );

        //  Open the file for reading
        rcb_p->file_p = file_open_read( file_name );

        log_write( MID_DEBUG_0, tcb_p->thread_name,
                      "Allocate a new list structure 'import_list_p' [%p].\n",
                      rcb_p->import_list_p );

        do
        {
            //  Read a line of text
            read_data_l = 0;
            read_data_p = NULL;
            read_data_l = getline( &read_data_p, &read_data_l, rcb_p->file_p );

            //  Was the read successful ?
            if ( read_data_l != -1 )
            {
                //  YES:    Remove CR/LF
                text_remove_crlf( read_data_p );

                //  Is this a binary file ?
                if ( text_is_binary( read_data_p, read_data_l ) == true )
                {
                    //  YES:    Log the binary file
                    log_write( MID_INFO, tcb_p->thread_name,
                               "Skipping binary file '%s'\n",
                               rcb_p->display_name );

                    //  Done reading this file.
                    read_data_l = -1;
                    break;
                }
                else
                {
                    //  NO:     Put the new line on the list
                    list_put_last( rcb_p->import_list_p, read_data_p );
                }
            }

            //  Keep reading until we reach the end-of-file
        }   while( read_data_l != -1 );

        //  Close the import file
        file_close( rcb_p->file_p ); rcb_p->file_p = 0;

        //  Set the packet destination
        rcb_p->dst_thread = DST_EMAIL;

        //  Put it in one of the IMPORT queue's
        queue_put_payload( router_queue_id, rcb_p  );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

}
/****************************************************************************/