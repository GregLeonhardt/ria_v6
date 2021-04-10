/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef XLATE_LIB_H
#define XLATE_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal 
 *  library components of the 'xlate' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_XLATE
   #define XLATE_EXT
#else
   #define XLATE_EXT         extern
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
XLATE_EXT
struct  list_base_t         *   unit_of_measurement_p;
XLATE_EXT
struct  list_base_t         *   categories_p;
XLATE_EXT
struct  list_base_t         *   abbreviations_p;
XLATE_EXT
struct  list_base_t         *   paragraph_starters_p;
XLATE_EXT
struct  list_base_t         *   key_words_p;
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
int
XLATE__table_input(
    FILE                        *   file_fp,
    struct  list_base_t         *   xlate_table_p
    );
//----------------------------------------------------------------------------
char  *
XLATE__find_in_table(
    struct  list_base_t         *   search_table_p,
    char                        *   search_string_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    XLATE_LIB_H
