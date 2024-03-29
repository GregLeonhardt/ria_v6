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
 *  library components of the 'encode' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE            ( "ALLOCATE STORAGE FOR ENCODE" )

#define STUB                    ( 0 )

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
#include "dbase_api.h"          //  API for all dbase_*             PUBLIC
                                //*******************************************
#include "encode_api.h"         //  API for all encode_*            PUBLIC
#include "encode_lib.h"         //  API for all ENCODE__*           PRIVATE
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
 *  Get the input file name.
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return data_p              Pointer to the input file name string.
 *
 *  @note
 *      @ToDo: 3 encode_get_if
 *
 ****************************************************************************/

char    *
encode_get_if(
    void
    )
{
    /**
     *  @param  data_p          Pointer to the output file name string.     */
//  char                    *   data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Save the input file name
//  data_p = in_file_name_p;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
//  return( data_p );
    return( NULL );
}

/****************************************************************************/
/**
 *  Set the input directory name.
 *
 *  @param  data_p              Pointer to the input directory name string.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *      @ToDo: 3 encode_set_dir
 *
 ****************************************************************************/

void
encode_set_dir(
    char                    *   data_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Save the input file name
//  in_dir_name_p = data_p;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Get the input directory name.
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return data_p              Pointer to the input directory name string.
 *
 *  @note
 *      @ToDo: 3 encode_get_dir
 *
 ****************************************************************************/

char    *
encode_get_dir(
    void
    )
{
    /**
     *  @param  data_p          Pointer to the output file name string.     */
//  char                    *   data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Save the input file name
//  data_p = in_dir_name_p;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
//  return( data_p );
    return( NULL );
}

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
encode(
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

#if ! STUB

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

        /********************************************************************
         *  Use one of the following encoders
         ********************************************************************/

        //  Did we get a new recipe to encode ?
        if ( rcb_p->recipe_p != NULL )
        {
            //  YES:    Are we encoding to a file ?
            if ( out_dir_name_p != NULL )
            {
                //  YES:    Go encode it.
//              encode_mmf( rcb_p );    //  @ToDo: 5 mmf encode
//              encode_mx2( rcb_p );    //  @ToDo: 5 mx2 encode
//              encode_mxp( rcb_p );    //  @ToDo: 5 mxp encode
//              encode_rxf( rcb_p );    //  @ToDo: 5 rxf encode
//              encode_ria( rcb_p );    //  @ToDo: 5 ria encode
//              encode_txt( rcb_p );    //  @ToDo: 5 ria encode
                encode_xml( rcb_p );    //  XML export format

                //  Put it in one of the DECODE queues
                queue_put_payload( export_tcb->queue_id, rcb_p  );
            }
            else
            {
                //  NO:     I guess we are exporting to the dBase
                dbase_insert( rcb_p );

                //  Kill the Recipe Control Block
                rcb_kill( rcb_p );
            }
        }

#else

        //  Kill the Recipe Control Block
        rcb_kill( rcb_p );

#endif

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/