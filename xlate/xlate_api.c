/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'xlate' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_XLATE          ( "ALLOCATEGORIESE STORAGE FOR XLATE" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <unistd.h>             //  Access to the POSIX operating system API
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include "global.h"             //  Global stuff for this application
#include "libtools_api.h"       //  My Tools Library
                                //*******************************************
#include "xlate_api.h"          //  API for all xlate_*             PUBLIC
#include "xlate_lib.h"          //  API for all XLATE__*            PRIVATE
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
#define TABLE                   "Table:"
#define TABLE_L                 strlen( TABLE )
//----------------------------------------------------------------------------
#define UNITOFMEASUREMENT       "Unit Of Measurement"
#define UNITOFMEASUREMENT_L     strlen( UNITOFMEASUREMENT )
//----------------------------------------------------------------------------
#define CATEGORIES              "Categories"
#define CATEGORIES_L            strlen( CATEGORIES )
//----------------------------------------------------------------------------
#define ABBREVIATIONS           "Abbreviations"
#define ABBREVIATIONS_L         strlen( ABBREVIATIONS )
//----------------------------------------------------------------------------
#define PARAGRAPHSTARTERS       "Paragraph Starters"
#define PARAGRAPHSTARTERS_L     strlen( PARAGRAPHSTARTERS )
//----------------------------------------------------------------------------
#define KEYWORDS                "Key Words"
#define KEYWORDS_L              strlen( KEYWORDS )
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

int
xlate_init(
    void
    )
{
    /**
     *  The assumption is that this function will pass                      */
    int                             xlate_rc;
    /**
     *  The file pointer for the Translations file                          */
    FILE                        *   xlate_fp;
    /**
     *  A place to put the read data.                                       */
    char                            in_data[ MAX_LINE_L ];
    /**
     *  A pointer into the in_data buffer                                   */
    char                        *   in_data_p;
    /**
     *  @param  path_p          Pointer to the directory path               */
    char                        *   path_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume a successful initialization
    xlate_rc = true;

    //  Create the translation lists
    unit_of_measurement_p = list_new( );
    categories_p          = list_new( );
    abbreviations_p       = list_new( );
    paragraph_starters_p  = list_new( );
    key_words_p           = list_new( );

    log_write( MID_DEBUG_0, "xlate_init",
                  "Allocate a new list structure 'unit_of_measurement_p' [%p].\n",
                  unit_of_measurement_p );
    log_write( MID_DEBUG_0, "xlate_init",
                  "Allocate a new list structure 'categories_p' [%p].\n",
                  categories_p );
    log_write( MID_DEBUG_0, "xlate_init",
                  "Allocate a new list structure 'abbreviations_p' [%p].\n",
                  abbreviations_p );
    log_write( MID_DEBUG_0, "xlate_init",
                  "Allocate a new list structure 'paragraph_starters_p' [%p].\n",
                  paragraph_starters_p );
    log_write( MID_DEBUG_0, "xlate_init",
                  "Allocate a new list structure 'key_words_p' [%p].\n",
                  key_words_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Get the LibTools directory path
    path_p = file_path_to_lib( );

    //  Append the Translations file name to the path
    strncat( path_p, "/", FILE_NAME_L );
    strncat( path_p, "Translations", FILE_NAME_L );

    //  Open the "Translations" file.
    xlate_fp = file_open_read( path_p );

    //  Start reading the file looking for another table
    while( 1 )
    {
        //  I always want to start with an empty input buffer
        memset( in_data, '\0', sizeof( in_data ) );

        //  Read another line of text from the file
        in_data_p = fgets( in_data, sizeof( in_data ), xlate_fp );

        //  Look for the version number
        if (    ( in_data_p != NULL )
             && ( strstr( in_data_p, "Version:" ) != NULL ) )
        {
            //  YES:    Log the version number.
            log_write( MID_INFO, "xlate_init",
                          "%s", in_data_p );
        }

        //  Test for End-Of_File
        if ( in_data_p == NULL )
        {
            break;
        }

        //  Strip off any CR LF characters at the end of this line
        text_remove_crlf( in_data_p );

        //  Skip over leading spaces and or tabs
        in_data_p = text_skip_past_whitespace( in_data_p );

        //  Look for a table header
        if ( strncmp( in_data_p, TABLE, TABLE_L ) == 0 )
        {
            //  Found a table header, Adjust the buffer pointer.
            in_data_p += TABLE_L;

            //  Skip over leading spaces and or tabs
            in_data_p = text_skip_past_whitespace( in_data_p );

            //  Identify the table type and process it
            if ( strncmp( in_data_p, UNITOFMEASUREMENT, UNITOFMEASUREMENT_L ) == 0 )
            {
                XLATE__table_input( xlate_fp, unit_of_measurement_p );
            }
            else
            if ( strncmp( in_data_p, CATEGORIES, CATEGORIES_L ) == 0 )
            {
                XLATE__table_input( xlate_fp, categories_p );
            }
            else
            if ( strncmp( in_data_p, ABBREVIATIONS, ABBREVIATIONS_L ) == 0 )
            {
                XLATE__table_input( xlate_fp, abbreviations_p );
            }
            else
            if ( strncmp( in_data_p, PARAGRAPHSTARTERS, PARAGRAPHSTARTERS_L ) == 0 )
            {
                XLATE__table_input( xlate_fp, paragraph_starters_p );
            }
            else
            if ( strncmp( in_data_p, KEYWORDS, KEYWORDS_L ) == 0 )
            {
                XLATE__table_input( xlate_fp, key_words_p );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( xlate_rc );
}

/****************************************************************************/
/**
 *  Search through the Units table looking for a match to the SearchParameter.
 *
 *  @param  search_string_p     Pointer to the string we are looking for.
 *
 *  @return                     Upon successful search, a pointer to the
 *                              translated data, else NULL is returned.
 *
 ****************************************************************************/

char  *
xlate_units(
    char                        *   search_string_p
    )
{
    char                        *   found_string_p = 0;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    // Find the Search String in the Measurements table
    found_string_p = XLATE__find_in_table( unit_of_measurement_p,
                                           search_string_p );

    //  Did we locate a translation for this string ?
    if ( found_string_p == NULL )
    {
        //  NO:     Write it to the log
#if 0
        log_write( MID_INFO, "xlate_units",
#else
        log_write( MID_DEBUG_0, "xlate_units",
#endif
                      "Could not find a units translation for '%s'\n",
                      search_string_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return a pointer to the found string
    return( found_string_p );
}

/****************************************************************************/
/**
 *  Search through the Categories table looking for a match to the
 *  SearchParameter.
 *
 *  @param  search_string_p     Pointer to the string we are looking for.
 *  @param  special_search      When TRUE a message is displayed when the
 *                              search string is not found.
 *
 *  @return                     Upon successful search, a pointer to the
 *                              translated data, else NULL is returned.
 *
 ****************************************************************************/

char  *
xlate_chapter(
    char                        *   search_string_p
    )
{
    /**
     *  Found_string_p          Pointer to the search target string.        */
    char                        *   found_string_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Find the Search String in the Measurements table
    found_string_p = XLATE__find_in_table( categories_p,
                                           search_string_p );

    //  Did we locate a translation for this string ?
    if ( found_string_p == NULL )
    {
        //  NO:     Write it to the log
#if 0
        log_write( MID_INFO, "xlate_categories",
#else
        log_write( MID_DEBUG_0, "xlate_categories",
#endif
                      "Could not find a chapter translation for '%s'\n",
                      search_string_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return a pointer to the found string
    return ( found_string_p );
}

/****************************************************************************/
/**
 *  Search through the Paragraph Starters table looking for a match
 *  to the SearchParameter.
 *
 *  @param  search_string_p     Pointer to the string we are looking for.
 *
 *  @return                     Upon successful search, a pointer to the
 *                              translated data, else NULL is returned.
 *
 ****************************************************************************/

char  *
xlate_paragraph_starters(
    char                        *   search_string_p
    )
{
    char                        *   found_string_p = NULL;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    // Find the Search String in the Measurements table
    found_string_p = XLATE__find_in_table( paragraph_starters_p,
                                           search_string_p );

    // Anything that ends with a Colon ( : ) is also the
    //        start of a new paragraph
    if ( search_string_p[ strlen( search_string_p ) - 1 ] == ':' )
    {
        found_string_p = search_string_p;
    }

        // Anything that starts with a asterisk ( * ) is also the
        //        start of a new paragraph
    else if ( search_string_p[ 0 ] == '*' )
    {
        found_string_p = search_string_p;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return a pointer to the found string
    return ( found_string_p );
}
