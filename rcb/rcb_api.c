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
#include "email_api.h"          //  API for all email_*             PUBLIC
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

    //  Does a file information structure exist ?
    if ( rcb_p->file_info_p != NULL )
    {
        //  YES:    Free it
        mem_free( rcb_p->file_info_p );
        rcb_p->file_info_p = NULL;
    }
    //------------------------------------------------------------------------
    //  Clear the display name string
    memset( rcb_p->file_path, '\0', sizeof( rcb_p->file_path ) );
    //------------------------------------------------------------------------
    //  Reset the destination thread id.
    rcb_p->dst_thread = DST_INVALID;
    //------------------------------------------------------------------------
    //  Clear all import data
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
    //  Clear all export data
    if ( rcb_p->export_list_p != NULL )
    {
        if ( list_query_count( rcb_p->export_list_p ) > 0 )
        {
            while( ( data_p = list_get_first( rcb_p->export_list_p ) ) != NULL )
            {
                list_delete( rcb_p->export_list_p, data_p );
                mem_free( data_p );
            }
        }
        list_kill( rcb_p->export_list_p );
        rcb_p->export_list_p = NULL;
    }
    //------------------------------------------------------------------------
    //  Is there an open file ?
    if ( rcb_p->file_p != 0 )
    {
        //  YES:    Close it.
        file_close( rcb_p->file_p ); rcb_p->file_p = 0;
    }
    //------------------------------------------------------------------------
    //  Does an recipe structure exist ?
    if ( rcb_p->recipe_p != NULL )
    {
        //  YES:    Free it
        recipe_kill( rcb_p->recipe_p );
    }
    //------------------------------------------------------------------------
    //  Reset the destination thread id.
    rcb_p->recipe_format = RECIPE_FORMAT_NONE;
    //------------------------------------------------------------------------
    //  Does an e-Mail information structure exist ?
    if ( rcb_p->email_info_p != NULL )
    {
        //  YES:    Free it
        mem_free( rcb_p->email_info_p );
        rcb_p->email_info_p = NULL;
    }
    //------------------------------------------------------------------------
    //  Release the structure
    mem_free( rcb_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Initialize the Translations tables.
 *
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
    struct  rcb_t           *   old_rcb_p,
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

    //  Is this going to be a split of an existing Recipe Control Block
    if ( old_rcb_p != NULL )
    {
        //  YES:    Copy Thread Control Block pointer
        new_rcb_p->tcb_p = old_rcb_p->tcb_p;

        //  Copy the file stats to the new RCB
        new_rcb_p->file_info_p = mem_malloc( sizeof( struct file_info_t ) );
        memcpy( new_rcb_p->file_info_p,
                old_rcb_p->file_info_p,
                sizeof( struct file_info_t ) );

        //  Copy the display file name
        memcpy( new_rcb_p->file_path,
                old_rcb_p->file_path,
                sizeof( old_rcb_p->file_path ) );

        //  Copy the e-Mail information to the new RCB
        if ( new_rcb_p->email_info_p != NULL )
        {
            new_rcb_p->email_info_p = mem_malloc( sizeof( struct email_info_t ) );
            memcpy( new_rcb_p->email_info_p,
                    old_rcb_p->email_info_p,
                    sizeof( struct email_info_t ) );
        }
    }

    //  No destination thread yet.
    new_rcb_p->dst_thread = DST_INVALID;

    //  Create a new import list
    new_rcb_p->import_list_p = list_new( );

    //  Create a new export list
    new_rcb_p->export_list_p = list_new( );

    //  There isn't an open file
    new_rcb_p->file_p = NULL;

    //  Set the recipe format
    new_rcb_p->recipe_format = recipe_format;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( new_rcb_p );
}
/****************************************************************************/