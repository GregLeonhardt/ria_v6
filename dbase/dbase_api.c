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
 *  library components of the 'dbase' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define _GNU_SOURCE

#define ALLOC_DBASE          ( "ALLOCATE STORAGE FOR DBASE" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <stdlib.h>             //  ANSI standard library.
#include <string.h>             //  Functions for managing strings
#include <unistd.h>             //  Access to the POSIX operating system API
#include <mysql/mysql.h>        //  MySQL API
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include "global.h"             //  Global stuff for this application
#include "libtools_api.h"       //  My Tools Library
                                //*******************************************
#include "rcb_api.h"            //  API for all rcb_*               PUBLIC
                                //*******************************************
#include "dbase_api.h"          //  API for all dbase_*             PUBLIC
#include "dbase_lib.h"          //  API for all DBASE__*            PRIVATE
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
/**
 * @param con               MySQL connector                                 */
MYSQL                       *   con;
//----------------------------------------------------------------------------

/****************************************************************************
 * Public API Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  .
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return void                Upon successful completion TRUE is returned
 *                              else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_init(
    void
    )
{
    /**
     *  @param  decode_rc       Return code from this function              */
    int                             dbase_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

     // Initialize the return code
     dbase_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

     //  Open the database
     DBASE__open( );


    /************************************************************************
     *  Function Exit
     ************************************************************************/

     // DONE!
     return( dbase_rc );
}

/****************************************************************************/
/**
 *  .Insert a new recipe into the database.
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     Upon successful completion TRUE is returned
 *                              else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_insert(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  decode_rc       Return code from this function              */
    int                             dbase_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

     // Initialize the return code
     dbase_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  Three things that are needed to have a real recipe
    //  1)  Name
    //  2)  Ingredients
    //  3)  Directions
    if (    ( rcb_p->recipe_p->name_p != NULL )
         && ( list_query_count( rcb_p->recipe_p->ingredient_p ) > 0 )
         && ( list_query_count( rcb_p->recipe_p->directions_p ) > 0 ) )
    {
        //  Display progress.
        log_write( MID_INFO, "dbase_insert",
                  "%s - %s\n",
                  rcb_p->recipe_p->recipe_id_p,
                  rcb_p->recipe_p->name_p );

        //-----------------------------------------------------------------------
        //  INSERT OR DISCARD THE NEW RECIPE
        if ( dbase_rc == true )
        {
            //  YES:    Should we discard the new recipe
            if ( DBASE__discard_recipe( rcb_p ) == false )
            {
                //  NO:     Insert it

                //-----------------------------------------------------------------------
                //  TITLE_TABLE
                if ( dbase_rc == true )
                {
                    //  YES:    Create the title record
                    dbase_rc = DBASE__title_create( rcb_p );
                }

                //-----------------------------------------------------------------------
                //  RECIPE_TABLE
                if ( dbase_rc == true )
                {
                    //  YES:    Create the recipe record
                    dbase_rc = DBASE__recipe_create( rcb_p );

                    //  Was the create a success ?
                    if ( dbase_rc != true )
                    {
                        //  NO:     We need to delete the new records
                        DBASE__title_delete( rcb_p );
                    }
                }

                //-----------------------------------------------------------------------
                //  INFO_TABLE
                if ( dbase_rc == true )
                {
                    dbase_rc = DBASE__info_create( rcb_p );
                    //  Was the create a success ?
                    if ( dbase_rc != true )
                    {
                        //  NO:     We need to delete the new records
                        DBASE__title_delete( rcb_p );
                        DBASE__recipe_delete( rcb_p );
                    }
                }

                //-----------------------------------------------------------------------
                //  CUISINE_TABLE
                if ( dbase_rc == true )
                {
                    dbase_rc = DBASE__cuisine_create( rcb_p );
                    //  Was the create a success ?
                    if ( dbase_rc != true )
                    {
                        //  NO:     We need to delete the new records
                        DBASE__title_delete( rcb_p );
                        DBASE__recipe_delete( rcb_p );
                        DBASE__info_delete( rcb_p );
                    }
                }

                //-----------------------------------------------------------------------
                //  SOURCE_TABLE
                if ( dbase_rc == true )
                {
                    dbase_rc = DBASE__source_create( rcb_p );

                    //  Was the create a success ?
                    if ( dbase_rc != true )
                    {
                        //  NO:     We need to delete the new records
                        DBASE__title_delete( rcb_p );
                        DBASE__recipe_delete( rcb_p );
                        DBASE__cuisine_delete( rcb_p );
                        DBASE__info_delete( rcb_p );
                    }
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

     // DONE!
     return( dbase_rc );
}
/****************************************************************************/