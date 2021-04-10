/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'xlate' library.
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
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Definitions local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
#define XLATE_MAX_L             ( 32 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Structures local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
struct  table_data_t
{
    char                        *   xlate_to_p;
    char                        *   xlate_from_p;
};
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
 *  The table_input function does all the hard work of reading
 *  the translation tables and saving the information.
 *
 *  @param  file_fp             A pointer to the file descriptor.
 *  @param  xlate_table_p       A pointer to the base of the table chain.
 *
 *  @return                     Upon successful completion PASS is returned
 *                              else FAIL is returned
 *
 ****************************************************************************/

int
XLATE__table_input(
    FILE                        *   file_fp,
    struct  list_base_t         *   xlate_table_p
    )
{
    /**
     *  @param  xlate_rc        Return code of a called function            */
    int                             xlate_rc;
    /**
     *  @param  table_data_p    Pointer to a table data entry               */
    struct  table_data_t        *   table_data_p;
    /**
     *  @param  to_string       A place to store the xlate 'to' data        */
    char                            to_string[ XLATE_MAX_L ];
    /**
     *  @param  from_string     A place to store the xlate 'from' data      */
    char                            from_string[ XLATE_MAX_L ];

    /**
     *  A place to put the read data.                                       */
    char                            in_data[ MAX_LINE_L ];
    /**
     *  A pointer into the in_data buffer                                   */
    char                        *   in_data_p;
    /**
     *  The size of the translation string                                  */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is there will NOT be any errors
    xlate_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Process all entries of the table
    while( 1 )
    {
        //  Always want to start with an empty input buffer
        memset( in_data, '\0', sizeof ( in_data ) );

        //  Initialize the translation string buffer
        memset( to_string, ' ', sizeof( to_string ) );
        to_string[ sizeof( to_string ) - 1 ] = '\0';

        //  Read another line of text from the file
        in_data_p = fgets( in_data, sizeof( in_data ) - 1 , file_fp );

        //  When in_data_p is equal to zero we are at End-Of_File
        if ( in_data_p == NULL )
        {
            break;
        }

        //  Strip off any CR LF characters at the end of this line
        text_remove_crlf( in_data_p );

        //  Look for a blank line to end the table
        if ( text_is_blank_line( in_data ) == true )
        {
            break;
        }

        //  Skip over leading whitespace
        in_data_p = text_skip_past_whitespace( in_data );

        //  Is this a comment line ?
        if ( in_data_p[ 0 ] != '#' )
        {
            //  NO; Copy the translation TO string into it's temporary buffer
            for( ndx = 0;
                 (    ( in_data_p[ 0 ] != ',' )
                   && ( in_data_p[ 0 ] != '\0' )
                   && ( in_data_p[ 0 ] != '\n' ) );
                 ndx += 1 )
            {
                to_string[ ndx ] = in_data_p[ 0 ];
                in_data_p += 1;
            }

            //  Now that we have the translation TO string, get all the
            //  translation SEARCH strings.
            while( 1 )
            {
                //  The input buffer pointer is pointing at End-Of-Line
                //  or a comma ( , ).  Move the pointer past it.
                in_data_p += 1;

                //  Skip over leading whitespace
                in_data_p = text_skip_past_whitespace( in_data_p );

                //  Initialize the translation string buffer
                memset( from_string, ' ', sizeof( from_string ) );
                from_string[ sizeof( from_string ) - 1 ] = '\0';

                //  Test for End-Of_line
                if (    ( in_data_p[ 0 ] == '\0' )
                     || ( in_data_p[ 0 ] == '\n' )
                     || ( in_data_p[ 0 ] == '\r' ) )
                {
                    break;
                }

                //  Read the translation search string into it's buffer
                for ( ndx = 0;
                      (    ( in_data_p[ 0 ] != ','  )
                        && ( in_data_p[ 0 ] != '\0' )
                        && ( in_data_p[ 0 ] != '\n' ) );
                      ndx += 1 )
                {
                    from_string[ ndx ] = in_data_p[ 0 ];
                    in_data_p += 1;
                }

                //  Is the from string more than a single character ?
                if ( from_string[ 1 ] != ' ' )
                {
                    //--------------------------------------------------------
                    //  @note   This is needed because 'T' as in Tablespoon
                    //          is not the same as 't' as in teaspoon.
                    //--------------------------------------------------------

                    //  YES:    Make the search data all UPPERCASE
                    text_to_uppercase( from_string );
                }

                //  It looks like this is a new table entry.  Allocate a
                //  new table buffer and add it to the list.
                table_data_p = mem_malloc( sizeof( struct table_data_t ) );

                log_write( MID_DEBUG_1, "xlate_lib.c", "Line: %d\n", __LINE__ );

                //  Save the translation to string
                table_data_p->xlate_to_p   = text_copy_to_new(   to_string );

                log_write( MID_DEBUG_1, "xlate_lib.c", "Line: %d\n", __LINE__ );

                table_data_p->xlate_from_p = text_copy_to_new( from_string );

                log_write( MID_DEBUG_1, "xlate_lib.c", "Line: %d\n", __LINE__ );

                //  Append the new entry to the table
                list_put_last( xlate_table_p, table_data_p );

                //  Log what is going into the table
                log_write( MID_DEBUG_0, "XLATE__table_input",
                              "TO: '%-30s'     FROM: '%-30s'\n",
                              table_data_p->xlate_to_p, table_data_p->xlate_from_p );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( xlate_rc );
}

/****************************************************************************/
/**
 *  Search through a translation table looking for a match to the
 *  SearchParameter.
 *
 *  @param  search_table_p      Pointer to the table being searched
 *  @param  search_string_p     Pointer to the string we are looking for.
 *
 *  @return found_string_p      Upon successful search, a pointer to the
 *                              translation data, else NULL is returned.
 *
 *  @note
 *      Not checked for strcpy strcat etc.
 *
 ****************************************************************************/

char  *
XLATE__find_in_table(
    struct  list_base_t         *   search_table_p,
    char                        *   search_string_p
    )
{
    /**
     *  @param  xlate_rc        Function return code                        */
    int                             xlate_rc;
    /**
     *  @param  found_string_p  Pointer to the string in the search buffer  */
    char                        *   found_string_p;
    /**
     *  @param  ndx             Relative position                           */
    int                             ndx;
    /**
     *  @param  search_string_l Size (in bytes) of the search string        */
    int                             search_string_l;
    /**
     *  @param  tmp_string      Temporary place to put a string             */
    char                            search_string[ XLATE_MAX_L ];
    /**
     *  @param  table_data_p    Pointer to a table data entry               */
    struct  table_data_t        *   table_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                             list_lock_key;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that the string will NOT be found.
    found_string_p = NULL;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    // Get the character length for the search string
    search_string_l = strlen( search_string_p );

    //  Will the search string fit in the local buffer ?
    if ( search_string_l >= sizeof ( search_string ) )
    {
        //  YES:    Log it.
        log_write( MID_DEBUG_0, "XLATE__find_in_table",
                      "Search string too long: '%-30s'\n", search_string_p );

        //  It isn't going to match anything in the search list.
        return( found_string_p );
    }

    // Flush the local search string
    memset( search_string, ' ', sizeof ( search_string ) );
    search_string[ sizeof ( search_string ) ] = '\0';

    //  If the search string length is greater then one, convert
    //  the search string to upper case.
    if ( search_string_l > 1 )
    {
        //  NO:     More than one character.
        //  Convert the search string to UPPER CASE
        for ( ndx = 0;
              ndx < search_string_l;
              ndx++ )
        {
            //  If this character is NOT alpha numeric copy it
            if ( isprint( search_string_p[ ndx ] ) != 0 )
            {
                //  Change to UPPER CASE and copy to the internal search buffer
                search_string[ ndx ] = toupper( search_string_p[ ndx ] );
            }
        }
    }
    else
    {
        //  NO:     Is this character alpha numeric ?
        if ( isprint( search_string_p[ 0 ] ) != 0 )
        {
            //  NO:     Only one character.
            search_string[ 0 ] = ( search_string_p[ 0 ] );
        }
    }

    //  Get the revised character length for the search string
    search_string_l = strlen( search_string );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( search_table_p );

    //  Scan the translation table for a match to the UPPER search string.
    for( table_data_p = list_fget_first( search_table_p, list_lock_key );
         table_data_p != NULL;
         table_data_p = list_fget_next( search_table_p, table_data_p, list_lock_key ) )
    {
        //  Compare the Search String against this table entry
        xlate_rc = strncmp( table_data_p->xlate_from_p,
                            search_string,
                            strlen( table_data_p->xlate_from_p ) );

        //  Are the table data and the search string the same ?
        if ( xlate_rc == 0 )
        {
            //  Update the return pointer with the translation data pointer.
            found_string_p = table_data_p->xlate_to_p;

            //  Found a translation.
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Unlock the list
    list_user_unlock( search_table_p, list_lock_key );

    //  DONE!
    return( found_string_p );
}

/****************************************************************************/
