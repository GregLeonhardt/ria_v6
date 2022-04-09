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
 *  for the RECIPE-TABLE in the RECIPE 'dbase' library.
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
 *  Create a new record in the RECIPE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     A pointer to the recipe buffer, NULL
 *                              if the recipe-id is not located.
 *
 *  @note
 *
 ****************************************************************************/

char    *
DBASE__recipe_build(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  buf_a_p         A data buffer                               */
    char                    *   buf_a_p;
    /**
     *  @param  buf_b_p         A data buffer                               */
    char                    *   buf_b_p;
    /**
     *  @param  buf_c_p         A data buffer                               */
    char                    *   buf_c_p;
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t          *   auip_p;
    /**
     *  @param  tmp_data_p      Pointer to a temporary data buffer          */
    char                    *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Code
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  <DESCRIPTION></DESCRIPTION>
    if ( rcb_p->recipe_p->description_p != NULL )
    {
        asprintf( &buf_a_p, "    <description>%s</description>\n",
                    rcb_p->recipe_p->description_p );
    }
    else
    {
        asprintf( &buf_a_p, "    <description></description>\n" );
    }

    //-----------------------------------------------------------------------
    //  <AUIP>

    //  Start tag
    asprintf( &buf_b_p, "    <auip-list>\n" );
    buf_c_p = text_join( buf_a_p, buf_b_p, false, false );
    free( buf_a_p ); free( buf_b_p ); buf_a_p = buf_c_p;

    //  AUIP list
    if ( list_query_count( rcb_p->recipe_p->ingredient_p ) > 0 )
    {
        for( auip_p = list_get_first( rcb_p->recipe_p->ingredient_p );
             auip_p != NULL;
             auip_p = list_get_next( rcb_p->recipe_p->ingredient_p, auip_p ) )
        {
            //-----------------------------------------------------------------
            //  <TYPE></TYPE>
            if ( auip_p->type_p != NULL )
            {
                asprintf( &buf_b_p, "        <type>?%s</type>\n", auip_p->type_p );
            }
            else
            {
                asprintf( &buf_b_p, "        <type>I</type>\n" );
            }
            buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
            free( buf_b_p ); buf_a_p = buf_c_p;

            //-----------------------------------------------------------------
            //  <AMOUNT></AMOUNT>
            if ( auip_p->amount_p != NULL )
            {
                asprintf( &buf_b_p, "        <amount>%s</amount>\n", auip_p->amount_p );
            }
            else
            {
                asprintf( &buf_b_p, "        <amount></amount>\n" );
            }
            buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
            free( buf_b_p ); buf_a_p = buf_c_p;

            //-----------------------------------------------------------------
            //  <UNIT></UNIT>
            if ( auip_p->unit_p != NULL )
            {
                asprintf( &buf_b_p, "        <unit>%s</unit>\n", auip_p->unit_p );
            }
            else
            {
                asprintf( &buf_b_p, "        <unit></unit>\n" );
            }
            buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
            free( buf_b_p ); buf_a_p = buf_c_p;

            //-----------------------------------------------------------------
            //  <INGREDIENT></INGREDIENT>
            if ( auip_p->ingredient_p != NULL )
            {
                asprintf( &buf_b_p, "        <ingredient>%s</ingredient>\n", auip_p->ingredient_p );
            }
            else
            {
                asprintf( &buf_b_p, "        <ingredient></ingredient>\n" );
            }
            buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
            free( buf_b_p ); buf_a_p = buf_c_p;

            //-----------------------------------------------------------------
            //  <PREPERATION></PREPERATION>
            if ( auip_p->preparation_p != NULL )
            {
                asprintf( &buf_b_p, "        <preperation>%s</preperation>\n", auip_p->preparation_p );
            }
            else
            {
                asprintf( &buf_b_p, "        <preperation></preperation>\n" );
            }
            buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
            free( buf_b_p ); buf_a_p = buf_c_p;

        }
    }

    //  Ending tag
    asprintf( &buf_b_p, "    </auip-list>\n" );
    buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
    free( buf_b_p ); buf_a_p = buf_c_p;

    //-----------------------------------------------------------------------
    //  <DIRECTIONS></DIRECTIONS>

    //  Start tag
    asprintf( &buf_b_p, "    <directions>\n" );
    buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
    free( buf_b_p ); buf_a_p = buf_c_p;

    if ( list_query_count( rcb_p->recipe_p->directions_p ) > 0 )
    {
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->directions_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->directions_p, tmp_data_p ) )
        {
            //  Is there something to write ?
            if ( text_is_blank_line( tmp_data_p ) != true )
            {
                //  YES:    Write the directions text.
                asprintf( &buf_b_p, "        %s\n", tmp_data_p );
                buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
                free( buf_b_p ); buf_a_p = buf_c_p;
            }
        }
    }

    //  Ending tag
    asprintf( &buf_b_p, "    </directions>\n" );
    buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
    free( buf_b_p ); buf_a_p = buf_c_p;
    //-----------------------------------------------------------------------
    //  <NOTES></NOTES>

    //  Start tag
    asprintf( &buf_b_p, "    <notes>\n" );
    buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
    free( buf_b_p ); buf_a_p = buf_c_p;

    if ( list_query_count( rcb_p->recipe_p->notes_p ) > 0 )
    {
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->notes_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->notes_p, tmp_data_p ) )
        {
            asprintf( &buf_b_p, "        %s\n", tmp_data_p );
            buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
            free( buf_b_p ); buf_a_p = buf_c_p;
        }
    }

    //  Ending tag
    asprintf( &buf_b_p, "    </notes>\n" );
    buf_c_p = text_join( buf_a_p, buf_b_p, true, false );
    free( buf_b_p ); buf_a_p = buf_c_p;

    //  Will the recipe fit in a BLOB ?
    if ( strlen( buf_c_p ) > 65536 )
    {
        //  YES:    OOPS the data won't fit into the allocated buffer.
        log_write( MID_FATAL, "DBASE__recipe",
                   "BUILD: At %d the recipe size too large to put into a 65536 "
                   "byte buffer.", strlen( buf_c_p ) );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( buf_c_p );
}

