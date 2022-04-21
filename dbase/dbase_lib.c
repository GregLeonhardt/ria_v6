/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'dbase' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


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
 *  Display a database access error.
 *
 *  @param  sql                 Command that failed
 *
 *  @return                     No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
MYQL__access_error(
    char                    *   sql_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Report the failure
    log_write( MID_WARNING, "DataBase",
               "SQL:          [%s\n]", sql_p );
    log_write( MID_WARNING, "DataBase",
               "Error code:   %d\n", mysql_errno( con ) );
    log_write( MID_WARNING, "DataBase",
               "Error text:   %s\n", mysql_error( con ) );
    mysql_close( con );
    exit( 1 );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  We will never get here.  All SQL errors are fatal and need to be
    //  resolved before continuing.
}

/****************************************************************************/
/**
 *  Read a record from the ADDRESS TABLE
 *
 *  @param  put_address_p       Pointer to the search address string
 *  @param  get_address_p       Pointer to the found address string
 *
 *  @return                     TRUE if the record is found, else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

#define WALLET_ADDRESS_SIZE 1

int
DBASE__address_get(
    char                    *   put_address_p,
    char                    *   get_address_p
    )
{
    char                        db_command[ DB_COMMAND_L ];
    int                         sql_rc;
    int                         found;
    MYSQL_RES               *   result;
    MYSQL_ROW                   row;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  NO:     Add it
    snprintf( db_command, sizeof( db_command),
              "SELECT address FROM address_table WHERE address='%s'; ",
              put_address_p );

    sql_rc = mysql_query( con, db_command );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__address_get",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", db_command );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database access filed.
        MYQL__access_error( db_command );
    }

    //  Read all rows returned
    do {
        result = mysql_store_result( con );

        if (result == NULL)
        {
            //  Failed to locate any records
            MYQL__access_error( db_command );
        }

        //  Access the next row of data
        row = mysql_fetch_row( result );

        //  Is there a row present ?
        if ( row != NULL )
        {
            //  YES:
            memset( get_address_p, '\0', WALLET_ADDRESS_SIZE );
            memcpy( get_address_p, row[ 0 ], WALLET_ADDRESS_SIZE );

            //  Set the found flag TRUE.
            found = true;

            mysql_free_result( result );

            //  Point to the next row
            sql_rc = mysql_next_result( con );
        }
        else
        {
            //  NO:
            found = false;
            mysql_free_result( result );

            break;
        }

    } while( sql_rc == 0 );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    return( found );
}

/****************************************************************************/
/**
 *  Open the database.  If required create the database and/or tables.
 *
 *  @param
 *
 *  @return                     TRUE for success. Any other value is an error.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__open(
    )
{
    int                             sql_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Establish a connection to MariaDB and create the the database
     *  if it doesn't already exist.
     ************************************************************************/

    //  Prepare and initialize the connection structure.
    con = mysql_init( NULL );

    //  Was the connection structure initialization successful ?
    if ( con == NULL )
    {
        //  NO:     Write an error message and exit
        MYQL__access_error( "mysql_init" );
    }

    //  Connect to the database
    if ( mysql_real_connect( con,           //  Connection structure
                             "localhost",   //  Location
                             "root",        //  User
                             "1YW9bjnf",    //  User-Password
                             NULL, 0,       //  database/port
                             NULL, 0 )      //  socket/flags
            == NULL )
    {
        //  The connection failed.
        MYQL__access_error( "mysql_real_connect" );
    }

    //  Will we be deleting the existing database ?
    if ( delete_flag == true )
    {
        //  YES:
        sql_rc = mysql_query( con, "DROP DATABASE IF EXISTS recipes;");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "DROP DATABASE IF EXISTS recipes;" );
