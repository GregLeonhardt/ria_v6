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
 *  library components of the 'decode' library.
 *
 *  @note
 *
 *  @ToDo: 3 "Recipe from" bla bla bla bla
 *  @ToDo: 3 "Posted to" bla bla bla bla
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE            ( "ALLOCATE STORAGE FOR DECODE" )

#define STUB                    ( 0 )

#define _GNU_SOURCE             //  See feature_test_macros(7)

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
#include <stdlib.h>             //  ANSI standard library.
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
#include "decode_api.h"         //  API for all decode_*            PUBLIC
#include "decode_lib.h"         //  API for all DECODE__*           PRIVATE
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
#define RECIPE_ID_L             ( 17 )
#define MMF_INGRED_L            ( 30 )
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
 *  Format the date/time string.
 *
 *  @param  src_date_time_p Pointer to source date/time string.
 *
 *  @return                 Pointer to a formatted date/time string, or NULL
 *                          when a valid formatted date string is not detected.
 *
 *  @note
 *      DOW     =   Day Of Week         text
 *      MON     =   Month               text
 *      AP      =   AM or PM            text
 *      YY      =   Year                numeric
 *      YYY     =   Century & Year      numeric
 *      MM      =   Month               numeric
 *      DD      =   Day (of month)      numeric
 *      HH      =   Hour                numeric
 *      MM      =   Minute              numeric
 *      SS      =   Second              numeric
 *
 *      Returns:    YYYY-MM-DD HH:MM:SS
 *
 *  @note
 *      Formats:                                            SEQUENCE-ID
 *          YY MM DD HH MM SS                                   #01
 *          MM DD YY HH MM   (AP)                               #02
 *          MM DD YY                                            #03
 *          DD MON YY HH MM SS                                  #04
 *          DOW     DD MON YY HH MM SS AP                       #05
 *          DOW     DD MON YY HH MM    AP                       #06
 *          DOW     DD MON YY HH MM( SS)                        #07
 *          DOW MON DD     YY HH MMam                           #08
 *          DOW MON DD     YY HH MM                             #09
 *          DOW MON DD        HH MM SS YY                       #10
 *          DOW MON DD     YY                                   #11
 *              MON DD     YY HH MM    AP                       #12
 *
 *  @note
 *      The following formats have been discovered but as there is no
 *      year specified.
 *      Should I use the file date (year) as a default ?
 *      I will not be creating a decode for them:
 *              09/14  1:22 AM
 *              Jan 02 01:47PM -0800
 *              02/27 From:   ASXV66A    JAMES KILGORE         Time:    12:01
 *
 *  @ToDo   3   Newly discovered Date/Time formats
 *      Saturday, November 02, 1996 1119
 *          My assumption is that 1119 should be the time 11:19
 *
 ****************************************************************************/

