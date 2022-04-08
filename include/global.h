/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that are accessible to the
 *  entire project.  I.E. "global".
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_MAIN
   #define MAIN_EXT
#else
   #define MAIN_EXT             extern
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
#include "recipe_api.h"         //  API for all recipe_*            PUBLIC
                                //*******************************************

/****************************************************************************
 * Global Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define THREAD_NAME_SIZE        (  24 )
//-----
#define THREAD_NAME_MONITOR     "MONITOR:"
#define THREAD_NAME_IMPORT      "IMPORT:"
#define THREAD_NAME_EMAIL       "EMAIL:"
#define THREAD_NAME_DECODE      "DECODE:"
#define THREAD_NAME_ENCODE      "ENCODE:"
#define THREAD_NAME_EXPORT      "EXPORT:"
//-----
#define THREAD_COUNT_IMPORT     (    1 )
#define THREAD_COUNT_EMAIL      (    1 )
#define THREAD_COUNT_DECODE     (    1 )
#define THREAD_COUNT_ENCODE     (    1 )
#define THREAD_COUNT_EXPORT     (    1 )
//----------------------------------------------------------------------------
#define MAX_QUEUE_DEPTH         (    0 )
#define QUEUE_DEPTH_IMPORT      (    1 )
#define QUEUE_DEPTH_EMAIL       (    1 )
#define QUEUE_DEPTH_DECODE      (    1 )
#define QUEUE_DEPTH_ENCODE      (   10 )
#define QUEUE_DEPTH_EXPORT      (  100 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Global Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  thread_state_e      The current state of a thread               */
enum    thread_stat_e
{
    TS_STARTUP              =   0,
    TS_WAIT                 =   1,
    TS_WORKING              =   2,
    TS_SHUTDOWN             =   3
};
//----------------------------------------------------------------------------
/**
 *  @param  destination_e       Destination Thread                          */
enum    destination_e
{
    DST_INVALID             =   0,      //  Zero is always invalid
    DST_IMPORT              =   1,
    DST_EMAIL               =   2,
    DST_DECODE              =   3,
    DST_ENCODE              =   4,
    DST_EXPORT              =   5
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Global Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Global Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 * @param monitor_tcb           TCB list for the monitor thread             */
MAIN_EXT
struct  tcb_t           *   monitor_tcb;
//----------------------------------------------------------------------------
/**
 * @param import_tcb            TCB list for all import threads             */
MAIN_EXT
struct  tcb_t           *   import_tcb;
//----------------------------------------------------------------------------
/**
 * @param email_tcb             TCB list for all import threads             */
MAIN_EXT
struct  tcb_t           *   email_tcb;
//----------------------------------------------------------------------------
/**
 * @param decode_tcb            TCB list for all decode threads             */
MAIN_EXT
struct  tcb_t           *   decode_tcb;
//----------------------------------------------------------------------------
/**
 * @param decode_tcb            TCB list for all encode threads             */
MAIN_EXT
struct  tcb_t           *   encode_tcb;
//----------------------------------------------------------------------------
/**
 * @param export_tcb            TCB list for all export threads             */
MAIN_EXT
struct  tcb_t           *   export_tcb;
//---------------------------------------------------------------------------
/**
 *  @param  store_value_p       ID number for the DECODE queue              */
MAIN_EXT
char                    *   recipe_id_p;
//----------------------------------------------------------------------------
/**
 *  @param  file_list           Pointer to a list of files                  */
MAIN_EXT
struct  list_base_t         *   file_list_p;
//----------------------------------------------------------------------------
/**
 * @param in_file_name_p        Pointer to the input file name              */
MAIN_EXT
char                        *   in_file_name_p;
//----------------------------------------------------------------------------
/**
 * @param in_dir_name_p         Pointer to the input directory name         */
MAIN_EXT
char                        *   in_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param out_dir_name_p        Pointer to the output directory name       */
MAIN_EXT
char                        *   out_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param delete_flag           Delete input file when true                 */
MAIN_EXT
int                             delete_flag;
//----------------------------------------------------------------------------

/****************************************************************************
 * Global Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    GLOBAL_H