/****************************************************************************/
/**
 *  Create a new record in the RECIPE-TABLE
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
DBASE__recipe_exists(
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
     *  @param  sql_rc          Return code from MySQL function call.       */
    int                         sql_rc;
    /**
     *  @param  result          MySQL structure with the results of a query */
    MYSQL_RES               *   result;

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
              "SELECT * FROM recipe_table "
              "WHERE recipe_id = '%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the command.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__recipe",
            "EXISTS: RC:(%s) = %.128s \n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Read all rows returned
    result = mysql_store_result( con );

    //  Does the RECIP-ID already exist ?
    if (result != NULL)
    {
        //  YES:    Will be NOT be zero if the RECIPE-ID exists.
        sql_rc = mysql_num_rows( result );

        //  Does the RECIPE-ID already exist ?
        if( sql_rc != 0 )
        {
            //  YES:    It already exists.
            dbase_rc = true;
        }

        //  Free the result structure
        mysql_free_result( result );
    }

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__recipe",
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
 *  Create a new record in the RECIPE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     A pointer to the recipe buffer, NULL
 *                              if the recipe-id is not located.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__recipe_create(
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

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Build the recipe
    rcb_p->db_recipe_p = DBASE__recipe_build( rcb_p );

    //  Escape all special characters
    mysql_real_escape_string( con,
                              db_escaped,                       //  Destination
                              rcb_p->db_recipe_p,               //  Source
                              strlen( rcb_p->db_recipe_p ) );   //  Source length

    //  Build the MySQL command
    snprintf( db_command, sizeof( db_command ),
              "INSERT INTO recipe_table (recipe_id, recipe) VALUES( '%s', '%s' ); ",
              rcb_p->recipe_p->recipe_id_p, db_escaped );

    //  Now perform the insert.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__recipe",
            "CREATE: RC:(%s) = %.128s \n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__recipe",
                "CREATE: RC:(%d) = %s\n", sql_rc, mysql_error( con ) );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Read a record from the RECIPE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     A pointer to the recipe buffer, NULL
 *                              if the recipe-id is not located.
 *
 *  @note
 *      This function is unique to the normal CRUD functions as the
 *      size of the recipe-data row is unknown and may range from very
 *      small to very large.  After the size is determined storage for
 *      the recipe buffer will be allocated.
 *
 ****************************************************************************/

char *
DBASE__recipe_read(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  db_command      Where the MySQL command is built            */
    char                        db_command[ DB_COMMAND_L ];
    char                    *   recipe_buffer_p;
    unsigned long           *   recipe_buffer_l;
    /**
     *  @param  sql_rc          Return code from MySQL function call.       */
    int                         sql_rc;
    MYSQL_RES               *   result;
    MYSQL_ROW                   row;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the recipe buffer
    recipe_buffer_p = NULL;

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Build the MySQL command
    snprintf( db_command, sizeof( db_command),
              "SELECT recipe FROM recipe_table WHERE recipe_id='%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__recipe",
            "READ: RC:(%s) = %.128s\n", sql_rc?"FAIL":"PASS", db_command );
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
        //  YES:    Get the size of the buffer needed to read the recipe.
        recipe_buffer_l = mysql_fetch_lengths( result );

        //  Allocate storage for the recipe.
        recipe_buffer_p = mem_malloc( recipe_buffer_l[ 0 ] + 1 );

        //  Copy the recipe to the recipe buffer.
        memcpy( recipe_buffer_p, row[ 0 ], recipe_buffer_l[ 0 ] );

        mysql_free_result( result );

        //  Point to the next row
        sql_rc = mysql_next_result( con );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    return( recipe_buffer_p );
}

/****************************************************************************/
/**
 *  Update an existing record in the RECIPE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE if the record is found, else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__recipe_update(
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

    //  Clear out the MySQL command buffer.
    memset( db_command, '\0', sizeof( db_command ) );

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Build the recipe
    rcb_p->db_recipe_p = DBASE__recipe_build( rcb_p );

    //  Escape all special characters
    mysql_real_escape_string( con,
                              db_escaped,                       //  Destination
                              rcb_p->db_recipe_p,               //  Source
                              strlen( rcb_p->db_recipe_p ) );   //  Source length

    //  Build the MySQL command
    snprintf( db_command, sizeof( db_command ),
              "UPDATE recipe_table SET recipe = '%s' WHERE recipe-id = '%s' ); ",
              rcb_p->recipe_p->recipe_id_p,
              db_escaped );

    //  Now perform the insert.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__recipe",
            "UPDATE: RC:(%s) = %.128s\n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__recipe",
                "update: RC:(%d) = %s\n", sql_rc, mysql_error( con ) );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Delete a record from the RECIPE-TABLE
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return                     TRUE if the record is found, else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__recipe_delete(
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
//            DELETE FROM `table_name` [WHERE condition];
              "DELETE FROM  recipe_table WHERE recipe_id = '%s'; ",
              rcb_p->recipe_p->recipe_id_p );

    //  Now perform the insert.
    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__recipe",
            "DELETE: RC:(%s) = %.128s\n", sql_rc?"FAIL":"PASS", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        log_write( MID_FATAL, "DBASE__recipe",
                "DELETE: RC:(%d) = %s\n", sql_rc, mysql_error( con ) );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}
/****************************************************************************/