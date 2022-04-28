/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains the CRUD (Create, Read, Update, and Delete) functions
 *  for the APPLIANCE-TABLE in the RECIPE 'dbase' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define _GNU_SOURCE             // See feature_test_macros(7)

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <ctype.h>              //  Determine the type contained
#include <mysql/mysql.h>        //  MySQL API
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
#include "recipe_api.h"         //  API for all recipe_*            PUBLIC
                                //*******************************************
#include "dbase_api.h"          //  API for all dbase_*             PUBLIC
#include "dbase_lib.h"          //  API for all DBASE__*            PRIVATE
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Definitions local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Structures local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Storage Allocation local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Create a new record in the APPLIANCE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE when the RECIPE-ID already exists, else
 *                              FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__appliance_exists(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  dbase_rc        Function return code                        */
    int                         dbase_rc;
    /**
     *  @param  func_rc         Called function return code                 */
    int                         func_rc;
    /**
     *  @param  db_appliance_p    Structure to hold appliance data              */
    struct  db_appliance_t        db_appliance;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = false;

    //  Zero out and initialize the appliance data structure
    memset( &db_appliance, 0x00, sizeof( struct  db_appliance_t ) );
    db_appliance.appliance_list_p = list_new( );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Attempt to read the appliance from the dBase
    func_rc = DBASE__appliance_read( rcb_p, &db_appliance );

    //  Does the RECIPE-ID already exist ?
    if( func_rc == true )
    {
        //  YES:    It already exists.
        dbase_rc = true;

        //  Release the storage used to hold the appliance
        mem_free( db_appliance.recipe_id_p   );

        //--------------------------------------------------------------------
        if ( list_query_count( db_appliance.appliance_list_p ) > 0 )
        {
            while( ( data_p = list_get_first( db_appliance. appliance_list_p) ) != NULL )
            {
                list_delete_payload( db_appliance.appliance_list_p, data_p );
                mem_free( data_p );
            }
        }
        if ( list_kill( db_appliance.appliance_list_p ) != true )
        {
            log_write( MID_FATAL, "DBASE__appliance",
                          "list_kill( recipe_p->appliance ) failed\n" );
        }
        //--------------------------------------------------------------------

    }

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__appliance",
            "EXISTS: %s\n", dbase_rc == 0?"FALSE":"TRUE" );
#endif

    /************************************************************************
     *  Function Exit
     ************************************************************************/
    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Create a new record in the APPLIANCE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE when the record is successfully
 *                              created, else FALSE.
 *  @note
 *
 ****************************************************************************/

int
DBASE__appliance_create(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  dbase_rc        Function return code                        */
    int                         dbase_rc;
    /**
     *  @param  db_command      Where the MySQL command is built            */
    char                        db_command[ DB_COMMAND_L + 256 ];
    char                        db_command_col[ DB_COMMAND_L / 2 ];
    char                        db_command_val[ DB_COMMAND_L / 2 ];
    /**
     *  @param  sql_rc          Return code from MySQL function call.       */
    int                         sql_rc;
    /**
     *  @param  tmp_data_p      Pointer to a temporary data buffer          */
    char                    *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = true;

    /************************************************************************
     *  Build the MySQL command
     ************************************************************************/

    if ( list_query_count( rcb_p->recipe_p->appliance_p ) != 0 )
    {
        char * db_cmd_p = db_command;
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->appliance_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->appliance_p, tmp_data_p ) )
        {

            //  Clear out the MySQL command buffer.
            memset( db_command, '\0', sizeof( db_command ) );
            memset( db_command_col, '\0', sizeof( db_command_col ) );
            memset( db_command_val, '\0', sizeof( db_command ) );

            //    RECIPE_ID
            if ( rcb_p->recipe_p->recipe_id_p != NULL )
            {
                DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                                    db_command_val, sizeof( db_command_val ),
                                    "recipe_id", rcb_p->recipe_p->recipe_id_p );
            }

            //    APPLIANCE
            if ( rcb_p->recipe_p->name_p != NULL )
            {
                DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                                    db_command_val, sizeof( db_command_val ),
                                    "name", tmp_data_p );
            }
            //  NO:     Build the complete command
            snprintf( db_command, sizeof( db_command ),
                      "INSERT INTO appliance_table ( %s ) VALUES( %s );",
                      db_command_col, db_command_val );

            /****************************************************************
             *  Perform the MySQL query and validate the success or failure.
             ****************************************************************/

            //  Now perform the command.
            sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
            //  Log the dBase access command
            log_write( MID_LOGONLY, "DBASE__appliance",
                    "CREATE: RC:(%s) = %.768s \n", sql_rc?"FAIL":"PASS", db_command );
