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
 *  library components of the 'post' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define _GNU_SOURCE             //  See feature_test_macros(7)

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <unistd.h>             //  UNIX standard library.
#include <string.h>             //  Functions for managing strings
#include <ctype.h>              //  Determine the type contained
#include <openssl/evp.h>        //  OpenSSL API for EVP Crypto libraries.
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
#include "xlate_api.h"          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include "decode_post_api.h"    //  API for all post_*              PUBLIC
#include "decode_post_lib.h"    //  API for all POST__*             PRIVATE
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
#define TIME_L                  ( 5 + 1 )
//----------------------------------------------------------------------------
#define MAKES_AMOUNT            ( 10 + 1 )
//----------------------------------------------------------------------------
#define MAKES_UNIT              ( 25 + 1 )
//----------------------------------------------------------------------------
#define LINE_L                  ( 255 + 1 )
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
 *  Scan the recipe title for information that can categorize the recipe.
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

static
char    *
DECODE_POST__get_tag_data(
    char                        *   source_data_p,
    char                        *   search_string_p
    )
{
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  string_beg_p    Beginning of the source string              */
    char                        *   string_beg_p;
    /**
     *  @param  string_end_p    End of the source string                    */
    char                        *   string_end_p;
    /**
     *  @param  string_l        Length of the source string                 */
    int                             string_l;
    /**
     *  @param  data_p          Pointer to a data buffer with the tag data  */
    char                        *   data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return data pointer.
    data_p = NULL;

    /************************************************************************
     *  Process a [FROM: "xxxx"] tag
     ************************************************************************/

    //  NO:     Is there a FROM: in this directions line ?
    if ( ( temp_p = strcasestr( source_data_p, search_string_p ) ) != NULL )
    {
        //  YES:    Move past the tag and any white space that follows
        string_beg_p = &temp_p[ strlen( search_string_p ) ];
        string_beg_p = text_skip_past_whitespace( string_beg_p );

        //  Is the next character a quotation ["]
        if (    ( string_beg_p[ 0 ] == '"'  )
             && ( string_beg_p[ 1 ] != '"'  )
             && ( string_beg_p[ 1 ] != '\0' ) )
        {
            //  YES:    Look for the ending quotation mark
            string_end_p = strchr( &string_beg_p[ 1 ], '"' );

            //  Is it a well formed source tag ?
            if ( string_end_p != NULL )
            {
                //  YES:    Allocate a data buffer for the tag data
                data_p = mem_malloc( MAX_LINE_L );

                //  Set the length of the data to copy out
                string_l = ( string_end_p - string_beg_p ) - 1;

                if ( MAX_LINE_L > string_l )
                {
                    //  Copy the source data string
                    memcpy( data_p, &string_beg_p[ 1 ], string_l );
                }
                else
                {
                    log_write( MID_FATAL, "FATAL-ERROR:",
                               "%s @ line %d (D:%d < S:%d)\n",
                               __FILE__, __LINE__,
                               MAX_LINE_L, string_l );
                }
                //  Now remove the FROM: string from the directions line.
                text_remove( temp_p, 0, ( string_end_p - temp_p ) + 1 );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( data_p );
}

/****************************************************************************/
/**
 *  The decode_save_chapter will identify which category subsection is
 *  identified and store the data in that section.
 *
 *  @param  data_p              Pointer to the raw data category
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

static
void
DECODE_POST__save_chapter(
    char                    *   data_p,
    struct  rcb_t           *   rcb_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Do we have a category to save ?
    if ( data_p != NULL )
    {
        //  Appliance ?
        if ( strncmp( APPLIANCE, data_p, APPLIANCE_L ) == 0 )
        {
            recipe_append( rcb_p->recipe_p->appliance,
                           ( data_p + APPLIANCE_L ) );
        }
        //  Diet ?
        else
        if ( strncmp( DIET, data_p, DIET_L ) == 0 )
        {
            recipe_append( rcb_p->recipe_p->diet,
                           ( data_p + DIET_L ) );
        }
        //  Course ?
        else
        if ( strncmp( COURSE, data_p, COURSE_L ) == 0 )
        {
            recipe_append( rcb_p->recipe_p->course,
                           ( data_p + COURSE_L ) );
        }
        //  Cuisine ?
        else
        if ( strncmp( CUISINE, data_p, CUISINE_L ) == 0 )
        {
            recipe_append( rcb_p->recipe_p->cuisine,
                           ( data_p + CUISINE_L ) );
        }
        //  Occasion ?
        else
        if ( strncmp( OCCASION, data_p, OCCASION_L ) == 0 )
        {
            recipe_append( rcb_p->recipe_p->occasion,
                           ( data_p + OCCASION_L ) );
        }
        //  Category ?
        else
        if ( strncmp( CATEGORY, data_p, CATEGORY_L ) == 0 )
        {
            recipe_append( rcb_p->recipe_p->chapter,
                           ( data_p + CATEGORY_L ) );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe title for information that can categorize the recipe.
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__title_information(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  name_p          Pointer to a local copy of the recipe title */
    char                    *   name_p;
    /**
     *  @param  buffer_p        A local pointer to the input buffer         */
    char                    *   buffer_p;
    /**
     *  @param  next_word       A local bucket for the next directions word */
    char                        next_word[ MAX_LINE_L ];
    /**
     * @param chapter_p         Pointer to a temp data buffer               */
    char                    *   chapter_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is there a recipe title to analyze ?
    if ( rcb_p->recipe_p->name != NULL )
    {
        //  YES:    Allocate storage for the local copy of the recipe title
        name_p = mem_malloc( ( strlen( rcb_p->recipe_p->name ) + 1 ) );
        log_write( MID_DEBUG_1, "decode_lib.c", "Line: %d\n", __LINE__ );

        //  Copy the recipe title to the local buffer
        memcpy( name_p, rcb_p->recipe_p->name, strlen( rcb_p->recipe_p->name ) );

        //  Clean the next word buffer
        memset( next_word, '\0', sizeof( next_word ) );

        //  Process all words in the input buffer
        for( buffer_p = text_next_word( name_p, next_word );
             buffer_p != NULL;
             buffer_p = text_next_word( buffer_p, next_word ) )
        {
            //  Lookup to see if this word has meaning
            chapter_p = xlate_chapter( next_word );

            //  Do we have a chapter to save ?
            if ( chapter_p != NULL )
            {
                //  YES:    Save it
                DECODE_POST__save_chapter( chapter_p, rcb_p );
            }
        }

        //  Release the recipe name buffer
        mem_free( name_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Append a new line of text to the directions data.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__fmt_directions(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  scan_buffer_p   A local pointer to the input buffer         */
    char                    *   scan_buffer_p;
    /**
     *  @param  formatted_text  Temp buffer - formatted directions line     */
    char                        formatted_text[ MAX_LINE_L + 1 ];
    /**
     *  @param  next_word       A local bucket for the next directions word */
    char                        next_word[ MAX_LINE_L ];
    /**
     *  @param  index           Scan index                                  */
    int                         index;
    /**
     *  @param  fwos            First Word of Sentence                      */
    static
    int                         fwos;
    /**
     *  @param  fwol            First Word of Line                          */
    static
    int                         fwol;
    /**
     *  @param  tmp_p           Pointer to a temporary buffer               */
    char                    *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Are we starting the directions for a new recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) == 0 )
    {
        //  YES:    Reset the first word flags
        fwos = true;
        fwol = true;
    }

    //  Is the first word of a new line ?
    if ( fwol == true )
    {
        //  YES:    Clear the formatted line buffer
        memset( formatted_text, '\0', sizeof( formatted_text ) );
    }
    else
    if ( list_query_count( rcb_p->recipe_p->directions ) != 0 )
    {
        //  NO:     Get the saved partial line of text from the list.
        tmp_p = list_get_last( rcb_p->recipe_p->directions );

        //  Move the data to the scan formatted text buffer
        strncpy( formatted_text, tmp_p, ( sizeof( formatted_text ) - 1 ) );

        //  Remove the last line of text from the list.
        list_delete( rcb_p->recipe_p->directions, tmp_p );

        //  Release the storage used by the temporary buffer
        mem_free( tmp_p );
    }

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Did we just get passed a blank line ?
    if ( text_is_blank_line( rcb_p->recipe_p->instructions ) == false )
    {
        //  NO:     Clean the next word buffer
        memset( next_word, '\0', sizeof( next_word ) );

        //  Process all words in the input buffer
        for( scan_buffer_p = text_next_word( rcb_p->recipe_p->instructions, next_word );
             scan_buffer_p != NULL;
             scan_buffer_p = text_next_word( scan_buffer_p, next_word ) )
        {

            /****************************************************************
             *  Replace 0x14 with a NULL
             ****************************************************************/

            //  Is this a matching character sequence ?
            if ( next_word[ strlen( next_word ) - 1 ] == 0x14 )
            {
                //  YES:    Replace with CR and terminate the line.
                next_word[ strlen( next_word ) - 1 ] = '\n';
            }

            /****************************************************************
             *  Replace '~-' with a CR
             ****************************************************************/

            //  Is this a matching character sequence ?
            if (    ( next_word[ 0 ] == '~' )
                 && ( next_word[ 1 ] == '-' ) )
            {
                //  YES:    Replace with CR and terminate the line.
                next_word[ 0 ] = '\n';
                next_word[ 1 ] = '\0';
            }

            /****************************************************************
             *  Remove format bars of all dashes.
             ****************************************************************/

            for( index = 0;
                 index < strlen( next_word );
                 index++ )
            {
                if ( next_word[ index ] != '-' )
                {
                    break;
                }
            }
            //  Was it just a line of dashes ?
            if ( index == strlen( next_word ) )
            {
                //  YES:    Ignore it..
                continue;
            }

            /****************************************************************
             *  Special transformation of a numbered directions list.
             ****************************************************************/

             // Is this a numbered directions sequence ?
            if (    ( fwos == true )
                 && ( isdigit( next_word[ 0 ] ) !=  0  )
                 && (    ( next_word[ 1 ]   == '.' )
                      || ( next_word[ 1 ]   == ')' )
                      || ( next_word[ 1 ]   == ']' )
                      || ( next_word[ 1 ]   == ';' )
                      || ( next_word[ 1 ]   == ':' ) ) )
            {
                //  YES:    Throw it away
                next_word[ 0 ] = '\0';

                //  That also makes whatever is next the First-Word-of-Line
                fwol = true;
            }

            /****************************************************************
             *  Anything that starts with a asterisk ( * ) is also the
             *  start of a new paragraph.
             ****************************************************************/

            if ( next_word[ 0 ] == '*' )
            {
                fwos = true;
                fwol = false;
            }

            /****************************************************************
             *  If the next string causes the formatted line to be to large,
             *  write it and start a new line.
             ****************************************************************/

            //  Will this word overflow the formatted output buffer ?
            if (   ( strlen( formatted_text ) + strlen( next_word ) + 1 )
                > MAX_LINE_L )
            {
                //  YES:    Save the current buffer and start a new buffer.
                tmp_p = text_copy_to_new( formatted_text );
                log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

                //  Add it to the list.
                list_put_last( rcb_p->recipe_p->directions, tmp_p );

                //  Clear the formatted text buffer.
                memset( formatted_text, '\0', sizeof( formatted_text ) );
            }

//  @note:  This was initially put in to assist in formatting sections of the
//          directions such as 'Dough:'.  However all to frequently the recipe
//          is written something like 'For the dough:' which will leave 'For the'
//          on the current line and start a new line with 'Dough:'.
//          Anyhow for now it's more trouble then it's worth so I am disabling
//          the code.
#if 0
            /****************************************************************
             *  ANY word that ends with a colon (:) is the start of a new
             *  sentence and line.
             ****************************************************************/

            // Does this word end with a colon ?
            if ( strchr( next_word, ':' ) != NULL )
            {
                //  YES:    Set the flags.
                fwos = true;
                fwol = true;

                //  Is there anything in the formatted text buffer ?
                if ( strlen( formatted_text ) > 0 )
                {
                    //  YES:    Copy it to an allocated buffer
                    tmp_p = text_copy_to_new( formatted_text );
                    log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

                    //  Add it to the list.
                    list_put_last( rcb_p->recipe_p->directions, tmp_p );

                    //  Clear the formatted text buffer.
                    memset( formatted_text, '\0', sizeof( formatted_text ) );
                    }
            }
#endif

            /****************************************************************
             *  First Word of Sentence and First-Word-of-Line
             ****************************************************************/

            //  First-Word-Of-Sentence && First-Word-Of-Line && something to save
            if (    ( fwos == true )
                 && ( fwol == true )
                 && ( strlen( next_word ) > 0 ) )
            {
                //  YES:    Drop the new word in the output buffer.
                strncpy( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                formatted_text[ 0 ] = toupper( formatted_text[ 0 ] );

                //  It is no lonfer the first word of anything.
                fwos = false;
                fwol = false;
            }

            /****************************************************************
             *  First Word of Sentence AND NOT First-Word-of-Line
             ****************************************************************/

            else
            if (    ( fwos == true  )
                 && ( fwol == false )
                 && ( strlen( next_word ) > 0 ) )
            {
                //  This is the first word of a new sentence but not
                //  the first word of a new output line.  Test the
                //  word for a paragraph starter.
                if ( xlate_paragraph_starters( next_word ) )
                {
                    //  Make sure there is something in the current line
                    if ( text_is_blank_line( formatted_text ) == false )
                    {
                        //  This word marks the start of a new sentence.
                        //  Write the current output line and start a new one
                        tmp_p = text_copy_to_new( formatted_text );
                        log_write( MID_DEBUG_1, "recipe_api.c",
                                "Line: %d\n", __LINE__ );

                        //  Add it to the list.
                        list_put_last( rcb_p->recipe_p->directions, tmp_p );

                        //  Clear the formatted text buffer.
                        memset( formatted_text, '\0', sizeof( formatted_text ) );
                    }
                }
                strncat( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                formatted_text[ 0 ] = toupper( formatted_text[ 0 ] );
                fwos = false;
                fwol = false;
            }

            /****************************************************************
             *  NOT First-Word-of-Sentence AND First-Word-of-Line
             ****************************************************************/

            else
            if (    ( fwos == false )
                 && ( fwol == true  )
                 && ( strlen( next_word ) > 0 ) )
            {
                //  Append the new word to an existing sentence.
                strncat( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ", MAX_LINE_L - strlen( formatted_text ) );
                fwos = false;
                fwol = false;
            }

            /****************************************************************
             *  NOT First-Word-of-Sentence and NOT First-Word-of-Line
             ****************************************************************/

            else
            if (    ( fwos == false )
                 && ( fwol == false )
                 && ( strlen( next_word ) > 0 ) )
            {
                //  Append the new word to an existing sentence.
                strncat( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                fwos = false;
                fwol = false;
            }
            //  Is this the end of a sentence ?
            if ( next_word[ strlen( next_word ) - 1 ] == '.' )
            {
                //  YES:    Insert an extra space in the formatted text.
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                fwos = true;
            }
            //  Is this a special ?
            if (    ( fwos == true )
                 && ( formatted_text[ 0 ] == '*' ) )
            {
                //  YES:    Make sure there is something in the current line
                if ( text_is_blank_line( formatted_text ) == false )
                {
                    //  This word marks the start of a new sentence.
                    //  Write the current output line and start a new one
                    tmp_p = text_copy_to_new( formatted_text );
                    log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

                    //  Add it to the list.
                    list_put_last( rcb_p->recipe_p->directions, tmp_p );

                    //  Clear the formatted text buffer.
                    memset( formatted_text, '\0', sizeof( formatted_text ) );
                    fwol = true;
                }
            }

            //  Clean the next word buffer
            memset( next_word, '\0', sizeof( next_word ) );
        }
    }
    else
    {
        //  YES:    Whatever (if anything else) is next will start a new line.
        fwos = true;
        fwol = true;
    }

    /************************************************************************
     *  We are done with the input line of text.  Save it on the list.
     ************************************************************************/

    //  Is there something in the formatted text buffer ?
    if ( strlen( formatted_text ) > 0 )
    {
        //  YES:    Copy it to a temporary buffer.
        tmp_p = text_copy_to_new( formatted_text );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        //  Add it to the list.
        list_put_last( rcb_p->recipe_p->directions, tmp_p );

        //  Clear the formatted text buffer.
        memset( formatted_text, '\0', sizeof( formatted_text ) );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Some recipes have embedded content in the recipe directions.  Such as
 *  the original poster, number of people it serves, etc.
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *      Be careful here.  If you try to use this to simply change case the
 *      function will go into an infinite loop.
 *
 ****************************************************************************/

void
DECODE_POST__directions_cleanup(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                    *   directions_p;
    /**
     *  @param  compare_p       Pointer to a compare match string           */
    char                    *   compare_p;
    /**
     *  @param  replace         A flag showing that something was replace   */
    int                         replace;
    /**
     *  @param  found           A flag showing that a tag was found         */
    int                         found;
    /**
     *  @param  directions_l    Initial length of the directions string     */
    int                         directions_l;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the found flag;
    found = false;

    //  @NOTE - Some substitutions will take more space then the original text
    //          so the source buffer size is being increased by an arbitrary
    //          number.

    //  Set the initial size of the buffer
    directions_l = strlen( rcb_p->recipe_p->instructions ) + LINE_L;

    //  Allocate a new (larger) buffer.
    directions_p = mem_malloc( directions_l );

    //  Copy the instructions data to the new buffer.
    memcpy( directions_p, rcb_p->recipe_p->instructions,
           ( directions_l - LINE_L ) );

    //  Release the old buffer
    mem_free( rcb_p->recipe_p->instructions );

    //  And finally, replace the pointer with the new buffer
    rcb_p->recipe_p->instructions = directions_p;

    /************************************************************************
     *  Cleanup some common formatting problems
     ************************************************************************/

    do
    {
        //  Set the flag
        replace = false;

        //--------------------------------------------------------------------
        //  "Date :"
        compare_p = strcasestr( directions_p, "date :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Date:");

            replace = true;
        }
        //====================================================================
        //  "From ::
        compare_p = strcasestr( directions_p, "from :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "From:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  "From The:"
        compare_p = strcasestr( directions_p, "from the:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "From:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  ">From:"
        compare_p = strcasestr( directions_p, ">from:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "From:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  "From The "
        //  Can't use this because "from the" occurs in normal recipe directions.
        //====================================================================
        //  Makes :
        compare_p = strcasestr( directions_p, "makes :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Makes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Makes
        compare_p = strcasestr( directions_p, "makes " );
        if ( compare_p != NULL )
        {
            /**
             *  @param  tmp_p           Temporary pointer                   */
            char                        *   tmp_p;

            //  Is the next thing numeric ?
            tmp_p = compare_p + 6;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is it numeric ?
            if ( isdigit( tmp_p[ 0 ] ) != 0 )
            {
                memcpy( compare_p, "Makes:",       6 );
                replace = true;
            }
        }
        //====================================================================
        //  Notes :
        compare_p = strcasestr( directions_p, "notes :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 7 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Notes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Note :
        compare_p = strcasestr( directions_p, "note :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Notes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Notes
        compare_p = strcasestr( directions_p, "notes " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Notes:");

            replace = true;
        }
        //====================================================================
        //  Per Serving:
        compare_p = strcasestr( directions_p, "per serving:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 12 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "PerServing:");

            replace = true;
        }
        //====================================================================
        //  Posted By :
        compare_p = strcasestr( directions_p, "posted by :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 11 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted By:
        compare_p = strcasestr( directions_p, "posted by:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted By
        compare_p = strcasestr( directions_p, "posted by " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //====================================================================
        //  Posted To :
        compare_p = strcasestr( directions_p, "posted to :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 11 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted To:
        compare_p = strcasestr( directions_p, "posted to:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted To
        compare_p = strcasestr( directions_p, "posted to " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //====================================================================
        //  Recipe By :
        compare_p = strcasestr( directions_p, "recipe by :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 11 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "RecipeBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By:
        compare_p = strcasestr( directions_p, "recipe by:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "RecipeBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By
        compare_p = strcasestr( directions_p, "recipe by " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "RecipeBy:");

            replace = true;
        }
        //====================================================================
        //  Sent By :
        compare_p = strcasestr( directions_p, "sent by :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent By:
        compare_p = strcasestr( directions_p, "sent by:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent By
        compare_p = strcasestr( directions_p, "sent by " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //====================================================================
        //  Sent To :
        compare_p = strcasestr( directions_p, "sent to :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent To:
        compare_p = strcasestr( directions_p, "sent to:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent To
        compare_p = strcasestr( directions_p, "sent to " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //====================================================================
        //  Servings :
        compare_p = strcasestr( directions_p, "servings :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Serves:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Servings:
        compare_p = strcasestr( directions_p, "servings:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Serves:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Serves :
        compare_p = strcasestr( directions_p, "serves :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Serves:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Serves
        //  NOTE:   This code gets confused by compound words such as
        //          preserves.  They get translated into 'preServes:'
#if 0
        compare_p = strcasestr( directions_p, "serves " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Serves:",     7 );
            replace = true;
        }
#endif
        //====================================================================
        //  Source :
        compare_p = strcasestr( directions_p, "source :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Source:");

            replace = true;
        }
        //====================================================================
        //  To Plate:
        compare_p = strcasestr( directions_p, "to plate:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "ToPlate:");

            replace = true;
        }
        //====================================================================
        //  Yield :
        compare_p = strcasestr( directions_p, "yield :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 7 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Makes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Yield:
        compare_p = strcasestr( directions_p, "yield:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Makes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Yield
        compare_p = strcasestr( directions_p, "yield " );
        if ( compare_p != NULL )
        {
            /**
             *  @param  tmp_p           Temporary pointer                   */
            char                        *   tmp_p;

            //  Is the next thing numeric ?
            tmp_p = compare_p + 6;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is it numeric ?
            if ( isdigit( tmp_p[ 0 ] ) != 0 )
            {
                memcpy( compare_p, "Makes:",       6 );
                replace = true;
            }
        }
        //====================================================================
        //  Copyright,
        compare_p = strcasestr( directions_p, "copyright," );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Copyright:");

            replace = true;
        }
        //====================================================================
        //  Imported-From:
        //====================================================================
        compare_p = strcasestr( directions_p, "S(Imported From):" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 17 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Imported-From:");

            replace = true;
        }
        //====================================================================
        //  TIME-COOK:
        //====================================================================
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook time on High):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 21 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cooking Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 16 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Baking Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 15 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook Time:):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 13 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Bake Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 13 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 13 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cooking):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 11 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Baking):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 10 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Grill):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 9 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 8 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Bake):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 8 );

                //  Set the found flag
                found = true;
            }
        }

        //  Did we find a tag ?
        if ( found == true )
        {
            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Time-Cook:");

            //  Something was replace and changed.
            replace = true;

            //  Reset the found flag
            found = false;
        }
        //====================================================================
        //  TIME-WAIT:
        //====================================================================
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Marinating time:):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 20 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Standing Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 17 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Standing Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 17 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Chilling):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 12 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Chill:):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 10 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Chill):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 9 );

                //  Set the found flag
                found = true;
            }
        }

        //  Did we find a tag ?
        if ( found == true )
        {
            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Time-Wait:");

            //  Something was replace and changed.
            replace = true;

            //  Reset the found flag
            found = false;
        }
        //====================================================================
        //  TIME-TOTAL:
        //====================================================================
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "Start to Finish Time:" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 21 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Prep & Cook Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 20 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Total Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 14 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Ready in):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 12 );

                //  Set the found flag
                found = true;
            }
        }

        //  Did we find a tag ?
        if ( found == true )
        {
            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Time-Total:");

            //  Something was replace and changed.
            replace = true;

            //  Reset the found flag
            found = false;
        }
        //--------------------------------------------------------------------

    }   while( replace == true );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [FROM: "wxyz"]
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_from(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [FROM: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "From:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Format the output data
                memset( temp_data, '\0', sizeof( temp_data ) );
                snprintf( temp_data, sizeof( temp_data ),
                          "FROM: \"%s\"", temp_p );

                //  Free storage used by the old buffer
                mem_free( temp_p );


                //  Add it to [NOTES :].
                list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [SOURCE: "wxyz"]
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_source(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                    *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                    *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                        temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                         saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [SOURCE: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Source:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a source ?
                if ( rcb_p->recipe_p->source == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->source = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Source: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [COPYRIGHT: "wxyz"]
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_copyright(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                             saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [COPYRIGHT: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Copyright:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a copyright ?
                if ( rcb_p->recipe_p->copyright == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->copyright = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Copyright: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [Description: "wxyz"]
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_description(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                    *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                    *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                        temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                         saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [DESCRIPTION: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Description:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( rcb_p->recipe_p->description == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->description = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Description: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [MAKES: "0 0/0 wxyz"]
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_makes(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                    *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                    *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                        temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                         saved;
    /**
     *  @param  makes_amount    How many of something will be made          */
    char                    *   makes_amount;
    /**
     *  @param  makes_unit      What is being made                          */
    char                    *   makes_unit;
    /**
     *  @param  ndx             Index into a buffer                         */
    int                         ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [MAKES: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Makes:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Allocate storage for the two parts
                makes_amount = mem_malloc( MAKES_AMOUNT );
                makes_unit   = mem_malloc( MAKES_UNIT );

                //  Copy the makes_amount
                for ( ndx = 0;
                      ndx < MAKES_AMOUNT;
                      ndx += 1 )
                {
                    //  Is this a valid amount character ?
                    if (    ( isdigit( temp_p[ ndx ] ) !=  0  )
                         || ( temp_p[ ndx ]            == '/' )
                         || ( temp_p[ ndx ]            == ' ' ) )
                    {
                        //  YES:    Copy the character
                        strncat( makes_amount, &temp_p[ ndx ], 1 );
                    }
                    else
                    {
                        //  NO:     Done with makes_amount
                        break;
                    }
                }

                //  Copy the makes_unit
                for ( ;
                      ndx < MAKES_UNIT;
                      ndx += 1 )
                {
                    //  Is this a valid amount character ?
                    if ( temp_p[ ndx ] != '\0' )
                    {
                        //  YES:    Copy the character
                        strncat( makes_unit, &temp_p[ ndx ], 1 );
                    }
                    else
                    {
                        //  NO:     Done with makes_unit
                        break;
                    }
                }

                //  Release the storage used by the temporary buffer.
                mem_free( temp_p );

                //  Is there already a makes amount or unit ?
                if (    ( rcb_p->recipe_p->makes      == NULL )
                     && ( rcb_p->recipe_p->makes_unit == NULL ) )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if (    ( strlen( makes_amount ) < MAKES_AMOUNT - 1 )
                         && ( strlen( makes_unit   ) < MAKES_UNIT   - 1 ) )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->makes      = makes_amount;
                        rcb_p->recipe_p->makes_unit = makes_unit;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Yield: \"%s %s\"", makes_amount, makes_unit );

                    //  Free storage used by the old buffer
                    mem_free( makes_amount );
                    mem_free( makes_unit );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [TIME-wxyz: "00:00"]
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_time(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                    *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                    *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                        temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                         saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [TIME-wxyz: "00:00"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //----------------------------------------------------------------
            //  TIME-PREP:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Time-Prep:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( rcb_p->recipe_p->time_prep == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->time_prep = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-PREP: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-WAIT:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Time-Wait:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( rcb_p->recipe_p->time_wait == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->time_wait = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-WAIT: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-COOK:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Time-Cook:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( rcb_p->recipe_p->time_cook == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->time_cook = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-COOK: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-REST:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Time-Rest:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( rcb_p->recipe_p->time_rest == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->time_rest = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-REST: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-TOTAL:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Time-Total:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( rcb_p->recipe_p->time_total == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->time_total = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-TOTAL: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [S(Imported From): "wxyz"]
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_import_from(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                    *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                    *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                        temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                         saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [Imported-From: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE_POST__get_tag_data( directions_p, "Imported-From:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( rcb_p->recipe_p->import_from == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        rcb_p->recipe_p->import_from = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Imported-From: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( rcb_p->recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe title for information that can categorize the recipe.
 *
 *  @param  rcb_p               Pointer to a recipe control block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__directions_notes(
    struct  rcb_t           *   rcb_p
    )
{
    /**
     *  @param  free_note       Flag to free memory for the buffer          */
    int                         free_note;
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                    *   directions_p;
    /**
     *  @param  notes_p         Pointer to the start of a notes string      */
    char                    *   notes_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that the buffer will not be free'd when we are done
    free_note = false;

    /************************************************************************
     *  Function
     ************************************************************************/

    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        for( directions_p = list_get_first( rcb_p->recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p ) )
        {
            //  Look for the keyword "NOTES:" or "NOTE:"
            if (    ( ( notes_p = strstr( directions_p, "NOTES:" ) ) != NULL )
                 || ( ( notes_p = strstr( directions_p, "Notes:" ) ) != NULL )
                 || ( ( notes_p = strstr( directions_p, "Note:"  ) ) != NULL )
                 || ( ( notes_p = strstr( directions_p, "Note:"  ) ) != NULL ) )
            {
                //  Is this a Notes: ONLY directions line ?
                if ( notes_p == directions_p )
                {
                    //  YES:    Remove it from the directions
                    list_delete( rcb_p->recipe_p->directions, directions_p );

                    //  We are go to have to free this buffer later.
                    free_note = true;
                }

                //  Remove the remaining text from directions by NULL terminating
                notes_p[ 0 ] = '\0';

                //  Locate the end of the notes tag
                notes_p = strchr( ++notes_p, ':' );

                //  Move past any leading white space
                notes_p = text_skip_past_whitespace( ++notes_p );

                //  Is there anything else on this line ?
                if ( text_is_blank_line( notes_p ) == false )
                {
                    //  YES:    Add the remaining text to the notes.
                    recipe_fmt_notes( rcb_p->recipe_p, notes_p );

                    //  Are we supposed to free the buffer ?
                    if ( free_note == true )
                    {
                        //  YES:    Release the storage buffer.
                        mem_free( directions_p );

                        //  And clear the flag
                        free_note = false;
                    }
                }
                else
                {

                    //  NO:     Are we supposed to free the buffer ?
                    if ( free_note == true )
                    {
                        //  YES:    Release the storage buffer.
                        mem_free( directions_p );

                        //  And clear the flag
                        free_note = false;
                    }

                    //  Add the next line to the notes:
                    directions_p = list_get_next( rcb_p->recipe_p->directions, directions_p );

                    //  Sanity check! Did we get something
                    if ( directions_p != NULL )
                    {
                        //  YES:    Remove the entire line from the directions
                        list_delete( rcb_p->recipe_p->directions, directions_p );

                        //  Now add it to the notes
                        recipe_fmt_notes( rcb_p->recipe_p, directions_p );

                        //  Release the storage buffer.
                        mem_free( directions_p );
                    }
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Compute the recipe checksum (Recipe-ID)
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return void                Upon successful completion TRUE is returned
 *                              else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_POST__recipe_id(
    struct  rcb_t           *   rcb_p
    )
{
    EVP_MD_CTX              *   mdctx;
    const   EVP_MD          *   md;
    unsigned char               md_value[ EVP_MAX_MD_SIZE ];
    unsigned int                md_len;


    /**
     *  @param  AUIP_count      Number of ingredients in this recipe        */
    int                         AUIP_count;
    /**
     *  @param  recipe_id       Temporary data buffer for the recipe id     */
    unsigned char               recipe_id[ SHA1_DIGEST_SIZE + 2 ];
    /**
     *  @param  id_string       Recipe-ID string to identify a recipe       */
    char                        id_string[ ( SHA1_DIGEST_SIZE * 2 ) + 4 ];
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t          *   auip_p;

    mdctx = EVP_MD_CTX_new();

    md = EVP_get_digestbyname( "SHA256" );
    if ( md == NULL )
    {
        printf( "Unknown message digest SHA256\n" );
        exit( 1 );
    }

    memset( recipe_id, 0x00, sizeof( recipe_id ) );

    //  Query the number of ingredients for this recipe
    AUIP_count = list_query_count( rcb_p->recipe_p->ingredient );

    //  Are there any ingredients in this recipe ?
    if ( AUIP_count > 0 )
    {
        //  YES:    Loop through all of the ingredients in this recipe
        for( auip_p = list_get_first( rcb_p->recipe_p->ingredient );
             auip_p != NULL;
             auip_p = list_get_next( rcb_p->recipe_p->ingredient, auip_p ) )
        {
            //  Is there an amount ?
            if ( auip_p->amount_p != NULL )
            {
                //  YES:    Initialize
                EVP_DigestInit_ex( mdctx, md, NULL );

                //  Build SHA1 version of the amount field
                EVP_DigestUpdate( mdctx, auip_p->amount_p, strlen( auip_p->amount_p ) );

                //  Finalize the SHA1 operation
                EVP_DigestFinal_ex( mdctx, md_value, &md_len );

                //  Add the two together
                sha1_sum( (char*)recipe_id, (char*)recipe_id, (char*)md_value );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                //  YES:    Initialize SHA1
                EVP_DigestInit_ex( mdctx, md, NULL );

                //  Build SHA1 version of the unit field
                EVP_DigestUpdate( mdctx, auip_p->unit_p, strlen( auip_p->unit_p ) );

                //  Finalize the SHA1 operation
                EVP_DigestFinal_ex( mdctx, md_value, &md_len );

                //  Add the two together
                sha1_sum( (char*)recipe_id, (char*)recipe_id, (char*)md_value );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                //  YES:    Initialize SHA1
                EVP_DigestInit_ex( mdctx, md, NULL );

                //  Build SHA1 version of the unit field
                EVP_DigestUpdate( mdctx, auip_p->ingredient_p, strlen( auip_p->ingredient_p ) );

                //  Finalize the SHA1 operation
                EVP_DigestFinal_ex( mdctx, md_value, &md_len );

                //  Add the two together
                sha1_sum( (char*)recipe_id, (char*)recipe_id, (char*)md_value );
            }
        }

        //  Format the Recipe-ID as a hex string
        snprintf( id_string, sizeof( id_string ),
                  "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
                  "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
                  recipe_id[  0 ], recipe_id[  1 ], recipe_id[  2 ],
                  recipe_id[  3 ], recipe_id[  4 ], recipe_id[  5 ],
                  recipe_id[  6 ], recipe_id[  7 ], recipe_id[  8 ],
                  recipe_id[  9 ], recipe_id[ 10 ], recipe_id[ 11 ],
                  recipe_id[ 12 ], recipe_id[ 13 ], recipe_id[ 14 ],
                  recipe_id[ 15 ], recipe_id[ 16 ], recipe_id[ 17 ],
                  recipe_id[ 18 ], recipe_id[ 19 ],
                  AUIP_count );

        //  Add it to the recipe
        rcb_p->recipe_p->recipe_id = text_copy_to_new( id_string );
    }
    else
    {
        //  NO:     A recipe without ingredients isn't a recipe.
        rcb_p->recipe_p->recipe_id =
                text_copy_to_new( "000000000000000000000000000000000000000000" );
    }

    EVP_MD_CTX_free( mdctx );
}
/****************************************************************************/