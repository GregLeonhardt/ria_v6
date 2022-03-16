/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef DECODE_RXF_LIB_H
#define DECODE_RXF_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal 
 *  library components of the 'decode_rxf' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_DECODE_RXF
   #define DECODE_RXF_EXT
#else
   #define DECODE_RXF_EXT       extern
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
#define RXF_START               "-----  Recipe Import Assist Start -----"
#define RXF_START_L             strlen( RXF_START )
//----------------------------------------------------------------------------
#define RXF_DESCRIPTION         "Description:"
#define RXF_DESCRIPTION_L       strlen( RXF_DESCRIPTION )
//----------------------------------------------------------------------------



#define RXF_BY                  "Recipe By:"
#define RXF_BY_L                strlen( RXF_BY )
//----------------------------------------------------------------------------
#define RXF_SERVING             "Serving Size:"
#define RXF_SERVING_L           strlen( RXF_SERVING )
//----------------------------------------------------------------------------
#define RXF_CUISINE             "Cuisine:"
#define RXF_CUISINE_L           strlen( RXF_CUISINE )
//----------------------------------------------------------------------------
#define RXF_MAIN                "Main Ingredient:"
#define RXF_MAIN_L              strlen( RXF_MAIN )
//----------------------------------------------------------------------------
#define RXF_CATEGORY            "Categories:"
#define RXF_CATEGORY_L          strlen( RXF_CATEGORY )
//----------------------------------------------------------------------------
#define RXF_INGREDIENTS         "-= Ingredients =-"
#define RXF_INGREDIENTS_L       strlen( RXF_INGREDIENTS )
//----------------------------------------------------------------------------
#define RXF_INSTRUCTIONS        "-= Instructions =-"
#define RXF_INSTRUCTIONS_L      strlen( RXF_INSTRUCTIONS )
//----------------------------------------------------------------------------
#define RXF_END                 "-----  Recipe Import Assist End -----"
#define RXF_END_L               strlen( RXF_END )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    rxf_decode_state_e
{
    RXF_DS_START                =   0,
    RXF_DS_TITLE                =   1,
    RXF_DS_DESCRIPTION          =   2,

    RXF_DS_NEXT_SEGMENT         =  72,
    RXF_DS_BY                   =  73,
    RXF_DS_SERVING              =  74,
    RXF_DS_CUISINE              =  75,
    RXF_DS_MAIN                 =  76,
    RXF_DS_CATEGORY             =  77,
    RXF_DS_INGREDIENTS          =  78,
    RXF_DS_AUIP                 =  79,
    RXF_DS_INSTRUCTIONS         =  80,
    RXF_DS_DIRECTIONS           =  81,

    RXF_DS_END                  =  99
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
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
int
DECODE_RXF__is_start (
    char                        *   char_string_p
    );
//----------------------------------------------------------------------------
int
DECODE_RXF__is_end(
    char                        *   char_string_p
    );
//----------------------------------------------------------------------------
int
DECODE_RXF__is_description(
    char                        *   char_string_p
    );
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
int
DECODE_RXF__do_title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    );
//----------------------------------------------------------------------------
int
DECODE_RXF__do_description(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    );





//----------------------------------------------------------------------------
int
DECODE_RXF__recipe_by(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    );
//----------------------------------------------------------------------------
int
DECODE_RXF__srv_size(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    );
//----------------------------------------------------------------------------
int
DECODE_RXF__categories(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    );
//----------------------------------------------------------------------------
int
DECODE_RXF__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    );
//----------------------------------------------------------------------------
int
DECODE_RXF__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DECODE_RXF_LIB_H