#endif

            //  Was the command successful ?
            if ( sql_rc != 0 )
            {
                //  The database creation filed.
                log_write( MID_FATAL, "DBASE__appliance",
                        "CREATE: RC:(%d) = %s\n", sql_rc, mysql_error( con ) );

                //  Set the return code to success
                dbase_rc = false;
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Read a record from the APPLIANCE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *  @param  db_appliance_p    *   Structure to hold appliance data
 *
 *  @return                     TRUE when the record is successfully
 *                              read, else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__appliance_read(
    struct  rcb_t           *   rcb_p,
    struct  db_appliance_t    *   db_appliance_p
    )
{
    /**
     *  @param  dbase_rc        Function return code                        */
    int                         dbase_rc;
    /**
     *  @param  db_command      Where the MySQL command is built            */
    char                        db_command[ DB_COMMAND_L ];
    /**
     *  @param  sql_rc          Return code from MySQL function call.       */
    int                         sql_rc;
    /**
     *  @param  result          .                                           */
    MYSQL_RES               *   result;
    /**
     *  @param  row             Returned database data                      */
    MYSQL_ROW                   row;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = false;

    //  Clear any old pointers in the appliance structure
    db_appliance_p->recipe_id_p = NULL;

    //  Verify the appliance list is empty
    if ( list_query_count( rcb_p->recipe_p->appliance_p ) != 0 )
    {
        //  The list for appliance MUST be empty when this function is called.
        log_write( MID_LOGONLY, "DBASE__appliance",
                   "READ: The appliance list is NOT empty ( %d )\n",
                   list_query_count( rcb_p->recipe_p->appliance_p ) );
    }

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Build the MySQL command
    snprintf( db_command, sizeof( db_command),
              "SELECT recipe_id, appliance FROM appliance_table WHERE recipe_id='%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__appliance",
            "READ: RC:(%s) = %.768s\n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database access filed.
        MYQL__access_error( db_command );
    }

    //  Read all rows returned
    result = mysql_store_result( con );

    if (result == NULL)
    {
        //  Failed to locate the records
        MYQL__access_error( db_command );
    }

    //  Access the next row of data
    row = mysql_fetch_row( result );

    //  Is there a row present ?
    if ( row != NULL )
    {
        //  YES:    Read the RECIPE-ID
        db_appliance_p->recipe_id_p   = text_copy_to_new( row[  0 ] );

        //  Read the APPLIANCE-DATA
        list_put_last( db_appliance_p->appliance_list_p, text_copy_to_new( row[  1 ] ) );

        //  Release the results
        mysql_free_result( result );

        //  Set the return code for "record exists".
        dbase_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Update an existing record in the APPLIANCE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE when the record is successfully
 *                              updated (replaced), else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__appliance_update(
    struct  rcb_t           *   rcb_p
    )
{
#if 0
    /**
     *  @param  dbase_rc        Function return code                        */
    int                         dbase_rc;
    /**
     *  @param  db_command      Where the MySQL command is built            */
    char                        db_command[ DB_COMMAND_L + 256 ];
    /**
     *  @param  db_escaped      Where the MySQL command is built            */
    char                        db_escaped[ DB_COMMAND_L ];
    /**
     *  @param  sql_rc          Return code from MySQL function call.       */
    int                         sql_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = false;

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Delete the existing record
    dbase_rc = DBASE__appliance_delete( rcb_p );

    //  Was the delete successful ?
    if ( dbase_rc == true )
    {
        //  YES:    CREATE the new record
        dbase_rc = DBASE__appliance_create( rcb_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
#else
    return( false );
#endif
}

/****************************************************************************/
/**
 *  Delete a record from the APPLIANCE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE when the record is successfully
 *                              deleted, else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__appliance_delete(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     * @param   dbase_rc        //  Function return code
     */
    int                         dbase_rc;
    /**
     *  @param  db_command      Where the MySQL command is built            */
    char                        db_command[ DB_COMMAND_L ];
    /**
     *  @param  sql_rc          Return code from MySQL function call.       */
    int                         sql_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = false;

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Build the MySQL command
    snprintf( db_command, sizeof( db_command ),
              "DELETE FROM appliance_table WHERE recipe_id = '%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__appliance",
            "DELETE: RC:(%s) = %.768s\n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__appliance",
                "DELETE: RC:(%d) = %s\n", sql_rc, mysql_error( con ) );
    }
    else
    {
        //  Set the return code to success
        dbase_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}
/****************************************************************************/