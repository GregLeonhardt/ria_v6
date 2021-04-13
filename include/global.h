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
                                //*******************************************

/****************************************************************************
 * Global Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define THREAD_NAME_SIZE        (  24 )
//-----
#define THREAD_NAME_ROUTER      "ROUTER:"
#define THREAD_NAME_IMPORT      "IMPORT:"
#define THREAD_NAME_EMAIL       "EMAIL:"
#define THREAD_NAME_DECODE      "DECODE:"
#define THREAD_NAME_ENCODE      "ENCODE:"
#define THREAD_NAME_EXPORT      "EXPORT:"
//-----
#define THREAD_COUNT_IMPORT     (   3 )
#define THREAD_COUNT_EMAIL      (   1 )
#define THREAD_COUNT_DECODE     (   1 )
#define THREAD_COUNT_ENCODE     (   1 )
#define THREAD_COUNT_EXPORT     (   1 )
//----------------------------------------------------------------------------
#define MAX_QUEUE_DEPTH         (   0 )
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
/**
 *  @param  rcb                 Recipe Control Block                        */
struct  rcb_t
{
    /**
     *  @param  file_info_p     Pointer to a file information structure     */
    struct  file_info_t     *   file_info_p;
    /**
     *  @param  display_name    Display file name                           */
    char                        display_name[ FILENAME_MAX ];
    /**
     *  @param  destination     Destination Thread                          */
    enum    destination_e       dst_thread;
    /**
     * @param file_ll_p         Pointer to a list where the file is stored  */
    struct  list_base_t     *   import_list_p;
    /**
     *  @param  file_p          File pointer for the import file            */
    FILE                    *   file_p;
};
//----------------------------------------------------------------------------
/**
 *  @param  tcb                 Thread Control Block                        */
struct  tcb_t
{
    /**
     *  @param  queue_id            Data queue id                           */
    int                             queue_id;
    /**
     *  @param  thread_id           Identifies the thread                   */
    int                             thread_id;
    /**
     *  @param  thread_name         Name of this thread                     */
    char                            thread_name[ THREAD_NAME_SIZE ];
    /**
     *  @param  thread_state        State of the thread                     */
    enum    thread_stat_e           thread_state;
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Global Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
    int                             router_queue_id;
//----------------------------------------------------------------------------
/**
 * @param router_tcb                TCB list for the router thread          */
    struct  tcb_t               *   router_tcb;
//----------------------------------------------------------------------------
/**
 * @param import_tcb                TCB list for all import threads         */
    struct  tcb_t               *   import_tcb[ THREAD_COUNT_IMPORT ];
//----------------------------------------------------------------------------
/**
 * @param email_tcb                 TCB list for all import threads         */
    struct  tcb_t               *   email_tcb[ THREAD_COUNT_EMAIL ];
//----------------------------------------------------------------------------
/**
 * @param decode_tcb                TCB list for all decode threads         */
    struct  tcb_t               *   decode_tcb[ THREAD_COUNT_DECODE ];
//----------------------------------------------------------------------------

/****************************************************************************
 * Global Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    GLOBAL_H