#endif

        //  Was the command successful ?
        if ( sql_rc != 0 )
        {
            //  The database creation filed.
            MYQL__access_error( "DROP DATABASE IF EXISTS recipes;" );
        }
    }

    //  Create a new database
    sql_rc = mysql_query( con, "CREATE DATABASE IF NOT EXISTS recipes;" );

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE DATABASE IF NOT EXISTS recipes;" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database creation filed.
        MYQL__access_error( "CREATE DATABASE IF NOT EXISTS recipes;" );
    }

    //  THe database exists so close the connection
    mysql_close( con );

    /************************************************************************
     *  Establish a connection to the database.
     ************************************************************************/

    //  Prepare and initialize the connection structure.
    con = mysql_init( NULL );

    //  Was the connection structure initialization successful ?
    if ( con == NULL )
    {
        //  NO:     Write an error message and exit
        MYQL__access_error( "mysql_init" );
    }

    //  Now open a connection to the test database
    if ( mysql_real_connect( con,           //  Connection structure
                             "localhost",   //  Location
                             "root",        //  User
                             "1YW9bjnf",    //  User-Password
                             "recipes", 0,  //  database/port
                             NULL, 0 )      //  socket/flags
            == NULL )
    {
        //  The connection failed.
        MYQL__access_error( "mysql_real_connect" );
    }

    /************************************************************************
     *  Create the recipe table (If it doesn't already exist).
     ************************************************************************/

    //  TINYINT         (1)     A very small integer
    //  SMALLINT        (2)     A small integer
    //  MEDIUMINT       (3)     A medium-sized integer
    //  INT             (4)     A standard integer
    //  BIGINT          (8)     A large integer
    //  DECIMAL                 A fixed-point number
    //  FLOAT                   A single-precision floating-point number
    //  DOUBLE                  A double-precision floating-point number
    //  BIT                     A bit
    //
    //  CHAR                    A fixed-length nonbinary (character) string
    //  VARCHAR         (255)   A variable-length non-binary string
    //  BINARY                  A fixed-length binary string
    //  VARBINARY       (255)   A variable-length binary string
    //  TINYBLOB        (255)   A very small BLOB (binary large object)
    //  BLOB            (64K)   A small BLOB
    //  MEDIUMBLOB      (16M)   A medium-sized BLOB
    //  LONGBLOB	(4GB)   A large BLOB
    //  TINYTEXT        (255)   A very small non-binary string
    //  TEXT            (64K)   A small non-binary string
    //  MEDIUMTEXT      (16M)   A medium-sized non-binary string
    //  LONGTEXT        (4GB)   A large non-binary string
    //  ENUM            (64K)   An enumeration
    //  SET                     A set
    //
    //  DATE            A date value in CCYY-MM-DD format
    //  TIME            A time value in hhHmm:ss format
    //  DATETIME	A date and time value inCCYY-MM-DD hh:mm:ssformat
    //  TIMESTAMP	A timestamp value in CCYY-MM-DD hh:mm:ss format
    //  YEAR            A year value in CCYY or YY format
    //
    //  GEOMETRY	A spatial value of any type
    //  POINT           A point (a pair of X-Y coordinates)
    //  LINESTRING	A curve (one or more POINT values)
    //  POLYGON         A polygon
    //  GEOMETRYCOLLECTION	A collection of GEOMETRY values
    //  MULTILINESTRING	A collection of LINESTRINGvalues
    //  MULTIPOINT	A collection of POINT values
    //  MULTIPOLYGON	A collection of POLYGONvalues
    //
    //  PRIMARY KEY
    //  FOREIGN KEY
    //  CHECK           CHECK(COUNT > 0)
    //  UNIQUE
    //  NOT NULL
    //  AUTO INCREMENT
    //

    /************************************************************************
     *  Create the TITLE table (If it doesn't already exist).
     ************************************************************************/

    //  Create TITLE_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS title_table("
                               "recipe_id VARCHAR(50),"
                               "title TINYBLOB,"
                               "PRIMARY KEY(recipe_id)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS title_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... title_table" );
    }

    /************************************************************************
     *  Create the RECIPE table (If it doesn't already exist).
     ************************************************************************/

    //  Create RECIPE_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS recipe_table("
                               "recipe_id VARCHAR(50),"
                               "recipe BLOB,"
                               "PRIMARY KEY(recipe_id)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS recipe_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... recipe_table" );
    }

    /************************************************************************
     *  Create the INFO table (If it doesn't already exist).
     ************************************************************************/

    //  Create ADDRESS_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS info_table("
                               "recipe_id VARCHAR(50),"
                               "author TINYBLOB,"
                               "serves TINYBLOB,"
                               "makes TINYBLOB,"
                               "makes_unit TINYBLOB,"
                               "time_prep VARCHAR(10),"
                               "time_cook VARCHAR(10),"
                               "time_wait VARCHAR(10),"
                               "time_rest VARCHAR(10),"
                               "source TINYBLOB,"
                               "copyright TINYBLOB,"
                               "skill TINYINT,"
                               "rating TINYINT,"
                               "PRIMARY KEY(recipe_id)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS info_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... info_table" );
    }

    /************************************************************************
     *  Create the CUISINE table (If it doesn't already exist).
     ************************************************************************/

    //  Create CUISINE_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS cuisine_table ("
                               "recipe_id VARCHAR(50),"
                               "name VARCHAR(35)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS cuisine_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... cuisine_table" );
    }

    /************************************************************************
     *  Create the OCCASION table (If it doesn't already exist).
     ************************************************************************/

    //  Create OCCASION_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS occasion_table ("
                               "recipe_id VARCHAR(50),"
                               "name VARCHAR(35)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS occasion_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... occasion_table" );
    }

    /************************************************************************
     *  Create the COURSE table (If it doesn't already exist).
     ************************************************************************/

    //  Create COURSE_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS course_table ("
                               "recipe_id VARCHAR(50),"
                               "name VARCHAR(35)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS course_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... course_table" );
    }

    /************************************************************************
     *  Create the DIET table (If it doesn't already exist).
     ************************************************************************/

    //  Create DIET_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS diet_table ("
                               "recipe_id VARCHAR(50),"
                               "name VARCHAR(35)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS diet_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... diet_table" );
    }

    /************************************************************************
     *  Create the APPLIANCE table (If it doesn't already exist).
     ************************************************************************/

    //  Create APPLIANCE_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS appliance_table ("
                               "recipe_id VARCHAR(50),"
                               "name VARCHAR(35)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS appliance_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... appliance_table" );
    }

    /************************************************************************
     *  Create the CHAPTER table (If it doesn't already exist).
     ************************************************************************/

    //  Create CHAPTER_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS chapter_table ("
                               "recipe_id VARCHAR(50),"
                               "name VARCHAR(35)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS chapter_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... chapter_table" );
    }

    /************************************************************************
     *  Create the SOURCE table (If it doesn't already exist).
     ************************************************************************/

    //  Create SOURCE_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS source_table ("
                               "recipe_id VARCHAR(50),"
                               "format TINYTEXT,"
                               "file_name TINYTEXT,"
                               "file_size INTEGER,"
                               "file_date_time DATETIME,"
                               "group_name TINYBLOB,"
                               "group_subject TINYBLOB,"
                               "group_date_time DATETIME,"
                               "email_name TINYBLOB,"
                               "email_subject TINYBLOB,"
                               "email_date_time DATETIME"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS source_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... source_table" );
    }

    /************************************************************************
     *  Create the EDITED table (If it doesn't already exist).
     ************************************************************************/

    //  Create EDITED_TABLE
    sql_rc = mysql_query( con, "CREATE TABLE IF NOT EXISTS edited_table ("
                               "recipe_id VARCHAR(50),"
                               "user_id VARCHAR(50)"
                               ");");

