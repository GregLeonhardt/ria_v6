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
 *  library components of the 'email' library.
 *
 *  @note
 *
 *  @ToDo: 2 e-Mail
 *          Reformat e-Mail back to plain-text.
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_EMAIL             ( "ALLOCATE STORAGE FOR EMAIL" )

#define STUB                    ( 0 )

#define _GNU_SOURCE

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
#include "tcb_api.h"            //  API for all tcb_*               PUBLIC
#include "rcb_api.h"            //  API for all rcb_*               PUBLIC
#include "recipe_api.h"         //  API for all recipe_*            PUBLIC
                                //*******************************************
#include "email_api.h"          //  API for all email_*             PUBLIC
#include "email_lib.h"          //  API for all EMAIL__*            PRIVATE
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
#define SRCH_CONTENT_TYPE           "Content-Type:"
#define SRCH_CONTENT_TYPE_L         strlen( SRCH_CONTENT_TYPE )
//----------------------------------------------------------------------------
#define SRCH_CT_T_HTML              "text/html"
#define SRCH_CT_T_HTML_L            strlen( SRCH_CT_T_HTML )
//----------------------------------------------------------------------------
#define SRCH_CT_TEXT                "text"
#define SRCH_CT_TEXT_L              strlen( SRCH_CT_TEXT )
//----------------------------------------------------------------------------
#define SRCH_CT_MULTIPART           "multipart/"
#define SRCH_CT_MULTIPART_L         strlen( SRCH_CT_MULTIPART )
//----------------------------------------------------------------------------
#define SRCH_CT_APPLICATION         "application/"
#define SRCH_CT_APPLICATION_L       strlen( SRCH_CT_APPLICATION )
//----------------------------------------------------------------------------
#define SRCH_CT_IMAGE               "image/"
#define SRCH_CT_IMAGE_L             strlen( SRCH_CT_IMAGE )
//----------------------------------------------------------------------------
#define SRCH_CT_VIDEO               "video/"
#define SRCH_CT_VIDEO_L             strlen( SRCH_CT_VIDEO )
//----------------------------------------------------------------------------
#define SRCH_CT_AUDIO               "audio/"
#define SRCH_CT_AUDIO_L             strlen( SRCH_CT_AUDIO )
//----------------------------------------------------------------------------
#define SRCH_CT_MESSAGE             "message/"
#define SRCH_CT_MESSAGE_L           strlen( SRCH_CT_MESSAGE )
//----------------------------------------------------------------------------
#define SRCH_CT_UNKNOWN             "unknown/"
#define SRCH_CT_UNKNOWN_L           strlen( SRCH_CT_UNKNOWN )
//----------------------------------------------------------------------------
#define SRCH_ENCODING_TYPE          "Content-Transfer-Encoding:"
#define SRCH_ENCODING_TYPE_L        strlen( SRCH_ENCODING_TYPE )
//----------------------------------------------------------------------------
#define SRCH_CTE_QUOTE_PRINT        "quoted-printable"
#define SRCH_CTE_QUOTE_PRINT_L      strlen( SRCH_CTE_QUOTE_PRINT )
//----------------------------------------------------------------------------
#define SRCH_CTE_7BIT               "7bit"
#define SRCH_CTE_7BIT_L             strlen( SRCH_CTE_7BIT )
//----------------------------------------------------------------------------
#define SRCH_CTE_8BIT               "8bit"
#define SRCH_CTE_8BIT_L             strlen( SRCH_CTE_8BIT )
//----------------------------------------------------------------------------
#define SRCH_CTE_BASE64             "base64"
#define SRCH_CTE_BASE64_L           strlen( SRCH_CTE_BASE64 )
//----------------------------------------------------------------------------
#define SRCH_CTE_BINARY             "binary"
#define SRCH_CTE_BINARY_L           strlen( SRCH_CTE_BINARY )
//----------------------------------------------------------------------------
#define SRCH_SOURCE                 "From - "
#define SRCH_SOURCE_L               strlen( SRCH_SOURCE )
//----------------------------------------------------------------------------
#define SRCH_PGO                    "PGO-ID:"
#define SRCH_PGO_L                  strlen( SRCH_PGO )
//----------------------------------------------------------------------------
#define SRCH_RETURN                 "Return-Path:"
#define SRCH_RETURN_L               strlen( SRCH_RETURN )
//----------------------------------------------------------------------------
#define SRCH_RXF_BREAK              "........................................"
#define SRCH_RXF_BREAK_L            strlen( SRCH_RXF_BREAK )
//----------------------------------------------------------------------------
#define SRCH_FROM                   "From:"
#define SRCH_FROM_L                 strlen( SRCH_FROM )
//----------------------------------------------------------------------------
#define SRCH_POSTED                 "Posted by:"
#define SRCH_POSTED_L               strlen( SRCH_POSTED )
//----------------------------------------------------------------------------
#define SRCH_MAILINGLIST            "Mailing-List:"
#define SRCH_MAILINGLIST_L          strlen( SRCH_MAILINGLIST )
//----------------------------------------------------------------------------
#define SRCH_SENDER                 "Sender:"
#define SRCH_SENDER_L               strlen( SRCH_SENDER )
//----------------------------------------------------------------------------
#define SRCH_X_TO                   "X-Apparently-To:"
#define SRCH_X_TO_L                 strlen( SRCH_X_TO )
//----------------------------------------------------------------------------
#if 0
#define SRCH_DELIVER                "Delivered-To:"
#define SRCH_DELIVER_L              strlen( SRCH_DELIVER )
#endif
//----------------------------------------------------------------------------
#define SRCH_DATETIME               "Date:"
#define SRCH_DATETIME_L             strlen( SRCH_DATETIME )
//----------------------------------------------------------------------------
#define SRCH_SUBJECT                "Subject:"
#define SRCH_SUBJECT_L              strlen( SRCH_SUBJECT )
//----------------------------------------------------------------------------
#define SRCH_BOUNDARY               "Boundary"
#define SRCH_BOUNDARY_L             strlen( SRCH_BOUNDARY )
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
 *  Test the input text line to see if it is the start of a new e-Mail message.
 *
 *  @param  data_p              Pointer to a line of text data.
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      From - Wed Jun 18 07:08:29 1997
 *
 ****************************************************************************/

