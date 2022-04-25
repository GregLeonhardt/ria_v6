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
 *  for the INFO-TABLE in the RECIPE 'dbase' library.
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
 *  Create a new record in the INFO-TABLE
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
DBASE__info_exists(
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
     *  @param  db_info_p      Structure to hold info data                */
    struct  db_info_t          db_info_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = false;

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Attempt to read the info from the dBase
    func_rc = DBASE__info_read( rcb_p, &db_info_p );

    //  Does the RECIPE-ID already exist ?
    if( func_rc == true )
    {
        //  YES:    It already exists.
        dbase_rc = true;

        //  Release the storage used to hold the info
                                              mem_free( db_info_p.recipe_id_p   );
        if ( db_info_p.author_p     != NULL ) mem_free( db_info_p.author_p );
        if ( db_info_p.serves_p     != NULL ) mem_free( db_info_p.serves_p );
        if ( db_info_p.makes_p      != NULL ) mem_free( db_info_p.makes_p );
        if ( db_info_p.makes_unit_p != NULL ) mem_free( db_info_p.makes_unit_p );
        if ( db_info_p.time_prep_p  != NULL ) mem_free( db_info_p.time_prep_p );
        if ( db_info_p.time_cook_p  != NULL ) mem_free( db_info_p.time_cook_p );
        if ( db_info_p.time_wait_p  != NULL ) mem_free( db_info_p.time_wait_p );
        if ( db_info_p.time_rest_p  != NULL ) mem_free( db_info_p.time_rest_p );
        if ( db_info_p.source_p     != NULL ) mem_free( db_info_p.source_p );
        if ( db_info_p.copyright_p  != NULL ) mem_free( db_info_p.copyright_p );
    }

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__info",
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
 *  Create a new record in the INFO-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE when the record is successfully
 *                              created, else FALSE.
 *  @note
 *
 ****************************************************************************/

int
DBASE__info_create(
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
     *  @param  count           Number of columns to insert                 */
    int                         count;
    /**
     *  @param  sql_rc          Return code from MySQL function call.       */
    int                         sql_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = false;
    count = 0;

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );
    memset( db_command_col, '\0', sizeof( db_command_col ) );
    memset( db_command_val, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Build the MySQL command
     ************************************************************************/

    //    RECIPE_ID
    if ( rcb_p->recipe_p->recipe_id_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "recipe_id", rcb_p->recipe_p->recipe_id_p );

        count += 1;
    }

    //    AUTHOR
    if ( rcb_p->recipe_p->author_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "author", rcb_p->recipe_p->author_p );

        count += 1;
    }

    //    SERVES
    if ( rcb_p->recipe_p->serves_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "serves", rcb_p->recipe_p->serves_p );

        count += 1;
    }

    //    MAKES
    if ( rcb_p->recipe_p->makes_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "makes", rcb_p->recipe_p->makes_p );

        count += 1;
    }

    //    MAKES_UNIT
    if ( rcb_p->recipe_p->makes_unit_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "makes_unit", rcb_p->recipe_p->makes_unit_p );

        count += 1;
    }

    //    TIME_PREP
    if ( rcb_p->recipe_p->time_prep_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "time_prep", rcb_p->recipe_p->time_prep_p );

        count += 1;
    }

    //    TIME_COOK
    if ( rcb_p->recipe_p->time_cook_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "time_cook", rcb_p->recipe_p->time_cook_p );

        count += 1;
    }

    //    TIME_WAIT
    if ( rcb_p->recipe_p->time_wait_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "time_wait", rcb_p->recipe_p->time_wait_p );

        count += 1;
    }

    //    TIME_REST
    if ( rcb_p->recipe_p->time_rest_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "time_rest", rcb_p->recipe_p->time_rest_p );

        count += 1;
    }

    //    SOURCE
    if ( rcb_p->recipe_p->source_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "source", rcb_p->recipe_p->source_p );

        count += 1;
    }

    //    COPYRIGHT
    if ( rcb_p->recipe_p->copyright_p != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "copyright", rcb_p->recipe_p->copyright_p );

        count += 1;
    }

    //    SKILL
    if ( rcb_p->recipe_p->skill_p > 0 )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "skill", rcb_p->recipe_p->skill_p );

        count += 1;
    }

    //    RATING
    if ( rcb_p->recipe_p->rating_p > 0 )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "rating", rcb_p->recipe_p->rating_p );

        count += 1;
    }

    //  Are there more than one columns ?
    if ( count > 1 )
    {
        //  YES:    Build the complete command
        snprintf( db_command, sizeof( db_command ),
                  "INSERT INTO info_table ( %s ) VALUES( %s );",
                  db_command_col, db_command_val );
    }
    else
    {
        //  NO:     Build the complete command
        snprintf( db_command, sizeof( db_command ),
                  "INSERT INTO info_table ( %s ) VALUE( %s );",
                  db_command_col, db_command_val );
    }

    /************************************************************************
     *  Perform the MySQL query and validate the success or failure.
     ************************************************************************/

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__info",
            "CREATE: RC:(%s) = %.768s \n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__info",
                "CREATE: RC:(%d) = %s\n", sql_rc, mysql_error( con ) );
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
/**
 *  Read a record from the INFO-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE when the record is successfully
 *                              read, else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__info_read(
    struct  rcb_t           *   rcb_p,
    struct  db_info_t      *   db_info_p
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

    //  Clear any old pointers in the info structure
    db_info_p->recipe_id_p  = NULL;
    db_info_p->author_p     = NULL;
    db_info_p->author_p     = NULL;
    db_info_p->serves_p     = NULL;
    db_info_p->makes_p      = NULL;
    db_info_p->makes_unit_p = NULL;
    db_info_p->time_prep_p  = NULL;
    db_info_p->time_cook_p  = NULL;
    db_info_p->time_wait_p  = NULL;
    db_info_p->time_rest_p  = NULL;
    db_info_p->source_p     = NULL;
    db_info_p->copyright_p  = NULL;

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Build the MySQL command
    snprintf( db_command, sizeof( db_command),
              "SELECT recipe_id, info FROM info_table WHERE recipe_id='%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__info",
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
        //  YES:    Read the INFO record
        db_info_p->recipe_id_p  = text_copy_to_new( row[  0 ] );
        db_info_p->author_p     = text_copy_to_new( row[  1 ] );
        db_info_p->serves_p     = text_copy_to_new( row[  2 ] );
        db_info_p->makes_p      = text_copy_to_new( row[  2 ] );
        db_info_p->makes_unit_p = text_copy_to_new( row[  2 ] );
        db_info_p->time_prep_p  = text_copy_to_new( row[  3 ] );
        db_info_p->time_cook_p  = text_copy_to_new( row[  4 ] );
        db_info_p->time_wait_p  = text_copy_to_new( row[  5 ] );
        db_info_p->time_rest_p  = text_copy_to_new( row[  6 ] );
        db_info_p->source_p     = text_copy_to_new( row[  7 ] );
        db_info_p->copyright_p  = text_copy_to_new( row[  8 ] );
        //  @ToDo:  3   The next two are 'INT' not strings.
//      db_info_p->skill_p      = text_copy_to_new( row[  9 ] );
//      db_info_p->rating_p     = text_copy_to_new( row[ 10 ] );

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
 *  Update an existing record in the INFO-TABLE
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
DBASE__info_update(
    struct  rcb_t           *   rcb_p
    )
{
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
    dbase_rc = DBASE__info_delete( rcb_p );

    //  Was the delete successful ?
    if ( dbase_rc == true )
    {
        //  YES:    CREATE the new record
        dbase_rc = DBASE__info_create( rcb_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Delete a record from the INFO-TABLE
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
DBASE__info_delete(
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
              "DELETE FROM info_table WHERE recipe_id = '%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__info",
            "DELETE: RC:(%s) = %.768s\n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__info",
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