char  *
decode_fmt_datetime(
    char                    *   src_datetime_p
    )
{
    /**
     *  @param  tmp_date_p      Pointer to the temp date string             */
    char                    *   tmp_datetime_p;
    /**
     *  @param  fmt_date_p      Pointer to the formatted date string        */
    char                    *   fmt_datetime_p;
    /**
     *  @param  decoded         TRUE Dtae & Time have been decoded          */
    int                         decoded;
    /**
     *  @param                  Date & Time values                          */
    int                         dt_dow;
    int                         dt_year;
    int                         dt_month;
    int                         dt_day;
    int                         dt_hour;
    int                         dt_minute;
    int                         dt_second;
    int                         dt_ap;
    /**
     *  @param  dow             Day-Of-Week                                 */
    char                        dow[ 10 ];
    /**
     *  @param  month           Month abbreviation                          */
    char                        month[ 10 ];
    /**
     *  @param  ap              AM or PM                                    */
    char                        ap[ 50 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
    decoded = false;

    //  Assume there isn't a valid date string present
    fmt_datetime_p = NULL;
    dt_dow = dt_year = dt_month = dt_day = dt_hour = dt_minute = dt_second = dt_ap     = -1;

    //  Clear the text buffers
    memset( dow,   '\0', sizeof( dow   ) );
    memset( month, '\0', sizeof( month ) );
    memset( ap,    '\0', sizeof( ap    ) );

    /************************************************************************
     *  Copy the Date/Time string and remove punctuation characters
     ************************************************************************/

    //  Allocate a temporary data buffer
    tmp_datetime_p = mem_malloc( strlen( src_datetime_p ) + 1 );

    //  Copy the source Date/Time into the temporary buffer
    memcpy( tmp_datetime_p, src_datetime_p, strlen( src_datetime_p ) );

    //  ":"
    fmt_datetime_p = tmp_datetime_p;
    do
    {
        //  Scan for a 'Colon' ':' character
        fmt_datetime_p = strchr( fmt_datetime_p, ':' );
        if ( fmt_datetime_p != NULL ) fmt_datetime_p[ 0 ] = ' ';
    }   while( fmt_datetime_p != NULL );

    //  "/"
    fmt_datetime_p = tmp_datetime_p;
    do
    {
        //  Scan for a 'Slash' '/' character
        fmt_datetime_p = strchr( fmt_datetime_p, '/' );
        if ( fmt_datetime_p != NULL ) fmt_datetime_p[ 0 ] = ' ';
    }   while( fmt_datetime_p != NULL );

    //  "-"
    fmt_datetime_p = tmp_datetime_p;
    do
    {
        //  Scan for a 'Minus' '-' character
        fmt_datetime_p = strchr( fmt_datetime_p, '-' );
        if ( fmt_datetime_p != NULL ) fmt_datetime_p[ 0 ] = ' ';
    }   while( fmt_datetime_p != NULL );

    //  ","
    fmt_datetime_p = tmp_datetime_p;
    do
    {
        //  Scan for a 'Comma' ',' character
        fmt_datetime_p = strchr( fmt_datetime_p, ',' );
        if ( fmt_datetime_p != NULL ) fmt_datetime_p[ 0 ] = ' ';
    }   while( fmt_datetime_p != NULL );

    //  "("
    fmt_datetime_p = tmp_datetime_p;
    do
    {
        //  Scan for a 'Left Parenthesis' '(' character
        fmt_datetime_p = strchr( fmt_datetime_p, '(' );
        if ( fmt_datetime_p != NULL ) fmt_datetime_p[ 0 ] = ' ';
    }   while( fmt_datetime_p != NULL );

    //  ")"
    fmt_datetime_p = tmp_datetime_p;
    do
    {
        //  Scan for a 'Right Parenthesis' ')' character
        fmt_datetime_p = strchr( fmt_datetime_p, ')' );
        if ( fmt_datetime_p != NULL ) fmt_datetime_p[ 0 ] = ' ';
    }   while( fmt_datetime_p != NULL );


    /************************************************************************
     *  Decode the Date / Time string
     ************************************************************************/

    //  Is the source date string long enough to contain a valid date ?
    if ( strlen( tmp_datetime_p ) >= 5 )
    {
        //  YES:    Allocate storage for the formatted date.
        fmt_datetime_p = mem_malloc( 20 );

        //--------------------------------------------------------------------
        //  Source format   YY MM DD HH MM SS                              #01
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%d %d %d %d %d %d",
                    &dt_year  , &dt_month , &dt_day   ,
                    &dt_hour  , &dt_minute, &dt_second );

            //  Verify a successful decode
            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second != -1 )
                 && ( dt_dow    == -1 )
                 && ( dt_ap     == -1 ) )
            {
                //  YES:    Good decode
                decoded = true;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   MM DD YY HH MM   (AP)                          #02
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%d %d %d %d %d %s",
                    &dt_month , &dt_day   , &dt_year  ,
                    &dt_hour  , &dt_minute, ap         );

            //  Convert the month string to an integer
            dt_ap = DECODE__am_or_pm( ap );

            //  Verify a successful decode
            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second == -1 )
                 && ( dt_dow    == -1 )
                 && ( dt_ap     != -2 ) )   //  May or may-not be present
            {
                //  YES:    Good decode
                decoded = true;

                //  Seconds isn't included in this format.
                dt_second = 0;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   MM DD YY                                       #03
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%d %d %d %d %d %s",
                    &dt_month , &dt_day   , &dt_year  ,
                    &dt_hour  , &dt_minute, ap         );

            //  Convert the month string to an integer
            dt_ap = DECODE__am_or_pm( ap );

            //  Verify a successful decode
            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   == -1 )
                 && ( dt_minute == -1 )
                 && ( dt_second == -1 )
                 && ( dt_dow    == -1 )
                 && ( dt_ap     ==  0 ) )
            {
                //  YES:    Good decode
                decoded = true;

                //  There isn't a time with this format so use '00:00:00'
                dt_hour   = 0;
                dt_minute = 0;
                dt_second = 0;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DD MON YY HH MM SS                             #04
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%d %s %d %d %d %d",
                    &dt_day   ,  month    , &dt_year  ,
                    &dt_hour  , &dt_minute, &dt_second );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second != -1 )
                 && ( dt_dow    == -1 )
                 && ( dt_ap     == -1 ) )
            {
                //  YES:    Good decode
                decoded = true;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DOW     DD MON YY HH MM SS AP                  #05
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %d %s %d %d %d %d %s",
                    dow,
                    &dt_day   , month     , &dt_year  ,
                    &dt_hour  , &dt_minute, &dt_second, ap       );

            //  Convert the month string to an integer
            dt_ap = DECODE__am_or_pm( ap );

            //  Convert the month string to an integer
            dt_dow = DECODE__day_of_week( dow );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second != -1 )
                 && ( dt_dow    >   0 )
                 && ( dt_ap     >   0 ) )
            {
                //  YES:    Good decode
                decoded = true;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DOW     DD MON YY HH MM    AP                  #06
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %d %s %d %d %d %s",
                    dow,
                    &dt_day   , month     , &dt_year,
                    &dt_hour  , &dt_minute, ap       );

            //  Convert the month string to an integer
            dt_ap = DECODE__am_or_pm( ap );

            //  Convert the month string to an integer
            dt_dow = DECODE__day_of_week( dow );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second == -1 )
                 && ( dt_dow    >   0 )
                 && ( dt_ap     >   0 ) )
            {
                //  YES:    Good decode
                decoded = true;

                //  Seconds isn't included in this format.
                dt_second = 0;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DOW     DD MON YY HH MM( SS)                   #07
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %d %s %d %d %d %d",
                    dow,
                    &dt_day   ,  month    , &dt_year  ,
                    &dt_hour  , &dt_minute, &dt_second );

            //  Convert the month string to an integer
            dt_dow = DECODE__day_of_week( dow );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