int
email_is_start(
    char                        *   data_p
    )
{
    /**
     * @param email_rc          Return code for this function               */
    int                             email_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a new e-Mail message
    email_rc = false;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    if (    ( start_p != NULL )                           //  Data is present
         && ( start_p[ 0 ] != '>' ) )                     //  Not forwarded e-mail
    {
        //  Is this the start of a new e-Mail ?
        if (    ( strncmp( start_p, SRCH_SOURCE, SRCH_SOURCE_L ) == 0 )
             || ( strncmp( start_p, SRCH_PGO,    SRCH_PGO_L    ) == 0 ) )
        {
            //  YES:    Change the return code
            email_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Test the input text line to see if it's a multipart boundary.
 *
 *  @param  data_p              Pointer to a line of text data.
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

enum    boundary_type_e
email_is_boundary(
    char                        *   data_p
    )
{
    /**
     * @param email_rc          Return code for this function               */
    int                             email_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;
    /**
     * @param boundary_p        Pointer to the start of 'boundary='         */
    char                        *   boundary_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a boundary tag
    email_rc = BT_SEARCHING;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Is this a Boundary tag ?
     ************************************************************************/

    //  Is there anything to search ?
    if ( text_is_blank_line( start_p ) == false )
    {
        //  Look for the 'boundary=' tag here ?
        boundary_p = strcasestr( start_p, SRCH_BOUNDARY );

        //  Was a boundary tag found ?
        if ( boundary_p != NULL )
        {

            /****************************************************************
             *  Format 1:   boundary="_1af2321e-e051-4911-833c-77b9b34c619e_"
             ****************************************************************/

            //  YES:    Position the pointer to the start of the boundary identifier
            start_p = strchr( boundary_p, '"' );

            //  Did we find it ?
            if ( start_p != NULL )
            {
                //  YES:    Will it fit in the buffer ?
                if ( strlen( &start_p[ 1 ] ) <= ( EMAIL_BOUNDARY_L - 1 ) )
                {
                    //  YES:    Save it.
                    strcpy( email_boundary, &start_p[ 1 ] );

                    //  Remove the ending quote ["]
                    start_p = strchr( email_boundary, '"' );
                    if ( start_p != NULL )
                    {
                        start_p[ 0 ] = '\0';
                    }

                    //  Found it and saved it.  All is good!
                    email_rc = BT_BOUNDARY;
                }
            }

            /****************************************************************
             *  Format 2:   boundary cf300fb1c1e09cec04c0942d07
             ****************************************************************/

            else
            {
                //  YES:    Position the pointer to the past the tag
                start_p = &boundary_p[ SRCH_BOUNDARY_L ];

                //  Move past all white space
                start_p = text_skip_past_whitespace( start_p );

                //  Is there anything here ?
                if ( text_is_blank_line( start_p ) == false )
                {
                    //  YES:    Will it fit in the buffer ?
                    if ( strlen( start_p ) <= ( EMAIL_BOUNDARY_L - 1 ) )
                    {
                        //  YES:    Save it.
                        strcpy( email_boundary, start_p );

                        //  Found it and saved it.  All is good!
                        email_rc = BT_BOUNDARY;
                    }
                }

                /************************************************************
                 *  Format 3:   boundary
                 ************************************************************/

                else
                {
                    //  NO:     Found the 'boundary tag but not the identifier.
                    email_rc = BT_NO_IDENTIFIER;
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Test the input text line to see if it's a multipart boundary.
 *
 *  @param  data_p              Pointer to a line of text data.
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

enum    boundary_type_e
email_is_boundary_identifier(
    char                        *   data_p
    )
{
    /**
     * @param email_rc          Return code for this function               */
    int                             email_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a boundary tag
    email_rc = BT_NO_IDENTIFIER;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Could this be the boundary identifier
     ************************************************************************/

    //  Is there anything to search ?
    if ( text_is_blank_line( start_p ) == false )
    {
        //  YES:    Is the length correct ?
        if ( strlen( data_p ) == 30 )
        {
            //  YES:    Does it start with two dashes [--] ?
            if (    ( data_p[ 0 ] == '-' )
                 && ( data_p[ 1 ] == '-' ) )
            {
                //  YES:    This must be it so save it.
                strcpy( email_boundary, &start_p[ 2 ] );

                //  Found it and saved it.  All is good!
                email_rc = BT_BOUNDARY;
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Test the input text line to see if it is a multi-part message break
 *
 *  @param  data_p              Pointer to a line of text data.
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

int
email_is_multipart_break(
    char                        *   data_p
    )
{
    /**
     * @param email_rc          Return code for this function               */
    int                             email_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a match for the current boundary
    email_rc = false;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Does the line start with two dash [-] characters ?
    if (    ( strlen( email_boundary ) >   0  )
         && ( start_p[ 0 ]             == '-' )
         && ( start_p[ 1 ]             == '-' ) )
    {
        //  Do we have a boundary marker match ?
        if ( strncmp( &start_p[ 2 ], email_boundary, strlen( email_boundary ) ) == 0 )
        {
            //  YES:    Set a good return code.
            email_rc = true;

            //  Is this also the end of the current multipart ?
            if (    ( data_p[ strlen( data_p ) -2 ] == '-' )
                 && ( data_p[ strlen( data_p ) -1 ] == '-' ) )
            {
                //  YES:    Erase the existing boundary marker
                memset( email_boundary, '\0', EMAIL_BOUNDARY_L );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains an e-Mail group break.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      ------------------------------
 *      --------------- MESSAGE bread-bakers.v096.n027.1 ---------------
 *      --------------- END bread-bakers.v096.n042 ---------------
 *      ________________________________________________________________________
 *
 ****************************************************************************/

int
email_is_group_break(
    char                        *   data_p
    )
{
    /**
     * @param email_flag        A mark on the wall if wr are doing an e-Mail*/
    int                             email_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'from' information.
    email_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the start of a new e-Mail ?
    if (    (    ( strncmp( tmp_data_p, "------------------------------", 30 ) ==  0 )
              && ( strlen( tmp_data_p )                                        == 30 ) )
         || (    ( strncmp( tmp_data_p, "______________________________", 30 ) ==  0 )
              && ( strlen( tmp_data_p )                                        == 30 ) )
         || (    ( strncmp( tmp_data_p, "----------------------------------------------------------------------", 70 ) == 0 )
              && ( strlen( tmp_data_p )                                        == 70 ) )
         || (    ( strncmp( tmp_data_p, "________________________________________________________________________", 72 ) == 0 )
              && ( strlen( tmp_data_p )                                        == 72 ) )
//  @note:  Removed because it occurs frequently inside recipes resulting
//          in the recipe being split (sometimes multiple times).
//       || (    ( strncmp( tmp_data_p, "-----"                         ,  5 ) ==  0 )
//            && ( strlen( tmp_data_p )                                        ==  5 ) )
         || (    ( strncmp( tmp_data_p, "........................................", 40 ) ==  0 )
              && ( strlen( tmp_data_p )                                        == 40 ) )
         || ( strncmp( tmp_data_p, "========================  Arch",      30 ) == 0 )
         || ( strncmp( tmp_data_p, "--------------- MESSAGE bread-",      30 ) == 0 )
         || ( strncmp( tmp_data_p, "--------------- END bread-bake",      30 ) == 0 )
         || ( email_is_multipart_break( tmp_data_p )                           == true ) )
    {
        //  YES:    Change the return code
        email_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Discover the 'Content-Type:' of the current text string.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return content_type        CT_NONE       = Content-Type: not found
 *                              CT_TEXT_PLAIN = text/plain;
 *                              CT_TEXT_HTML  = text/html;
 *                              CT_MP_ALT     = multipart/alternative;
 *                              CT_UNKNOWN    = unknown type
 *
 *  @note
 *
 ****************************************************************************/

enum    content_type_e
email_find_content(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;
    /**
     * @param content_type      Decoded content type.                       */
    enum    content_type_e          content_type;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    //  The assumption is that this is that the text string does NOT contain.
    content_type = CT_NONE;

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Content-Type: ?
    if ( strncasecmp( tmp_data_p, SRCH_CONTENT_TYPE, SRCH_CONTENT_TYPE_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_CONTENT_TYPE_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        /********************************************************************
         *  Discover the content type
         ********************************************************************/

        //  text/html ?
        if ( strncasecmp( tmp_data_p, SRCH_CT_T_HTML, SRCH_CT_T_HTML_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_TEXT_HTML;
        }

        //  text/ ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_TEXT, SRCH_CT_TEXT_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_TEXT;
        }

        //  multipart/ ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_MULTIPART, SRCH_CT_MULTIPART_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_MULTIPART;
        }

        //  application/x-ygp-stripped ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_APPLICATION, SRCH_CT_APPLICATION_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_APPLICATION;
        }

        //  image/ ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_IMAGE, SRCH_CT_IMAGE_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_IMAGE;
        }

        //  video/ ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_VIDEO, SRCH_CT_VIDEO_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_VIDEO;
        }

        //  audio/ ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_AUDIO, SRCH_CT_AUDIO_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_AUDIO;
        }

        //  message/ ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_MESSAGE, SRCH_CT_MESSAGE_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_MESSAGE;
        }

        //  unknown/ ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CT_UNKNOWN, SRCH_CT_UNKNOWN_L ) == 0 )
        {
            //  YES:    Set the return type code
            content_type = CT_UNKNOWN;
        }

        //  The content type is unknown.
        else
        {
            //  YES:    Set the return type code
            content_type = CT_UNKNOWN;

            //  STOP and display the string we didn't decode.
            log_write( MID_WARNING, "email_find_content",
                          "[%s]\n", data_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( content_type );
}

/****************************************************************************/
/**
 *  Discover the 'Content-Transfer-Encoding:' of the current text string.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return encoding_type       CTE_NONE        Content-Transfer-Encoding: not found
 *                              CTE_QUOTE_PRINT quoted-printable
 *                              CTE_7BIT        7BIT
 *                              CTE_8BIT        8BIT
 *                              CTE_BASE64      BASE64
 *                              CTE_UNKNOWN     An unknown type was found
 *
 *  @note
 *
 ****************************************************************************/

enum    encoding_type_e
email_find_encoding(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;
    /**
     * @param encoding_type     Decoded encoding type.                      */
    enum    encoding_type_e         encoding_type;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    //  The assumption is that this is that the text string does NOT contain.
    encoding_type = CTE_NONE;

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Content-Transfer-Encoding: ?
    if ( strncasecmp( tmp_data_p, SRCH_ENCODING_TYPE, SRCH_ENCODING_TYPE_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_ENCODING_TYPE_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        /********************************************************************
         *  Discover the encoding type
         ********************************************************************/

        //  quoted-printable ?
        if ( strncasecmp( tmp_data_p, SRCH_CTE_QUOTE_PRINT, SRCH_CTE_QUOTE_PRINT_L ) == 0 )
        {
            //  YES:    Set the return type code
            encoding_type = CTE_QUOTE_PRINT;
        }

        //  7BIT ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CTE_7BIT, SRCH_CTE_7BIT_L ) == 0 )
        {
            //  YES:    Set the return type code
            encoding_type = CTE_7BIT;
        }

        //  8BIT ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CTE_8BIT, SRCH_CTE_8BIT_L ) == 0 )
        {
            //  YES:    Set the return type code
            encoding_type = CTE_8BIT;
        }

        //  BASE64 ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CTE_BASE64, SRCH_CTE_BASE64_L ) == 0 )
        {
            //  YES:    Set the return type code
            encoding_type = CTE_BASE64;
        }

        //  BINARY ?
        else
        if ( strncasecmp( tmp_data_p, SRCH_CTE_BINARY, SRCH_CTE_BINARY_L ) == 0 )
        {
            //  YES:    Set the return type code
            encoding_type = CTE_BINARY;
        }

        //  The encoding type is unknown.
        else
        {
            //  YES:    Set the return type code
            encoding_type = CTE_UNKNOWN;

            //  STOP and display the string we didn't decode.
            log_write( MID_WARNING, "email_find_encoding",
                          "[%s]\n", data_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( encoding_type );
}

/****************************************************************************/
/**
 *  Search the text line for Yahoo news group source data.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return tmp_data_p          NULL if no 'source' information, else a
 *                              pointer to the source data.
 *
 *  @note
 *      Mailing-List: list MC_recipes@onelist.com; contact MC_recipes-owner@onelist.com
 *      Sender: bread-bakers-errors@lists.best.com
 *
 ****************************************************************************/

char    *
email_find_source(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  @NOTE:  "Deliver-To:" can be used for some early e-Mails but is
    //          is used as the "To:" later.  This dual use makes it impossible
    //          to use

    //  X-Apparently-To: ?
    if ( strncmp( tmp_data_p, SRCH_X_TO, SRCH_X_TO_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_X_TO_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    //  Mailing-List: ?
    else
    if ( strncmp( tmp_data_p, SRCH_MAILINGLIST, SRCH_MAILINGLIST_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_MAILINGLIST_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    //  Sender: ?
    else
    if ( strncmp( tmp_data_p, SRCH_SENDER, SRCH_SENDER_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_SENDER_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains where the e-Mail came from.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      From: 'Jane Doe' jane.doe@server.mail (Jane Doe)
 *
 ****************************************************************************/

char    *
email_find_from(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  From: ?
    if ( strncmp( tmp_data_p, SRCH_FROM, SRCH_FROM_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_FROM_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains when the e-Mail was sent.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      Date: Tue, 7 Mar 2000 10:18:45 -0800 (PST)
 *
 ****************************************************************************/

char    *
email_find_datetime(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  From: ?
    if ( strncmp( tmp_data_p, SRCH_DATETIME, SRCH_DATETIME_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_DATETIME_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains an e-Mail subject.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

char    *
email_find_subject(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  From: ?
    if ( strncmp( tmp_data_p, SRCH_SUBJECT, SRCH_SUBJECT_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_SUBJECT_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Analyze e-Mail messages for stuff that we don't want anything to do with.
 *  When located discard it.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_filtering     TRUE when we are processing stuff that is
 *                              considered to be trash, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

int
email_filter(
    char                        *   data_p
    )
{
    /**
     * @param email_rc          Return code for this function               */
    int                             email_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Was the last line from a reply message ?
    if ( email_filter_type == EMAIL_FILTER_REPLY )
    {
        //  YES:    Throw it in the trash
        email_filter_type = EMAIL_FILTER_NONE;
    }

    /************************************************************************
     *  Actively filtering
     ************************************************************************/

    switch( email_filter_type )
    {
        case    EMAIL_FILTER_X_YMAILISG:
        {
            //  Is this part of the current block ?
            if ( data_p[ 0 ] != ' ' )
            {
                //  NO:     Set the return code.
                email_filter_type = EMAIL_FILTER_NONE;
            }
        }   break;

        case    EMAIL_FILTER_BASE64:
        {
            //  Is this the end of the BASE-64 data ?
            if ( strstr( data_p, "--" ) != NULL )
            {
                //  YES:    Set the return code.
                email_filter_type = EMAIL_FILTER_NONE;
            }
        }   break;

        default:
        {
            /**
             *  @param  tmp_data        Temporary data buffer               */
            char                            tmp_data[ 100 ];

            //  Is the data length too long for a compare match ?
            if ( strlen( data_p ) < sizeof( tmp_data ) )
            {
                //  NO:     Copy the data to our temporary buffer
                memset( tmp_data, '\0', sizeof( tmp_data ) );
                strncpy( tmp_data, data_p, sizeof( tmp_data ) - 1 );

                //  Make the temporary data string all lowercase
                text_to_lowercase( tmp_data );

                //  Yahoo Mail  ?
                if ( strncmp( tmp_data, "x-ymailisg:", 11 ) == 0 )
                {
                    //  YES:    Set the filter type.
                    email_filter_type = EMAIL_FILTER_X_YMAILISG;
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Set the return code
    if ( email_filter_type == EMAIL_FILTER_NONE )
    {
        email_rc = false;
    }
    else
    {
        email_rc = true;
    }

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Initialize the Translations tables.
 *
 *  @param  void            *   Pointer
 *
 *  @return void                Upon successful completion TRUE is returned
 *                              else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

void
email(
    void                    *   void_p
    )
{
    /**
     *  @param  tcb_p           Pointer to a Thread Control Block           */
    struct  tcb_t           *   tcb_p;
    /**
     *  @param  rcb_p           Pointer to a Recipe Control Block           */
    struct  rcb_t           *   rcb_p;
#if ! STUB
    /**
     *  @param  new_rcb_p       Pointer to a Recipe Control Block           */
    struct  rcb_t           *   new_rcb_p;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   list_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                         list_lock_key;
    /**
     * @param encoding_type     TRUE = encoding_type                        */
    int                         quoted_printable;
    /**
     * @param email_start_flag  TRUE = e-mail processing                    */
    int                         email_start_flag;
    /**
     * @param tmp_data_p        Temporary data pointer                      */
    char                    *   tmp_data_p;
    /**
     *  @param  recipe_format   Format code for this recipe                 */
    enum    recipe_format_e     tmp_format;
#endif

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize variables
    quoted_printable = false;

    //  Set the pointer
    tcb_p = void_p;

    //  Progress report.
    log_write( MID_INFO, tcb_p->thread_name,
               "Initialization complete.\n" );

    //  Change execution state to "WAIT"ing for work.
    tcb_p->thread_state = TS_WAIT;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    while ( 1 )
    {

        /********************************************************************
         *  Get a new file
         ********************************************************************/

        //  Get the current File-ID.
        rcb_p = queue_get_payload( tcb_p->queue_id );

        //  This thread is now the owner of the RCB
        rcb_p->tcb_p = tcb_p;

        //  Progress report.
        log_write( MID_LOGONLY, tcb_p->thread_name,
                   "SIZE: %10d - FILE-ID: %s\n",
                   list_query_count( rcb_p->import_list_p ),
                   rcb_p->file_path );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

#if ! STUB

        //  Clear the new RCB pointer
        new_rcb_p = NULL;

        //  Initialize the e-Mail filter type
        email_filter_type = EMAIL_FILTER_NONE;

        /********************************************************************
         *  FUNCTIONAL CODE FOR THIS THREAD GOES HERE
         ********************************************************************/

        //  Lock the list for fast(er) access
        list_lock_key = list_user_lock( rcb_p->import_list_p );

        //  Scan the list
        for( list_data_p = list_fget_first( rcb_p->import_list_p, list_lock_key );
             list_data_p != NULL;
             list_data_p = list_fget_next( rcb_p->import_list_p, list_data_p, list_lock_key ) )
        {
            //  Remove the data from the level 1 list
            list_fdelete( rcb_p->import_list_p, list_data_p, list_lock_key );

            //  Are we filtering out e-Mail junk (images etc.)
            if ( email_filter( list_data_p ) == true )
            {
                //  YES:    Discard the data
                mem_free( list_data_p );
            }
            else
            {
                //  Test for the start of an e-Mail thread
                if (    ( rcb_p->recipe_format == RECIPE_FORMAT_NONE )
                     && ( EMAIL__is_start( list_data_p ) == true ) )
                {
                    email_start_flag = true;
                }

                //  Are we processing an e-Mail message ?
                if (    ( email_start_flag     ==               true )
                     && ( rcb_p->recipe_format == RECIPE_FORMAT_NONE ) )
                {
                    //  "TRANSFER-ENCODING"
                    if ( email_find_encoding( list_data_p ) == CTE_QUOTE_PRINT )
                    {
                        //  YES:    Set the decode flag
                        quoted_printable = true;
                    }
                    //  "NEWSGROUPS:"
                    tmp_data_p = EMAIL__find_newsgroup( list_data_p );
                    if ( tmp_data_p != NULL )
                    {
                        //  YES:    Save the information
                        memset( rcb_p->email_info_p->g_from, '\0', FROM_L );
                        if ( strlen( tmp_data_p ) < FROM_L )
                            memcpy( rcb_p->email_info_p->g_from,
                                    tmp_data_p, strlen( tmp_data_p ) );
                        else
                            memcpy( rcb_p->email_info_p->g_from,
                                    tmp_data_p, FROM_L - 1 );
                    }
                    //  "SUBJECT:"
                    tmp_data_p = EMAIL__find_subject( list_data_p );
                    if ( tmp_data_p != NULL )
                    {
                        //  YES:    Save the information
                        memset( rcb_p->email_info_p->e_subject, '\0', SUBJECT_L );
                        if ( strlen( tmp_data_p ) < FROM_L )
                            memcpy( rcb_p->email_info_p->e_subject,
                                    tmp_data_p, strlen( tmp_data_p ) );
                        else
                            memcpy( rcb_p->email_info_p->e_subject,
                                    tmp_data_p, SUBJECT_L - 1 );
                    }
                    //  "FROM:"
                    tmp_data_p = EMAIL__find_from( list_data_p );
                    if ( tmp_data_p != NULL )
                    {
                        //  YES:    Save the information
                        memset( rcb_p->email_info_p->e_from, '\0', FROM_L );
                        if ( strlen( tmp_data_p ) < FROM_L )
                            memcpy( rcb_p->email_info_p->e_from,
                                    tmp_data_p, strlen( tmp_data_p ) );
                        else
                            memcpy( rcb_p->email_info_p->e_from,
                                    tmp_data_p, FROM_L - 1 );
                    }
                    //  "DATE:"
                    tmp_data_p = EMAIL__find_datetime( list_data_p );
                    if ( tmp_data_p != NULL )
                    {
                        //  YES:    Save the information
                        memset( rcb_p->email_info_p->e_datetime, '\0', DATETIME_L );
                        if ( strlen( tmp_data_p ) < FROM_L )
                            memcpy( rcb_p->email_info_p->e_datetime,
                                    tmp_data_p, strlen( tmp_data_p ) );
                        else
                            memcpy( rcb_p->email_info_p->e_datetime,
                                    tmp_data_p, DATETIME_L - 1 );
                    }
                }

                //  Is there an active recipe split ?
                if ( rcb_p->recipe_format == RECIPE_FORMAT_NONE )
                {
                    //  NO:     Test for the start of a new recipe.
                    rcb_p->recipe_format = recipe_is_start( list_data_p );

                    //  Is this the start of a new recipe ?
                    if ( rcb_p->recipe_format != RECIPE_FORMAT_NONE )
                    {
                        //  YES:    Clone the RCB
                        new_rcb_p = rcb_new( rcb_p );

                        //  Add this data buffer to the new recipe list.
                        list_put_last( new_rcb_p->import_list_p, list_data_p );
                    }
                    else
                    {
                        //  This is trash data so throw it away
                        mem_free( list_data_p );
                    }
                }
                else
                {
                    //  Just in case this is the start of a new recipe
                    tmp_format = recipe_is_start( list_data_p );

                    //  Is this the end of the recipe
                    if (    ( recipe_is_end( rcb_p->recipe_format, list_data_p ) == true )
                         || ( tmp_format                           != RECIPE_FORMAT_NONE )
                         || ( EMAIL__is_group_break( list_data_p )               == true ) )
                    {
                        //  YES:    Is this a QUOTED-PRINTABLE e-Mail ?
                        if ( quoted_printable == true )
                        {
                            //  YES:    Decode the quoted printable text
                            EMAIL__quoted_printable( new_rcb_p );

                            //  Set the decode flag
                            quoted_printable = false;
                        }

                        //  Put it in one of the DECODE queue
                        queue_put_payload( decode_tcb->queue_id, new_rcb_p  );

                        //  Clear the current recipe format
                        rcb_p->recipe_format = RECIPE_FORMAT_NONE;

                        //  Clear the new RCB pointer
                        new_rcb_p = NULL;
                    }


                    //  Is this the start of a recipe ?
                    if ( tmp_format != RECIPE_FORMAT_NONE )
                    {
                       //  YES:    Set the new recipe format
                        rcb_p->recipe_format = tmp_format;

                        //  Clone the RCB
                        new_rcb_p = rcb_new( rcb_p );
                    }

                    //  Is there a RCB to store the data ?
                    if ( new_rcb_p != NULL )
                    {
                        //  YES:    Add this data buffer to the current recipe list.
                        list_put_last( new_rcb_p->import_list_p, list_data_p );
                    }
                }
            }
        }

        //  End-Of-File with an active recipe ?
        if ( new_rcb_p != NULL )
        {
            //  YES:    Is this a QUOTED-PRINTABLE e-Mail ?
            if ( quoted_printable == true )
            {
                //  YES:    Decode the quoted printable text
                EMAIL__quoted_printable( new_rcb_p );

                //  Set the decode flag
                quoted_printable = false;
            }

            //  Put it in one of the DECODE queue
            queue_put_payload( decode_tcb->queue_id, new_rcb_p  );

            //  Clear the new RCB pointer
            new_rcb_p = NULL;
        }

        //  Release the lock on the level 3 list
        list_user_unlock( rcb_p->import_list_p, list_lock_key );

#endif

        //  Kill the Recipe Control Block
        rcb_kill( rcb_p );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/