#if DBASE_ACCESS_LOG == 1
    //  Log the dBase access command
    log_write( MID_INFO, "DBASE__open",
            "(%s) = %-30s\n", sql_rc?"TRUE":"FALSE", "CREATE TABLE IF NOT EXISTS edited_table(" );
#endif

    //  Was the command successful ?
    if ( sql_rc != 0 )
    {
        //  The database table creation filed.
        MYQL__access_error( "CREATE TABLE ... source_table" );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Bye-Bye.
    return ( true );
}

/****************************************************************************/
/**
 *  Open the database.  If required create the database and/or tables.
 *
 *  @param  db_column_p         Pointer to a list of column names
 *  @param  db_column_l         Size of the db_column buffer
 *  @param  db_value_p          Pointer to a list of column values
 *  @param  db_value_l          Size of the db_column values buffer
 *  @param  name_p              Pointer to the name of the next column
 *  @param  value_p             Pointer to the value of the next column
 *
 *  @return                     TRUE for success. Any other value is an error.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__add_col_val(
    char                    *   db_column_p,
    int                         db_column_l,
    char                    *   db_value_p,
    int                         db_value_l,
    char                    *   name_p,
    char                    *   value_p
    )
{
    /**
     *  @param  dbase_rc        Function return code                        */
    int                         dbase_rc;
    /**
     *  @param  tmp_l           Temporary length variable                   */
    int                         tmp_l;
    /**
     *  @param  tmp_data_p      Temporary data buffer                       */
    char                    *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    dbase_rc = true;

    //  Allocate storage for the temporary data buffer
    tmp_data_p = mem_malloc( strlen( value_p ) * 2 );

    /************************************************************************
     *  Column Name
     ************************************************************************/

    //  Get the current length of the buffer
    tmp_l = strlen( db_column_p );

    //  Will this cause a buffer overflow ?
    if ( ( db_column_l - tmp_l - strlen( name_p ) ) > 0 )
    {
        //  NO:     Is this the first column name ?
        if ( tmp_l == 0 )
        {
            //  YES:    No leading coma
            snprintf( ( db_column_p + tmp_l ),
                      ( db_column_l - tmp_l ),
                      "%s", name_p );
        }
        else
        {
            //  NO:     With a leading comma
            snprintf( ( db_column_p + tmp_l ),
                      ( db_column_l - tmp_l ),
                      ", %s", name_p );
        }
    }

    /************************************************************************
     *  Column Value
     ************************************************************************/

    //  Escape to create a legal SQL string
    mysql_real_escape_string( con, tmp_data_p, value_p, strlen( value_p ) );

    //  Get the current length of the buffer
    tmp_l = strlen( db_value_p );

    //  Will this cause a buffer overflow ?
    if ( ( db_value_l - tmp_l - strlen( tmp_data_p ) ) > 0 )
    {
        //  NO:     Is this the first column name ?
        if ( tmp_l == 0 )
        {
            //  YES:    No leading coma
            snprintf( ( db_value_p + tmp_l ),
                      ( db_value_l - tmp_l ),
                      "'%s'", tmp_data_p );
        }
        else
        {
            //  NO:     With a leading comma
            snprintf( ( db_value_p + tmp_l ),
                      ( db_value_l - tmp_l ),
                      ", '%s'", tmp_data_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the temporary data buffer
    mem_free( tmp_data_p );

    //  Bye-Bye.
    return ( dbase_rc );
}
/****************************************************************************/
