/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef TCB_LIB_H
#define TCB_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal
 *  library components of the 'tcb' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_TCB
   #define TCB_EXT
#else
   #define TCB_EXT          extern
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
TCB_EXT
int                             FooBar;
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    TCB_LIB_H
