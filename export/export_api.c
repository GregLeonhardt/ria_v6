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
 *  library components of the 'export' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_EXPORT            ( "ALLOCATE STORAGE FOR EXPORT" )

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
                                //*******************************************
#include "export_api.h"         //  API for all export_*            PUBLIC
#include "export_lib.h"         //  API for all EXPORT__*           PRIVATE
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
//  This is the first four characters of the Recipe-ID.
#define SUBDIRECTORY_L          ( 4 +1 )
//----------------------------------------------------------------------------
//  This is the name of the directory where ALL formatted recipes are saved to.
#define DIR_FORMATTED           "Recipes_Formatted"
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
export(
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
     * @param out_name          Encoded output file name                    */
    char                        out_name[ FILE_NAME_L + 1 ];
    /**
     * @param subdirectory      Sub Directory name based on the Recipe-ID   */
    char                        subdirectory[ SUBDIRECTORY_L + 1 ];
    /**
     * @param input_file_fp     Output File pointer                         */
    FILE                    *   out_file_fp;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   list_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                         list_lock_key;
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
         *  Get a new file to import
         ********************************************************************/

        //  Get the current File-ID.
        rcb_p = queue_get_payload( tcb_p->queue_id );

        //  This thread is now the owner of the RCB
        rcb_p->tcb_p = tcb_p;

        //  Progress report.
        log_write( MID_LOGONLY, tcb_p->thread_name,
                   "FILE-ID: %s\n",
                   rcb_p->file_path );

        //  Display progress.
        log_write( MID_INFO, tcb_p->thread_name,
                   "%s - %s\n",
                   rcb_p->recipe_p->recipe_id_p,
                   rcb_p->recipe_p->name_p );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

#if ! STUB

        /********************************************************************
         *  Open the output file
         ********************************************************************/

        //  Build the subdirectory name
        snprintf( subdirectory, sizeof( subdirectory ),
                  "%s", rcb_p->recipe_p->recipe_id_p );

        //  Start building the output name
        snprintf( out_name, sizeof( out_name ),
                  "%s", out_dir_name_p );

        //  If the directory does not already exist, create it.
        file_dir_exist( out_name, true );

        //  Append DIR_FORMATTED to the output name
        snprintf( out_name, sizeof( out_name ),
                  "%s/%s", out_dir_name_p, DIR_FORMATTED );

        //  If the directory does not already exist, create it.
        file_dir_exist( out_name, true );

        //  Append the subdirectory to the output name
        snprintf( out_name, sizeof( out_name ),
                  "%s/%s/%s", out_dir_name_p, DIR_FORMATTED, subdirectory );

        //  If the directory does not already exist, create it.
        file_dir_exist( out_name, true );

        //  Append the file name to the output name
        snprintf( out_name, sizeof( out_name ),
                  "%s/%s/%s/%s.txt", out_dir_name_p, DIR_FORMATTED,
                  subdirectory, rcb_p->recipe_p->recipe_id_p );

        //  Is this a recipe with no instructions ?
        if ( strncmp( rcb_p->recipe_p->recipe_id_p, "000000000000000000000000000000", 30 ) == 0 )
        {
            //  YES:    Append to whatever is already there
            out_file_fp = file_open_append( out_name );
        }
        else
        {
            //  NO:     Open a unique file for the output.
            out_file_fp = file_open_write_no_dup( out_name, "duplicate_" );
        }

        /********************************************************************
         *  Write the recipe file
         ********************************************************************/

        //  Lock the list for fast(er) access
        list_lock_key = list_user_lock( rcb_p->export_list_p );

        //  Scan the list
        for( list_data_p = list_fget_first( rcb_p->export_list_p, list_lock_key );
             list_data_p != NULL;
             list_data_p = list_fget_next( rcb_p->export_list_p, list_data_p, list_lock_key ) )
        {
            //  Remove the data from the list
            list_fdelete( rcb_p->export_list_p, list_data_p, list_lock_key );

            //  Write the line of text to the output file
            fprintf( out_file_fp, "%s", list_data_p );

            //  Free the data buffer
            mem_free( list_data_p );
        }

        //  Close the output file.
        file_close( out_file_fp );

        //  Release the lock on the level 3 list
        list_user_unlock( rcb_p->export_list_p, list_lock_key );

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