//               && ( dt_second != -1 )     //  Seconds may or MAY NOT exist
                 && ( dt_dow    >   0 )
                 && ( dt_ap     == -1 ) )
            {
                //  YES:    Good decode
                decoded = true;

                //  Was the seconds included ?
                if ( dt_second == -1 )
                {
                    //  YES:    Set it to zero
                    dt_second = 0;
                }
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DOW MON DD     YY HH MMam                      @08
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %s %d %d %d %d%s",
                    dow,
                    month     , &dt_day   ,   &dt_year  ,
                    &dt_hour  , &dt_minute,   ap         );

            //  Convert the month string to an integer
            dt_ap = DECODE__am_or_pm( ap );

            //  Convert the month string to an integer
            dt_dow = DECODE__day_of_week( dow );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second == -1 )
                 && ( dt_dow    >   0 )
                 && ( dt_ap     >   0 ) )
            {
                //  YES:    Good decode
                decoded = true;

                //  Seconds isn't included in this format.
                dt_second = 0;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DOW MON DD     YY HH MM                        @09
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %s %d %d %d %d %d",
                    dow,
                    month     , &dt_day   , &dt_year   ,
                    &dt_hour  , &dt_minute, &dt_second );

            //  Convert the month string to an integer
            dt_dow = DECODE__day_of_week( dow );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second == -1 )
                 && ( dt_dow    >   0 )
                 && ( dt_ap     == -1 ) )
            {
                //  YES:    Good decode
                decoded = true;

                //  There isn't a seconds position for this format
                dt_second = 0;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DOW MON DD        HH MM SS YY                  @10
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %s %d %d %d %d %d",
                    dow,
                    month     , &dt_day   , &dt_hour  ,
                    &dt_minute, &dt_second, &dt_year    );

            //  Convert the month string to an integer
            dt_dow = DECODE__day_of_week( dow );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second != -1 )
                 && ( dt_dow    >   0 )
                 && ( dt_ap     == -1 ) )
            {
                //  YES:    Good decode
                decoded = true;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format   DOW MON DD     YY                              #11
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %s %d %d %d",
                    dow,
                    month     , &dt_day   , &dt_year   ,
                    &dt_hour                            );

            //  Convert the month string to an integer
            dt_dow = DECODE__day_of_week( dow );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   == -1 )
                 && ( dt_minute == -1 )
                 && ( dt_second == -1 )
                 && ( dt_dow    >   0 )
                 && ( dt_ap     == -1 ) )
            {
                //  YES:    Good decode
                decoded = true;

                //  Fill in the missing time
                dt_hour   = 0;
                dt_minute = 0;
                dt_second = 0;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Source format       MON DD     YY HH MM    AP                  @12
        //--------------------------------------------------------------------

        //  Do we have a successful decode yet ?
        if ( decoded == false )
        {
            //  Parse the date / time string
            sscanf( tmp_datetime_p, "%s %d %d %d %d %s",
                    month     , &dt_day   , &dt_year   ,
                    &dt_hour  , &dt_minute, ap          );

            //  Convert the month string to an integer
            dt_ap = DECODE__am_or_pm( ap );

            //  Convert the month string to an integer
            dt_month = DECODE__month( month );

            if (    ( dt_year   != -1 )
                 && ( dt_month  != -1 )
                 && ( dt_day    != -1 )
                 && ( dt_hour   != -1 )
                 && ( dt_minute != -1 )
                 && ( dt_second == -1 )
                 && ( dt_dow    == -1 )
                 && ( dt_ap     >   0 ) )
            {
                //  YES:    Good decode
                decoded = true;

                //  There isn't a seconds position for this format
                dt_second = 0;
            }
            else
            {
                //  NO:     Bad decode
                dt_dow = dt_year = dt_month = dt_day = dt_hour = \
                       dt_minute = dt_second = dt_ap     = -1;
            }
        }

        //--------------------------------------------------------------------
        //  Adjust the hour using AM/PM
        //--------------------------------------------------------------------

        //  Was the decode successful ?
        if ( decoded == true )
        {
            //  Was the time 'PM'
            if ( dt_ap == 2 )
            {
                //  YES:    Add twelve hours to the hour
                dt_hour += 12;

                //  Fix for 12:00 PM
                if ( dt_hour == 24 )
                {
                    //  YES:    Change the hour to 00
                    dt_hour = 0;
                }
            }
        }

        //--------------------------------------------------------------------
        //  Change a two digit year into a four digit year
        //--------------------------------------------------------------------

        //  Was the decode successful ?
        if ( decoded == true )
        {
            //  Do we have a two digit year ?
            if ( dt_year < 1970 )
            {
                //  Do we have a Y2K problem ?
                if ( dt_year >= 100 )
                {
                    //  YES:    Correct the year
                    dt_year += 1900;
                }
                //  YES:    In the twentieth century ?
                else
                if ( dt_year >= 70 )
                {
                    //  YES:    Correct the year
                    dt_year += 1900;
                }
                //  00 = 2000
                else
                {
                    //  Correct the year
                    dt_year += 2000;
                }
            }
        }

        //--------------------------------------------------------------------
        //  Decoding is complete, build the formatted date / time string
        //--------------------------------------------------------------------

        //  Did we successfully decode the date time string
        if (    ( ( 1970 <= dt_year   )                          )
             && ( (    1 <= dt_month  ) && (   12 >= dt_month  ) )
             && ( (    1 <= dt_day    ) && (   31 >= dt_day    ) )
             && ( (    0 <= dt_hour   ) && (   23 >= dt_hour   ) )
             && ( (    0 <= dt_minute ) && (   59 >= dt_minute ) )
             && ( (    0 <= dt_second ) && (   59 >= dt_second ) ) )
        {
            //  YES:    Format the Date/Time string
            snprintf( fmt_datetime_p, 20,
                    "%04d-%02d-%02d %02d:%02d:%02d",
                    dt_year  , dt_month , dt_day   ,
                    dt_hour  , dt_minute, dt_second );

//          log_write( MID_INFO, "decode_D/T", "From: '%s', To: '%s'\n",
//                  src_datetime_p, fmt_datetime_p );
        }
        else
        {
            //  NO:     The date/time format is unknown or
            //          the data is invalid.
            fmt_datetime_p = text_copy_to_new( "1900-01-01 00:00:00" );

            //  Write a warning message
            log_write( MID_WARNING, "decode_fmt_datetime",
                       "Improperly formatted DATE/TIME: '%s'\n",
                       src_datetime_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return the pointer to a formatted date string
    return ( fmt_datetime_p );
}

/****************************************************************************/
/**
 *  Test the data string for the tag string provided.
 *
 *  @param  data_p              Pointer to the test data string.
 *  @param  tag_p               Pointer to the tag to be tested.
 *
 *  @return return_data_p       When the tag string is located, a pointer to
 *                              the first byte of the tag data;  else NULL
 *                              is returned.
 *
 *  @note
 *
 ****************************************************************************/

char    *
decode_is_tag(
    char                        *   data_p,
    char                        *   tag_p
    )
{
    /**
     *  @param  return_data_p       Pointer to the data string              */
    char                        *   return_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Look for the first occurrence of the search tag data.
    return_data_p = strcasestr( data_p, tag_p );

    //  Was the tag string found ?
    if ( return_data_p != NULL )
    {
        //  YES:    Skip over the tag before looking for blank characters
        return_data_p += strlen( tag_p );

        //  Skip past blank characters to what should be a colon [:].
        for ( ;
              isblank( return_data_p[ 0 ] ) != 0;
              return_data_p += 1 );

        //  Is this a colon [:] character ?
        if ( return_data_p[ 0 ] == ':' )
        {
            //  YES:    Fix the pointer and we are done.
            return_data_p += 1;
        }
        else
        {
            //  NO:     This isn't the tag we are looking for
            return_data_p = NULL;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( return_data_p );
}

/****************************************************************************/
/**
 *  Format a Amount Unit Ingredient Preparation line and add the results to
 *  the recipe.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  auip_data_p         Pointer to an AUIP line of text
 *  @param  recipe_format       Indicator of the decode recipe format.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_fmt_auip(
    struct   recipe_t       *   recipe_p,
    char                    *   auip_data_p,
    enum    recipe_format_e     recipe_format
    )
{
    /**
     *  @param  tmp_data_p      Pointer to the local input line buffer      */
    char                        *   save_data_p;
    char                        *   tmp_data_p;
    /**
     *  @param  local_amount    Local character buffer for the amount field */
    char                            local_amount[ SIZE_AMOUNT + 1 ];
    /**
     *  @param  local_unit      Unit field                                  */
    char                            local_unit[ SIZE_UNIT + 1 ];
    /**
     *  @param  local_ingred    Ingredient field                            */
    char                            local_ingred[ SIZE_INGREDIENT + 1 ];
    /**
     *  @param  local_prep      Preparation field                           */
    char                            local_prep[ SIZE_PREPARATION + 1 ];
    /**
     *  @param  tmp_ingredient  Temporary ingredient field                  */
    char                            tmp_ingredient[ MMF_INGRED_L + 1 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Skip over leading spaces and or tabs
    tmp_data_p = text_skip_past_whitespace( auip_data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    // Was this recipe constructed with two AUIP per line ?
    do
    {
        int                             mmf_length;

        //  Initialize the data buffers
        memset( local_amount,   '\0', sizeof( local_amount ) );
        memset( local_unit,     '\0', sizeof( local_unit ) );
        memset( local_ingred,   '\0', sizeof( local_ingred ) );
        memset( local_prep,     '\0', sizeof( local_prep ) );
        memset( tmp_ingredient, '\0', sizeof ( tmp_ingredient ) );

        /********************************************************************
         *  Meal-Master dual column length
         ********************************************************************/

        //  Set the maximum length
        if ( recipe_format == RECIPE_FORMAT_MMF )
        {
            //  For a MealMaster recipe
            mmf_length = MMF_INGRED_L;
        }
        else
        {
            //  For all other recipe formats.
            mmf_length = strlen( tmp_data_p );
        }

        /********************************************************************
         *  A - Amount
         ********************************************************************/

        //  Format the Amount field
        tmp_data_p = DECODE__fmt_amount( tmp_data_p, local_amount, SIZE_AMOUNT );

        /********************************************************************
         *  U - Unit
         ********************************************************************/

        //  Format the Unit field
        if ( strlen( local_amount ) != 0 )
        {
            tmp_data_p = DECODE__fmt_unit( tmp_data_p, local_unit, SIZE_UNIT );
        }

        /********************************************************************
         *  IP - Ingredient & Preparation
         ********************************************************************/

        if ( recipe_format == RECIPE_FORMAT_MMF )
        {
            //  Format the Ingredient field
            save_data_p = tmp_data_p;
            strncpy( tmp_ingredient, tmp_data_p, mmf_length );
            tmp_data_p = &(tmp_ingredient[ 0 ]);
            tmp_data_p = DECODE__fmt_ingredient( tmp_data_p, local_ingred,
                                                 mmf_length, SIZE_INGREDIENT );
            mmf_length -= strlen( local_ingred );

            //  Format the Preparation field
            tmp_data_p = DECODE__fmt_preparation( tmp_data_p, local_prep,
                                                  mmf_length, SIZE_PREPARATION );
            tmp_data_p = save_data_p + MMF_INGRED_L;
            if ( tmp_data_p > ( auip_data_p + strlen( auip_data_p ) ) )
            {
                tmp_data_p = ( auip_data_p + strlen( auip_data_p ) );
            }
        }
        else
        {
            //  Format the Ingredient field
            tmp_data_p = DECODE__fmt_ingredient( tmp_data_p, local_ingred,
                                                 mmf_length, SIZE_INGREDIENT );
            mmf_length -= strlen( local_ingred );

            //  Format the Preparation field
            tmp_data_p = DECODE__fmt_preparation( tmp_data_p, local_prep,
                                                  mmf_length, SIZE_PREPARATION );
        }

        /********************************************************************
         *  Everything to lowercase
         ********************************************************************/

        //  Amount
        if ( text_is_blank_line( local_amount ) == false )
        {
            text_to_lowercase( local_amount );
        }
        if ( text_is_blank_line( local_unit ) == false )
        {
            text_to_lowercase( local_unit );
        }
        if ( text_is_blank_line( local_ingred ) == false )
        {
            text_to_lowercase( local_ingred );
        }
        if ( text_is_blank_line( local_prep ) == false )
        {
            text_to_lowercase( local_prep );
        }

        /********************************************************************
         *  Attach the new AUIP to the recipe.
         ********************************************************************/

        //  Is there anything in any of the fields
        if (    ( text_is_blank_line( local_amount ) == false )
             || ( text_is_blank_line( local_unit )   == false )
             || ( text_is_blank_line( local_ingred ) == false )
             || ( text_is_blank_line( local_prep )   == false ) )
        {
            //  Add the new ingredient information to the recipe
            DECODE__new_auip( recipe_p, local_amount, local_unit,
                              local_ingred, local_prep );

            log_write( MID_DEBUG_0, recipe_p->rcb_p->tcb_p->thread_name,
                          "A: '%s' U: '%s' I: '%s' P: '%s'\n",
                          local_amount, local_unit, local_ingred, local_prep );
        }
    }   while (    ( strlen( tmp_data_p )     >       0 )
                && ( recipe_format == RECIPE_FORMAT_MMF ) );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Cleanup a recipe name.
 *
 *  @param  name_p              Pointer to the recipe name
 *  @param  name_l              size of the name_p buffer
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_name_cleanup(
    char                    *   name_p,
    int                         name_l
    )
{
    /**
     *  @param  ndx             An index into a buffer                      */
    int                             ndx;
    /**
     *  @param  altered         A flag showing the name was altered         */
    int                             altered;
    /**
     *  @param  left_paran_p    Pointer to a Left Parentheses               */
    char                        * left_paren_p;
    /**
     *  @param  right_paran_p   Pointer to a Right Parentheses              */
    char                        * right_paren_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Leading crap
     ************************************************************************/

    //  Is there unwanted stuff in the beginning of the name ?
    if (    ( name_p[ 0 ] == '*' )
         || ( name_p[ 0 ] == '"' ) )
    {
        //  YES:    Remove it
        text_remove( name_p, 0, 1 );
    }

    /************************************************************************
     *  Sequence number inside parentheses
     ************************************************************************/

    //  Find '(' and ')'
    left_paren_p  = strrchr( name_p, '(' );
    right_paren_p = strrchr( name_p, ')' );

    //  Did we find a good set of both ?
    if (    ( left_paren_p  == NULL )
         || ( right_paren_p == NULL ) )
    {
        //  NO:     '{' and '}'
        left_paren_p  = strrchr( name_p, '{' );
        right_paren_p = strrchr( name_p, '}' );

        //  Did we find a good set of both ?
        if (    ( left_paren_p  == NULL )
             || ( right_paren_p == NULL ) )
        {
            //  NO:     '[' and ']'
            left_paren_p  = strrchr( name_p, '[' );
            right_paren_p = strrchr( name_p, ']' );

            //  Did we find a good set of both ?
            if (    ( left_paren_p  == NULL )
                 || ( right_paren_p == NULL ) )
            {
                //  NO:     '<' and '>'
                left_paren_p  = strrchr( name_p, '<' );
                right_paren_p = strrchr( name_p, '>' );
            }
        }
    }

    //  Do we have both left and right ?
    if (    ( left_paren_p  !=        NULL )
         && ( right_paren_p !=        NULL )
         && ( left_paren_p < right_paren_p )  )
    {
        int                         ndx;
        int                         not_digit;

        //  Assume everything is a digit
        not_digit = false;

        //  YES:    Only numbers between ?
        for( ndx = 1;
             &(left_paren_p[ ndx ]) < right_paren_p;
             ndx += 1 )
        {
            //  Is this a digit or Roman numeral ?
            if (    ( isdigit( left_paren_p[ ndx ] ) ==  0  )
                 && ( isspace( left_paren_p[ ndx ] ) ==  0  )
                 && ( ispunct( left_paren_p[ ndx ] ) ==  0  )
                 && ( toupper( left_paren_p[ ndx ] ) != 'I' )
                 && ( toupper( left_paren_p[ ndx ] ) != 'V' )
                 && ( toupper( left_paren_p[ ndx ] ) != 'X' ) )
            {
                //  NO:     Set the flag
                not_digit = true;
            }
        }

        //  Were they all digits ?
        if ( not_digit == false )
        {
            //  Step through the numeric string
            for( ndx = 0;
                 &(left_paren_p[ ndx ]) <= right_paren_p;
                 ndx += 1 )
            {
                //  Change everything to spaces
                left_paren_p[ ndx ] = ' ';
            }
        }
    }

    /************************************************************************
     *  Trailing Crap
     ************************************************************************/

    for( ndx = ( name_l - 1 ); ndx > 0; ndx -= 1 )
    {
        //  Reset the altered flag
        altered = false;

        //  Is this a "JUNK" character ?
        if (    ( name_p[ ndx ] == ' ' )     //  Space
             || ( name_p[ ndx ] == '.' )     //  Period
             || ( name_p[ ndx ] == '-' )     //  Dash
             || ( name_p[ ndx ] == '*' )     //  Asterisk
             || ( name_p[ ndx ] == '=' )     //  Equal
             || ( name_p[ ndx ] == '#' )     //  Number Sign
             || ( name_p[ ndx ] == '0' )     //  0
             || ( name_p[ ndx ] == '1' )     //  1
             || ( name_p[ ndx ] == '2' )     //  2
             || ( name_p[ ndx ] == '3' )     //  3
             || ( name_p[ ndx ] == '4' )     //  4
             || ( name_p[ ndx ] == '5' )     //  5
             || ( name_p[ ndx ] == '6' )     //  6
             || ( name_p[ ndx ] == '7' )     //  7
             || ( name_p[ ndx ] == '8' )     //  8
             || ( name_p[ ndx ] == '9' ) )   //  9
        {
            //  YES:    Dump it.
            name_p[ ndx ] = '\0';
            altered = true;
        }

        //  Was anything altered during this pass ?
        if ( altered == false )
        {
            //  NO:     That is the end.
            break;
        }
    }

    /************************************************************************
     *  Trailing Roman Numerals
     ************************************************************************/

    for( ndx = ( name_l - 1 ); ndx > 0; ndx -= 1 )
    {
        //  Is this Roman Numeral ?
        if (    ( toupper( name_p[ ndx     ] ) == 'I' )
             || ( toupper( name_p[ ndx     ] ) == 'V' )
             || ( toupper( name_p[ ndx     ] ) == 'X' )
             || (          name_p[ ndx     ]   == '(' )
             || (          name_p[ ndx     ]   == ')' )
             || (          name_p[ ndx     ]   == '[' )
             || (          name_p[ ndx     ]   == ']' )
             || (          name_p[ ndx     ]   == '<' )
             || (          name_p[ ndx     ]   == '>' )
             || (          name_p[ ndx     ]   == ' ' ) )
        {
            //  YES:    Is this a 'Space' character ?
            if ( name_p[ ndx ] == ' ' )
            {
                //  YES:    Truncate the line here
                name_p[ ndx ] = '\0';
            }
        }
        else
        {
            //  NO:     Not a Roman Numeral, just exit.
            break;
        }
    }

    /************************************************************************
     *  Back-To-Back spaces
     ************************************************************************/

    for( ndx = ( name_l - 2 ); ndx > 0; ndx -= 1 )
    {
        //  Two spaces back-to-back ?
        if (    ( name_p[ ndx     ]   == ' ' )
             && ( name_p[ ndx + 1 ]   == ' ' ) )
        {
            /**
             * @param   shift_ndx       File list key                       */
            int                             shift_ndx;

            //  YES:    Delete the second one by left shifting
            for( shift_ndx = ndx + 1;
                 name_p[ shift_ndx ] != '\0';
                 shift_ndx += 1 )
            {
                name_p[ shift_ndx ] = name_p[ shift_ndx + 1 ];
            }
        }
    }

    /************************************************************************
     *  Quotation Marks (")
     ************************************************************************/

    for( ndx = 0; ndx < strlen( name_p ); ndx += 1 )
    {
        //  Two spaces back-to-back ?
        if ( name_p[ ndx ]   == '"' )
        {
            //  Is this the last character of the string ?
            if ( name_p[ ndx + 1 ] == '\0' )
            {
                //  YES:    Just delete it
                name_p[ ndx ] = '\0';
            }
            else
            {
                /**
                 * @param   shift_ndx       File list key                   */
                int                             shift_ndx;

                //  NO:     Delete the quotation mark by left shifting
                for( shift_ndx = ndx;
                     name_p[ shift_ndx + 1 ] != '\0';
                     shift_ndx += 1 )
                {
                    name_p[ shift_ndx ] = name_p[ shift_ndx + 1 ];
                }
                //  Shorten the line (Because of the left shift).
                name_p[ shift_ndx ] = '\0';
            }
        }
    }

    //  Change the recipe name to Title Case
    text_title_case( name_p, name_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Append a new line of text to the notes data.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  data_p              Pointer to a directions line of text
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_fmt_notes(
    struct   recipe_t       *   recipe_p,
    char                    *   data_p
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
    if ( list_query_count( recipe_p->notes_p ) == 0 )
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
    if ( list_query_count( recipe_p->notes_p ) != 0 )
    {
        //  NO:     Get the saved partial line of text from the list.
        tmp_p = list_get_last( recipe_p->notes_p );

        //  Move the data to the scan formatted text buffer
        strncpy( formatted_text, tmp_p, ( sizeof( formatted_text ) - 1 ) );

        //  Remove the last line of text from the list.
        list_delete_payload( recipe_p->notes_p, tmp_p );

        //  Release the storage used by the temporary buffer
        mem_free( tmp_p );
    }

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Did we just get passed a blank line ?
    if ( text_is_blank_line( data_p ) == false )
    {
        //  NO:     Clean the next word buffer
        memset( next_word, '\0', sizeof( next_word ) );

        //  Process all words in the input buffer
        for( scan_buffer_p = text_next_word( data_p, next_word );
             scan_buffer_p != NULL;
             scan_buffer_p = text_next_word( scan_buffer_p, next_word ) )
        {

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

            if (   ( strlen( formatted_text ) + strlen( next_word ) + 1 )
                 >= MAX_LINE_L )
            {

                tmp_p = text_copy_to_new( formatted_text );
                log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

                //  Add it to the list.
                list_put_last( recipe_p->notes_p, tmp_p );

                //  Clear the formatted text buffer.
                memset( formatted_text, '\0', sizeof( formatted_text ) );
            }

            /****************************************************************
             *  First Word of Sentence and First-Word-of-Line
             ****************************************************************/

            if (    ( fwos == true )
                 && ( fwol == true )
                 && ( strlen( next_word ) > 0 ) )
            {
                strncpy( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                formatted_text[ 0 ] = toupper( formatted_text[ 0 ] );
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
                        list_put_last( recipe_p->notes_p, tmp_p );

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
            if (    ( next_word[ strlen( next_word ) - 1 ] == '.' )
                 || ( next_word[ strlen( next_word ) - 1 ] == ':' ) )
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
                    list_put_last( recipe_p->notes_p, tmp_p );

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
        list_put_last( recipe_p->notes_p, tmp_p );

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
 *  Create a new unique recipe.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  recipe_format       Indicator of the decode recipe format.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_next_id(
    struct  recipe_t        *   recipe_p
    )
{
    /**
     *  @param  recipe_id       Temporary data buffer for the recipe id     */
    unsigned char               recipe_id[ SHA1_DIGEST_SIZE + 2 ];
    /**
     *  @param  id_string       Recipe-ID string to identify a recipe       */
    char                        id_string[ ( SHA1_DIGEST_SIZE * 2 ) + 4 ];
    /**
     *  @param  store_value_p   Pointer to data get                         */
    char                    *   store_value_p;
    /**
     *  @param  recipe_id_val   Value of the stored recipe_id               */
    int                         recipe_id_val;
    /**
     *  @param  context         Control structure used by SHA1 functions    */
    SHA1_CTX                    context;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set an initialization value
    store_value_p = recipe_id_p;

    //  Convert the recipe-ID to an integer
    recipe_id_val = atoi( recipe_id_p );

    //  And increment it
    recipe_id_val += 1;

    /************************************************************************
     *  RECIPE-ID
     ************************************************************************/

    //  Initialize SHA1
    sha1_init( &context );

    //  Build SHA1 version of the recipe id
    sha1_update( &context, store_value_p, ( SHA1_DIGEST_SIZE ) );

    //  Finalize the SHA1 operation
    sha1_final( &context, (char*)recipe_id );

    //  Format the Recipe-ID as a hex string
    snprintf( id_string, sizeof( id_string ),
              "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
              "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
              recipe_id[  0 ], recipe_id[  1 ], recipe_id[  2 ], recipe_id[  3 ], recipe_id[  4 ],
              recipe_id[  5 ], recipe_id[  6 ], recipe_id[  7 ], recipe_id[  8 ], recipe_id[  9 ],
              recipe_id[ 10 ], recipe_id[ 11 ], recipe_id[ 12 ], recipe_id[ 13 ], recipe_id[ 14 ],
              recipe_id[ 15 ], recipe_id[ 16 ], recipe_id[ 17 ], recipe_id[ 18 ], recipe_id[ 19 ],
              0x00 );

    //  Add it to the recipe
    recipe_p->recipe_id_p = text_copy_to_new( id_string );
    log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Integer back to ASCII
    snprintf( recipe_id_p, RECIPE_ID_L, "%016d", recipe_id_val );

    //  DONE!
}

/****************************************************************************/
/**
 *  Append a new entry to a list (if the entry is not already on the list.
 *
 *  @param  list_p              Pointer to the list
 *  @param  data_p              Pointer to the data to add to the list.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_append(
    struct  list_base_t     *   list_p,
    char                    *   data_p
    )
{
    /**
     * @param   list_key        File list key                               */
    int                         list_key;
    /**
     * @param   table_data_p    Pointer to data from/to the table           */
    char                    *   table_data_p;
    /**
     *  @param  search_data     A buffer to hold the search string          */
    char                        search_data[ SIZE_CATEGORY + 1 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume there is no match (Only used for an empty list)
    table_data_p = NULL;

    //  Initialize the search buffer
    memset( search_data, ' ', sizeof( search_data ) );
    search_data[ sizeof( search_data ) ] = '\0';

    // Will the new data fit into the search data buffer ?
    if ( sizeof( search_data ) > strlen( data_p ) )
    {
        //  YES:    Copy the data
        memcpy( search_data, data_p, strlen( data_p ) );
    }
    else
    {
        //  NO:     This is a fatal error.
        log_write( MID_INFO, "FATAL-ERROR:",
                   "%s @ line %d (D:%d < S:%d)\n",
                   __FILE__, __LINE__,
                   sizeof( search_data ), strlen( data_p ) );
        log_write( MID_INFO,  "FATAL-ERROR:", "S: '%s'\n", search_data );
        log_write( MID_FATAL, "FATAL-ERROR:", "D: '%s'\n", data_p );
    }

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Is the list empty ?
    if ( list_query_count( list_p ) > 0 )
    {
        //  NO:     Lock the list for fast(er) access
        list_key = list_user_lock( list_p );

        //  Scan the list
        for( table_data_p = list_fget_first( list_p, list_key );
             table_data_p != NULL;
             table_data_p = list_fget_next( list_p, table_data_p, list_key ) )
        {
            //  Is this a match for the new thing we are adding ?
            if ( strncmp( search_data, table_data_p, strlen( table_data_p ) ) == 0 )
            {
                //  YES:    This entry matches the new entry
                break;
            }
        }

        //  Release the lock.
        list_user_unlock( list_p, list_key );
    }

    //  Did we find a match on the list ?
    if ( table_data_p == NULL )
    {
        //  NO:     Cleanup the new entry before adding it to the list.
        memset( search_data, ' ', sizeof( search_data ) );
        search_data[ sizeof( search_data ) ] = '\0';

        if ( sizeof( search_data ) > strlen( data_p ) )
        {
            memcpy( search_data, data_p, strlen( data_p ) );
        }
        else
        {
            log_write( MID_FATAL, "FATAL-ERROR:",
                       "%s @ line %d (D:%d < S:%d)\n",
                        __FILE__, __LINE__,
                        sizeof( search_data ), strlen( data_p ) );
        }
        text_strip_whitespace( search_data );

        //  Copy the new data to a buffer
        table_data_p = text_copy_to_new( search_data );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        //  Add it to the list.
        list_put_last( list_p, table_data_p );
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
 *  @param  data_p              Pointer to a directions line of text
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_add_instructions(
    struct   recipe_t       *   recipe_p,
    char                    *   data_p
    )
{
    /**
     *  @param  new_data_p      Pointer to a new data buffer                */
    char                    *   new_data_p;
    /**
     *  @param  new_data_l      Size needed for the combined (old & new)    */
    int                         new_data_l;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the first thing for the instructions buffer ?
    if ( recipe_p->instructions_p == NULL )
    {
        //  YES:    Just use this buffer
        recipe_p->instructions_p = text_copy_to_new( data_p );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );
    }
    else
    {
        //  NO:     Figure out how big it needs to be.
        new_data_l = ( strlen( data_p ) + ( strlen( recipe_p->instructions_p ) ) + 2 );

        //  Create a new buffer with the old and the new
        new_data_p = mem_malloc( new_data_l );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        //  Merge the two buffers together.
        strncpy( new_data_p, recipe_p->instructions_p, new_data_l );
        strcat( new_data_p, " " );
        strncat( new_data_p, data_p, new_data_l - strlen( new_data_p ) );

        //  Release the (now) unused buffers
        mem_free( recipe_p->instructions_p );

        //  Update the instructions pointer
        recipe_p->instructions_p = new_data_p;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  The decode_save_chapter will identify which category subsection is
 *  identified and store the data in that section.
 *
 *  @param  data_p              Pointer to the raw data category
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_save_chapter(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
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
            decode_append( recipe_p->appliance_p,
                           ( data_p + APPLIANCE_L ) );
        }
        //  Diet ?
        else
        if ( strncmp( DIET, data_p, DIET_L ) == 0 )
        {
            decode_append( recipe_p->diet_p,
                           ( data_p + DIET_L ) );
        }
        //  Course ?
        else
        if ( strncmp( COURSE, data_p, COURSE_L ) == 0 )
        {
            decode_append( recipe_p->course_p,
                           ( data_p + COURSE_L ) );
        }
        //  Cuisine ?
        else
        if ( strncmp( CUISINE, data_p, CUISINE_L ) == 0 )
        {
            decode_append( recipe_p->cuisine_p,
                           ( data_p + CUISINE_L ) );
        }
        //  Occasion ?
        else
        if ( strncmp( OCCASION, data_p, OCCASION_L ) == 0 )
        {
            decode_append( recipe_p->occasion_p,
                           ( data_p + OCCASION_L ) );
        }
        //  Category ?
        else
        if ( strncmp( CATEGORY, data_p, CATEGORY_L ) == 0 )
        {
            decode_append( recipe_p->chapter_p,
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
 *  Unit tests for decode_*
 *
 *  @param  void                No parameters are passed into this function.
 *
 *  @return decode_rc           TRUE if all test pass, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
decode_wbt(
    void
    )
{
   /**
     *  @param  decode_rc       Return code from this function              */
    int                         decode_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize variables
    decode_rc = true;

    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Verify success to this point
    if ( decode_rc == true ) decode_rc = DECODE__TEST_fmt_datetime( );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // Return the pointer that is set past the amount field
    return ( decode_rc );
}

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

void
decode(
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
     *  @param  ok_to_encode    TRUE = OK to encode the recipe              */
    int                         ok_to_encode;
#endif

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

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
         *  Get a new file to import
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

        /********************************************************************
         *  Decoder selection
         ********************************************************************/

        //  Based on the recipe format of the recipe start
        switch( rcb_p->recipe_format )
        {
            case    RECIPE_FORMAT_BOF:
            {
                decode_bof( rcb_p );
                ok_to_encode = true;
            }   break;
            case    RECIPE_FORMAT_MXP:
            {
                //  Decode a MasterCook recipe
                decode_mxp( rcb_p );
                ok_to_encode = true;
            }   break;
            case    RECIPE_FORMAT_MX2:
            {
                //  @ToDo: 3 mx2 decode
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'MX2' is not available\n" );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }   break;
            case    RECIPE_FORMAT_MMF:
            {
                //  Decode a Meal-Master recipe
                decode_mmf( rcb_p );
                ok_to_encode = true;
            }   break;
            case    RECIPE_FORMAT_RXF:
            {
                //  Decode a Recipe eXchange Format recipe
                decode_rxf( rcb_p );
                ok_to_encode = true;
            }   break;
            case    RECIPE_FORMAT_NYC:
            {
                //  @ToDo: 3 nyc decode
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'NYC' is not available\n" );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }   break;
            case    RECIPE_FORMAT_CP2:
            {
                //  @ToDo: 3 cp2 decode
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'CP2' is not available\n" );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }   break;
            case    RECIPE_FORMAT_GRF:
            {
                //  @ToDo: 3 grf decode
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'GRF' is not available\n" );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }   break;
            case    RECIPE_FORMAT_GF2:
            {
                //  @ToDo: 3 gf2 decode
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'GF2' is not available\n" );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }   break;
            case    RECIPE_FORMAT_ERD:
            {
                //  @ToDo: 3 erd decode
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'ERD' is not available\n" );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }   break;
            case    RECIPE_FORMAT_TXT:
            {
                //  @ToDo: 3 txt decode
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'TXT' is not available\n" );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }   break;
            default:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "recipe format (%d)\n",
                           rcb_p->recipe_format );
                //  Clean out the recipe control block
                ok_to_encode = false;
            }
        }


        /********************************************************************
         *  Pass the recipe on to the recipe encoder.
         ********************************************************************/

        //  Is it OK to encode this recipe ?
        if ( ok_to_encode == true )
        {
            //  Post processing recipe format.
            decode_post( rcb_p );

            //  Perform the final verification
            ok_to_encode = DECODE__recipe_verify( rcb_p );

            //  Put it in one of the IMPORT queue's
            queue_put_payload( encode_tcb->queue_id, rcb_p  );
        }
        else
        {
            //  NO:     Kill the RCB
            rcb_kill( rcb_p );
        }

#else

        //  Kill the Recipe Control Block
        rcb_kill( rcb_p );

#endif

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

}
/****************************************************************************/