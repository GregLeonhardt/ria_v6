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
 *  library components of the 'rcb' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_RCB          ( "ALLOCATE STORAGE FOR RCB" )

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
#include "rcb_api.h"            //  API for all rcb_*               PUBLIC
#include "rcb_lib.h"            //  API for all RCB__*              PRIVATE
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
 *  @param  @param  rcb_p       Pointer to a Recipe Control Block
 *
 *  @return void                No information is returned
 *
 *  @note
 *
 ****************************************************************************/

void
rcb_kill(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   data_p;


    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Clear the FILE-INFORMATION structure pointer ?
    if ( rcb_p->file_info_p != NULL )
    {
        //  Free it
        rcb_p->file_info_p = NULL;
    }
    //------------------------------------------------------------------------
    //  Clear the display name string
    memset( rcb_p->file_path, '\0', sizeof( rcb_p->file_path ) );
    //------------------------------------------------------------------------
    //  Reset the destination thread id.
    rcb_p->dst_thread = DST_INVALID;
    //------------------------------------------------------------------------
    //  Clear all imported data
    if ( rcb_p->import_list_p != NULL )
    {
        if ( list_query_count( rcb_p->import_list_p ) > 0 )
        {
            while( ( data_p = list_get_first( rcb_p->import_list_p ) ) != NULL )
            {
                list_delete( rcb_p->import_list_p, data_p );
                mem_free( data_p );
            }
        }
        list_kill( rcb_p->import_list_p );
        rcb_p->import_list_p = NULL;
    }
    //------------------------------------------------------------------------
    //  Is there an open file ?
    if ( rcb_p->file_p != 0 )
    {
        //  YES:    Close it.
        file_close( rcb_p->file_p ); rcb_p->file_p = 0;
    }
    //------------------------------------------------------------------------
    //  Delete the recipe structure
    recipe_kill( rcb_p->recipe_p );
    //------------------------------------------------------------------------
    //  Reset the destination thread id.
    rcb_p->recipe_format = RECIPE_FORMAT_NONE;
    //------------------------------------------------------------------------
    //  Reset the destination thread id.
    if ( rcb_p->email_info_p != NULL )
    {
        mem_free( rcb_p->email_info_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Initialize the Translations tables.
 *
 *  @param  tcb_p               Pointer to the current Thread Control Block
 *  @param  rcb_p               Pointer to the current Recipe Control Block
 *  @param  recipe_format       The recipe format for this recipe.
 *
 *  @return new_rcb_p           Pointer to a new Recipe Control Block
 *
 *  @note
 *
 ****************************************************************************/

struct  rcb_t   *
rcb_new(
    struct  tcb_t           *   tcb_p,
    struct  rcb_t           *   rcb_p,
    enum    recipe_format_e     recipe_format
    )
{
    /**
     *  @param  new_rcb_p       Pointer to a Recipe Control Block           */
    struct  rcb_t           *   new_rcb_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  YES:    Allocate a new recipe control block
    new_rcb_p = mem_malloc( sizeof( struct rcb_t ) );

    //  Set the initial thread control block
    new_rcb_p->tcb_p = tcb_p;

    //  Is this going to be a split of an existing Recipe Control Block
    if ( rcb_p != NULL )
    {
        //  YES:    Copy the file stats to the new RCB
        new_rcb_p->file_info_p = rcb_p->file_info_p;

        //  Copy the display file name
        memcpy( new_rcb_p->file_path,
                rcb_p->file_path,
                sizeof( rcb_p->file_path ) );
    }

    //  Create a new list
    new_rcb_p->import_list_p = list_new( );

    //  Set the recipe format
    new_rcb_p->recipe_format = recipe_format;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( new_rcb_p );
}
/****************************************************************************/