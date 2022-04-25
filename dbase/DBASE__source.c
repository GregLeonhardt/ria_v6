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
 *  for the SOURCE-TABLE in the RECIPE 'dbase' library.
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
#include "email_api.h"          //  API for all email_*             PUBLIC
#include "decode_api.h"         //  API for all decode_*            PUBLIC
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
 *  Create a new record in the SOURCE-TABLE
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
DBASE__source_exists(
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
     *  @param  db_source_p     Structure to hold source data               */
    struct  db_source_t         db_source_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = false;

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Attempt to read the source from the dBase
    func_rc = DBASE__source_read( rcb_p, &db_source_p );

    //  Does the RECIPE-ID already exist ?
    if( func_rc == true )
    {
        //  YES:    It already exists.
        dbase_rc = true;

        //  Release the storage used to hold the source
        //  Release the storage used to hold the info
                                                     mem_free( db_source_p.recipe_id_p   );
        if ( db_source_p.recipe_id_p       != NULL ) mem_free( db_source_p.recipe_id_p );
        if ( db_source_p.src_format_p      != NULL ) mem_free( db_source_p.src_format_p );
        if ( db_source_p.file_name_p       != NULL ) mem_free( db_source_p.file_name_p );
        if ( db_source_p.file_size_p       != NULL ) mem_free( db_source_p.file_size_p );
        if ( db_source_p.file_date_time_p  != NULL ) mem_free( db_source_p.file_date_time_p );
        if ( db_source_p.group_name_p      != NULL ) mem_free( db_source_p.group_name_p );
        if ( db_source_p.group_subject_p   != NULL ) mem_free( db_source_p.group_subject_p );
        if ( db_source_p.group_date_time_p != NULL ) mem_free( db_source_p.group_date_time_p );
        if ( db_source_p.email_name_p      != NULL ) mem_free( db_source_p.email_name_p );
        if ( db_source_p.email_subject_p   != NULL ) mem_free( db_source_p.email_subject_p );
        if ( db_source_p.email_date_time_p != NULL ) mem_free( db_source_p.email_date_time_p );
    }

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__source",
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
 *  Create a new record in the SOURCE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE when the record is successfully
 *                              created, else FALSE.
 *  @note
 *
 ****************************************************************************/

int
DBASE__source_create(
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

#if 0
    //    SOURCE_FORMAT
    if ( rcb_p->recipe_format != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "source_format", rcb_p->recipe_format );

        count += 1;
    }
#endif

    //    FILE_NAME
    if ( rcb_p->file_path != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "file_name", rcb_p->file_path );

        count += 1;
    }

    //    FILE_SIZE
    if ( rcb_p->file_info_p->file_size != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "file_size", rcb_p->file_info_p->file_size );

        count += 1;
    }

    //    FILE_DATE_TIME
    if ( rcb_p->file_info_p->date_time != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "file_date_time",
                            rcb_p->file_info_p->date_time );

        count += 1;
    }

    //    GROUP_NAME
    if ( rcb_p->email_info_p->g_from != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "group_name", rcb_p->email_info_p->g_from );

        count += 1;
    }

    //    GROUP_SUBJECT
    if ( rcb_p->email_info_p->g_subject != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "group_subject", rcb_p->email_info_p->g_subject );

        count += 1;
    }

    //    GROUP_DATE_TIME
    if (    ( rcb_p->email_info_p->g_datetime != NULL )
         && ( strlen( rcb_p->email_info_p->g_datetime ) > 8 ) )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "group_date_time",
                            rcb_p->email_info_p->g_datetime );

        count += 1;
    }

    //    EMAIL_NAME
    if ( rcb_p->email_info_p->e_from != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "email_name", rcb_p->email_info_p->e_from );

        count += 1;
    }

    //    EMAIL_SUBJECT
    if ( rcb_p->email_info_p->e_subject != NULL )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "email_subject", rcb_p->email_info_p->e_subject );

        count += 1;
    }

    //    EMAIL_DATE_TIME
    if (    ( rcb_p->email_info_p->e_datetime != NULL )
         && ( strlen( rcb_p->email_info_p->e_datetime ) > 8 ) )
    {
        DBASE__add_col_val( db_command_col, sizeof( db_command_col ),
                            db_command_val, sizeof( db_command_val ),
                            "email_date_time",
                            rcb_p->email_info_p->e_datetime );

        count += 1;
    }

    //  Are there more than one columns ?
    if ( count > 1 )
    {
        //  YES:    Build the complete command
        snprintf( db_command, sizeof( db_command ),
                  "INSERT INTO source_table ( %s ) VALUES( %s );",
                  db_command_col, db_command_val );
    }
    else
    {
        //  NO:     Build the complete command
        snprintf( db_command, sizeof( db_command ),
                  "INSERT INTO source_table ( %s ) VALUE( %s );",
                  db_command_col, db_command_val );
    }

    /************************************************************************
     *  Perform the MySQL query and validate the success or failure.
     ************************************************************************/

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__source",
            "CREATE: RC:(%s) = %.768s \n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__source",
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
 *  Read a record from the SOURCE-TABLE
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
DBASE__source_read(
    struct  rcb_t           *   rcb_p,
    struct  db_source_t     *   db_source_p
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

    //  Clear any old pointers in the source structure
    db_source_p->recipe_id_p       = NULL;
    db_source_p->recipe_id_p       = NULL;
    db_source_p->src_format_p      = NULL;
    db_source_p->file_name_p       = NULL;
    db_source_p->file_size_p       = NULL;
    db_source_p->file_date_time_p  = NULL;
    db_source_p->group_name_p      = NULL;
    db_source_p->group_subject_p   = NULL;
    db_source_p->group_date_time_p = NULL;
    db_source_p->email_name_p      = NULL;
    db_source_p->email_subject_p   = NULL;
    db_source_p->email_date_time_p = NULL;

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Build the MySQL command
    snprintf( db_command, sizeof( db_command),
              "SELECT recipe_id, format, file_name, file_size, file_date_time, "
                      "group_name, group_subject, group_date_time, "
                      "email_name, email_subject, email_date_time "
                      " FROM source_table WHERE recipe_id='%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__source",
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
        if ( row[  0 ] != NULL )
            db_source_p->recipe_id_p        = text_copy_to_new( row[  0 ] );
        if ( row[  1 ] != NULL )
            db_source_p->src_format_p       = text_copy_to_new( row[  1 ] );
        if ( row[  2 ] != NULL )
            db_source_p->file_name_p        = text_copy_to_new( row[  2 ] );
        if ( row[  3 ] != NULL )
            db_source_p->file_size_p        = text_copy_to_new( row[  3 ] );
        if ( row[  4 ] != NULL )
            db_source_p->file_date_time_p   = text_copy_to_new( row[  4 ] );
        if ( row[  5 ] != NULL )
            db_source_p->group_name_p       = text_copy_to_new( row[  5 ] );
        if ( row[  6 ] != NULL )
            db_source_p->group_subject_p    = text_copy_to_new( row[  6 ] );
        if ( row[  7 ] != NULL )
            db_source_p->group_date_time_p  = text_copy_to_new( row[  7 ] );
        if ( row[  8 ] != NULL )
            db_source_p->email_name_p       = text_copy_to_new( row[  8 ] );
        if ( row[  9 ] != NULL )
            db_source_p->email_subject_p    = text_copy_to_new( row[  9 ] );
        if ( row[ 10 ] != NULL )
            db_source_p->email_date_time_p  = text_copy_to_new( row[ 10 ] );

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
 *  Update an existing record in the SOURCE-TABLE
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
DBASE__source_update(
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
    dbase_rc = DBASE__source_delete( rcb_p );

    //  Was the delete successful ?
    if ( dbase_rc == true )
    {
        //  YES:    CREATE the new record
        dbase_rc = DBASE__source_create( rcb_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Delete a record from the SOURCE-TABLE
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
DBASE__source_delete(
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
              "DELETE FROM source_table WHERE recipe_id = '%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_LOGONLY, "DBASE__source",
            "DELETE: RC:(%s) = %.768s\n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__source",
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