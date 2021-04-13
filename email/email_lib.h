/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef EMAIL_LIB_H
#define EMAIL_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal
 *  library components of the 'email' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_EMAIL
   #define EMAIL_EXT
#else
   #define EMAIL_EXT          extern
#endif

/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Private Definitions
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
 * Library Private Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    content_type_e
{
    CT_NONE                     =   0,      //  Content-Type: not found
    CT_TEXT_HTML                =   1,      //  text/html
    CT_TEXT                     =   2,      //  text/
    CT_MULTIPART                =   3,      //  multipart/
    CT_APPLICATION              =   4,      //  application/
    CT_IMAGE                    =   5,      //  image/
    CT_VIDEO                    =   6,      //  video/
    CT_AUDIO                    =   7,      //  audio/
    CT_MESSAGE                  =   8,      //  message/
    CT_UNKNOWN                  =  -1       //  An unknown type was found
};
//----------------------------------------------------------------------------
enum    encoding_type_e
{
    CTE_NONE                    =   0,      //  Content-Transfer-Encoding: not found
    CTE_QUOTE_PRINT             =   1,      //  quoted-printable
    CTE_7BIT                    =   2,      //  7BIT
    CTE_8BIT                    =   3,      //  8BIT
    CTE_BASE64                  =   4,      //  BASE64
    CTE_BINARY                  =   5,      //  BINARY
    CTE_UNKNOWN                 =  -1       //  An unknown type was found
};
//----------------------------------------------------------------------------
enum    boundary_type_e
{
    BT_NOT_SEARCHING            =   0,      //  NOT searching for a boundary
    BT_SEARCHING                =   1,      //  Searching for a boundary
    BT_BOUNDARY                 =   2,      //  Boundary Found
    BT_NO_IDENTIFIER            =   3       //  Identifier NOT FOUND
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
EMAIL_EXT
int                             FooBar;
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
int
EMAIL__is_start(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
enum    boundary_type_e
EMAIL__is_boundary(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
enum    boundary_type_e
EMAIL__is_boundary_identifier(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
int
EMAIL__is_multipart_break(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
int
EMAIL__is_group_break(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
enum    content_type_e
EMAIL__find_content(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
enum    encoding_type_e
EMAIL__find_encoding(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
char    *
EMAIL__find_source(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
char    *
EMAIL__find_from(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
char    *
EMAIL__find_datetime(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
char    *
EMAIL__find_subject(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
int
EMAIL__filter(
    char                    *   data_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    EMAIL_LIB_H
