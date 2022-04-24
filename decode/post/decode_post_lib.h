/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef POST_LIB_H
#define POST_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal
 *  library components of the 'post' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_POST
   #define POST_EXT
#else
   #define POST_EXT          extern
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
#define APPLIANCE               "Appliance:"
#define APPLIANCE_L             strlen( APPLIANCE )
//----------------------------------------------------------------------------
#define DIET                    "Diet:"
#define DIET_L                  strlen( DIET )
//----------------------------------------------------------------------------
#define COURSE                  "Course:"
#define COURSE_L                strlen( COURSE )
//----------------------------------------------------------------------------
#define CUISINE                 "Cuisine:"
#define CUISINE_L               strlen( CUISINE )
//----------------------------------------------------------------------------
#define OCCASION                "Occasion:"
#define OCCASION_L              strlen( OCCASION )
//----------------------------------------------------------------------------
#define CATEGORY                "Category:"
#define CATEGORY_L              strlen( CATEGORY )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
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
POST_EXT
int                             decode_post_lib;
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
void
DECODE_POST__fmt_directions(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__title_information(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__recipe_id(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_cleanup(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_from(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_source(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_copyright(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_description(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_makes(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_time(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_import_from(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__directions_notes(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__recipe_id(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------
void
DECODE_POST__datetime(
    struct  rcb_t           *   rcb_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    POST_LIB_H
