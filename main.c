/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This is the main entry point for the application.
 *
 *  @note
 *      Basic data flow within the program.
 *      *   main( )
 *          Scans the source directory for files and makes a list of them.
 *          One by one files are removed from the file list and queues them
 *          to one of the IMPORT threads as a job.
 *          After all files are queued it begins to monitor its input queue.
 *          When something is detected it queries the job to determine where
 *          the job should go next and places it on the DECODE, ENCODE, or
 *          export queue.
 *      *   import( )
 *          All jobs in the input queue are preprocessed and marked for
 *          decode and returned to main.
 *      *   decode( )
 *          All jobs in the input queue are analyzed.
 *          When a recipe format is discovered  the recipe is decoded and
 *          passed back to main with a destination of encoding.
 *          NOTE:   A single input job (file) may contain many recipes.  Each
 *                  recipe is returned to main as a new job.
 *          When a recipe format isn't detected, all information is
 *          discarded and the next job in the queue is started.
 *      *   encode( )
 *          The job data is encoded and returned back to main with a
 *          destination of export.
 *
 ****************************************************************************/

//  This is a global to do list:

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_MAIN              ( "ALLOCATE STORAGE FOR MAIN" )

//  Generate default command line parameters
#define DEBUG_CMD_LINE          ( 1 )

//  Version Numbers
#define VER_MAJ                 6
#define VER_MIN                 0
#define VER_TMP                 2

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
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include "global.h"             //  Global stuff for this application
#include "libtools_api.h"       //  My tools library
                                //*******************************************
#include "tcb_api.h"            //  API for all tcb_*               PUBLIC
#include "rcb_api.h"            //  API for all rcb_*               PUBLIC
#include "monitor_api.h"        //  API for all monitor_*           PUBLIC
#include "xlate_api.h"          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include "router_api.h"         //  API for all router_*            PUBLIC
#include "import_api.h"         //  API for all import_*            PUBLIC
#include "email_api.h"          //  API for all email_*             PUBLIC
#include "decode_api.h"         //  API for all decode_*            PUBLIC
#include "encode_api.h"         //  API for all encode_*            PUBLIC
                                //*******************************************

/****************************************************************************
 * API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define NO_IF_OR_ID             ( 1 )
#define BOTH_IF_AND_ID          ( 2 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 * @param in_file_name_p        Pointer to the input file name              */
char                        *   in_file_name_p;
//----------------------------------------------------------------------------
/**
 * @param in_dir_name_p         Pointer to the input directory name         */
char                        *   in_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param out_dir_name_p        Pointer to the output directory name       */
char                        *   out_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param delete_flag           Delete input file when true                 */
int                             delete_flag;
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return main_rc             TRUE when all parameters are successfully
 *                              processed, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

static
void
help(
    int                             error_id
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    switch( error_id )
    {
        case    NO_IF_OR_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Missing -if or -id     "
                          "Did not find a file name or directory name that "
                          "is to be scanned.\n" );
        }   break;
        case    BOTH_IF_AND_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Both -if and -id found "
                          "Only one of the two may be used.\n" );
        }   break;
    }

    //  Command line options
    log_write( MID_INFO, "main: help",
                  "-if {file_name}          Input file name\n" );
    log_write( MID_INFO, "main: help",
                  "-id {directory_name}     Input directory name\n" );
    log_write( MID_FATAL, "main: help",
                  "-od {directory_name}     Output directory name\n" );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return void                Nothing is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

static
void
command_line(
    int                             argc,
    char                        *   argv[]
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the pointers
    in_file_name_p = NULL;
    in_dir_name_p  = NULL;
    out_dir_name_p = NULL;

    /************************************************************************
     *  Scan for parameters
     ************************************************************************/

    //  Scan for        Input File name
    in_file_name_p  = get_cmd_line_parm( argc, argv, "if" );

    //  Scan for        Input Directory name
    in_dir_name_p   = get_cmd_line_parm( argc, argv, "id" );

    //  Scan for        Input Directory name
    out_dir_name_p  = get_cmd_line_parm( argc, argv, "od" );

    //  Scan for        DELETE input file after processing.
    delete_flag = is_cmd_line_parm( argc, argv, "del" );

#if DEBUG_CMD_LINE
    in_dir_name_p        = "/home/greg/work/RecipeSourceFiles";
    out_dir_name_p       = "/home/greg/work/RecipeOutputFiles";
#endif

    /************************************************************************
     *  Verify everything is good
     ************************************************************************/

    //  Is there an Input File name or an Input Directory name ?
    if (    ( in_file_name_p == NULL )
         && ( in_dir_name_p  == NULL ) )
    {
        //  NO:     Write some help information
        help( NO_IF_OR_ID );
    }

    //  Is there both an Input File name and an Input Directory name ?
    if (    ( in_file_name_p != NULL )
         && ( in_dir_name_p  != NULL ) )
    {
        //  YES:    Write some help information
        help( BOTH_IF_AND_ID );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Return the queue depth for a thread group
 *
 *  @param  void                No information is passed to this function.
 *
 *  @return rc                  TRUE when the router thread group is done
 *                              FALSE when still working
 *
 *  @note
 *
 ****************************************************************************/

static
int
is_router_done(
    void
    )
{
    /**
     *  @param  func_rc         Function return code                        */
    int                         func_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume everything is complete
    func_rc = true;

    /************************************************************************
     *  Check for complete
     ************************************************************************/

    //  Anything in the input queue ?
    if ( queue_get_count( router_tcb->queue_id ) != 0 )
    {
        //  YES:    It's still working
        func_rc = false;
    }
    else
        //  NO:     Is the thread working on something
    if ( router_tcb->thread_state == TS_WORKING )
    {
        //  YES:    It's still working
        func_rc = false;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( func_rc );
}

/****************************************************************************/
/**
 *  Return the queue depth for a thread group
 *
 *  @param  void                No information is passed to this function.
 *
 *  @return rc                  TRUE when the router thread group is done
 *                              FALSE when still working
 *
 *  @note
 *
 ****************************************************************************/

static
int
is_import_done(
    void
    )
{
    /**
     *  @param  func_rc         Function return code                        */
    int                         func_rc;
    /**
     * @param thread_id         Unique thread id number                     */
    int                         thread_id;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume everything is complete
    func_rc = true;

    /************************************************************************
     *  Check for complete
     ************************************************************************/

    //  Loop through all IMPORT threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_IMPORT;
         thread_id += 1 )
    {
        //  Anything in the input queue ?
        if ( queue_get_count( import_tcb[ thread_id ]->queue_id ) != 0 )
        {
            //  YES:    It's still working
            func_rc = false;
        }
        else
        //  NO:     Is the thread working on something
        if ( import_tcb[ thread_id ]->thread_state == TS_WORKING )
        {
            //  YES:    It's still working
            func_rc = false;
        }

        //  Is this thread done ?
        if ( func_rc == false )
        {
            //  NO:     No need to look at the others.
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( func_rc );
}

/****************************************************************************/
/**
 *  Return the queue depth for a thread group
 *
 *  @param  void                No information is passed to this function.
 *
 *  @return rc                  TRUE when the router thread group is done
 *                              FALSE when still working
 *
 *  @note
 *
 ****************************************************************************/

static
int
is_email_done(
    void
    )
{
    /**
     *  @param  func_rc         Function return code                        */
    int                         func_rc;
    /**
     * @param thread_id         Unique thread id number                     */
    int                         thread_id;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume everything is complete
    func_rc = true;

    /************************************************************************
     *  Check for complete
     ************************************************************************/

    //  Loop through all EMAIL threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_EMAIL;
         thread_id += 1 )
    {
        //  Anything in the input queue ?
        if ( queue_get_count( email_tcb[ thread_id ]->queue_id ) != 0 )
        {
            //  YES:    It's still working
            func_rc = false;
        }
        else
        //  NO:     Is the thread working on something
        if ( email_tcb[ thread_id ]->thread_state == TS_WORKING )
        {
            //  YES:    It's still working
            func_rc = false;
        }

        //  Is this thread done ?
        if ( func_rc == false )
        {
            //  NO:     No need to look at the others.
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( func_rc );
}

/****************************************************************************/
/**
 *  Return the queue depth for a thread group
 *
 *  @param  void                No information is passed to this function.
 *
 *  @return rc                  TRUE when the router thread group is done
 *                              FALSE when still working
 *
 *  @note
 *
 ****************************************************************************/

static
int
is_decode_done(
    void
    )
{
    /**
     *  @param  func_rc         Function return code                        */
    int                         func_rc;
    /**
     * @param thread_id         Unique thread id number                     */
    int                         thread_id;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume everything is complete
    func_rc = true;

    /************************************************************************
     *  Check for complete
     ************************************************************************/

    //  Loop through all DECODE threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_ENCODE;
         thread_id += 1 )
    {
        //  Anything in the input queue ?
        if ( queue_get_count( decode_tcb[ thread_id ]->queue_id ) != 0 )
        {
            //  YES:    It's still working
            func_rc = false;
        }
        else
        //  NO:     Is the thread working on something
        if ( decode_tcb[ thread_id ]->thread_state == TS_WORKING )
        {
            //  YES:    It's still working
            func_rc = false;
        }

        //  Is this thread done ?
        if ( func_rc == false )
        {
            //  NO:     No need to look at the others.
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( func_rc );
}

/****************************************************************************/
/**
 *  Return the queue depth for a thread group
 *
 *  @param  void                No information is passed to this function.
 *
 *  @return rc                  TRUE when the router thread group is done
 *                              FALSE when still working
 *
 *  @note
 *
 ****************************************************************************/

static
int
is_encode_done(
    void
    )
{
    /**
     *  @param  func_rc         Function return code                        */
    int                         func_rc;
    /**
     * @param thread_id         Unique thread id number                     */
    int                         thread_id;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume everything is complete
    func_rc = true;

    /************************************************************************
     *  Check for complete
     ************************************************************************/

    //  Loop through all ENCODE threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_ENCODE;
         thread_id += 1 )
    {
        //  Anything in the input queue ?
        if ( queue_get_count( encode_tcb[ thread_id ]->queue_id ) != 0 )
        {
            //  YES:    It's still working
            func_rc = false;
        }
        else
        //  NO:     Is the thread working on something
        if ( encode_tcb[ thread_id ]->thread_state == TS_WORKING )
        {
            //  YES:    It's still working
            func_rc = false;
        }

        //  Is this thread done ?
        if ( func_rc == false )
        {
            //  NO:     No need to look at the others.
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( func_rc );
}
/****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  This is the main entry point for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return                     Zero for success. Any other value is an error.
 *
 *  @note
 *
 ****************************************************************************/

int
main(
    int                         argc,
    char                    *   argv[ ]
    )
{
    /**
     * @param main_rc           Return code from called functions.          */
    enum    queue_rc_e          main_rc;
    /**
     *  @param  file_info_p     Pointer to a file information structure     */
    struct  file_info_t     *   file_info_p;
    /**
     * @param thread_id         Unique thread id number                     */
    int                         thread_id;
    /**
     *  @param  id              Thread ID with smallest queue               */
    int                         id;
    /**
     *  @param  queue_size      Queue size                                  */
    int                         queue_size;

    /************************************************************************
     *  Application Initialization
     ************************************************************************/

    //  Initialize the CommonCode memory process
    token_init( );
    mem_init( );
    store_init( );

    /************************************************************************
     *  Initialize the log process
     ************************************************************************/

    //  Initialize the log handler
    log_init( "ria.log" );

    //  Mark the start of the run in the log file
    log_write( MID_INFO, "main",
                         "|==============================================|\n" );
    log_write( MID_INFO, "main",
                         "Starting: Recipe Import Assist:\n" );
    log_write( MID_INFO, "main",
                         "\tVersion: %02d:%02d:%02d\n",
                         VER_MAJ, VER_MIN, VER_TMP );
    log_write( MID_INFO, "main",
                         "\tCompiled: %s %s.\n",
                         __DATE__, __TIME__ );

    for ( int count = 0;
              count < argc;
              count ++ )
    {
        log_write( MID_INFO, "main",
                             "arcv[ %d ]: '%s'\n",
                             count, argv[ count ] );
    }

    //  Log the event
    log_write( MID_INFO, "main",
                         "Log initialization complete.\n" );

    /************************************************************************
     *  Command line processing
     ************************************************************************/

    //  Process the command line parameters
    command_line( argc, argv );

    //  Create the file-list
    file_list_p = list_new( );

    /************************************************************************
     *  Initialize the queue process
     ************************************************************************/

    //  Initialize the queue handler
    main_rc = queue_init( );

    //  Was the queue initialization successful ?
    if ( main_rc != QUEUE_RC_SUCCESS )
    {
        //  NO:     Something bad happened.
        log_write( MID_FATAL, "main",
                      "Queue initialization failed.\n" );
    }
    else
    {
        //  YES:    Log the event
        log_write( MID_INFO, "main",
                      "Queue initialization complete.\n" );
    }

    /************************************************************************
     *  Initialize the translation process
     ************************************************************************/

    //  Initialize the queue handler
    main_rc = xlate_init( );

    //  Was the queue initialization successful ?
    if ( main_rc != true )
    {
        //  NO:     Something bad happened.
        log_write( MID_FATAL, "main",
                      "Xlate initialization failed.\n" );
    }
    else
    {
        //  YES:    Log the event
        log_write( MID_INFO, "main",
                      "Xlate initialization complete.\n" );
    }

    /************************************************************************
     *  ROUTER      Thread and Queue Initialization
     ************************************************************************/

    //  Allocate storage for a Thread Control Block
    router_tcb = tcb_new( THREAD_NAME_ROUTER, 0, MAX_QUEUE_DEPTH );

    //  The router queue id needs to be global
    router_queue_id = router_tcb->queue_id;

    //  Launch the import thread
    thread_new( router, router_tcb );

    //  Wait for the thread to be initialized
    do
    {
        usleep( 100 );

        //  Loop until the thread is 'WAIT'ing for work
    }   while( router_tcb->thread_state != TS_WAIT );

    /************************************************************************
     *  IMPORT      Thread and Queue Initialization
     ************************************************************************/

    //  Loop through all IMPORT threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_IMPORT;
         thread_id += 1 )
    {
        //  Allocate storage for a Thread Control Block
        import_tcb[ thread_id ] = tcb_new( THREAD_NAME_IMPORT,
                                           thread_id,
                                           MAX_QUEUE_DEPTH );

        //  Launch the import thread
        thread_new( import, import_tcb[ thread_id ] );

        //  Wait for the thread to be initialized
        do
        {
            usleep( 100 );

            //  Loop until the thread is 'WAIT'ing for work
        }   while( import_tcb[ thread_id ]->thread_state != TS_WAIT );
    }

    /************************************************************************
     *  EMAIL       Thread and Queue Initialization
     ************************************************************************/

    //  Loop through all EMAIL threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_EMAIL;
         thread_id += 1 )
    {
        //  Allocate storage for a Thread Control Block
        email_tcb[ thread_id ] = tcb_new( THREAD_NAME_EMAIL,
                                          thread_id,
                                          MAX_QUEUE_DEPTH );

        //  Launch the EMAIL thread
        thread_new( email, email_tcb[ thread_id ] );

        //  Wait for the thread to be initialized
        do
        {
            usleep( 100 );

            //  Loop until the thread is 'WAIT'ing for work
        }   while( import_tcb[ thread_id ]->thread_state != TS_WAIT );
    }

    /************************************************************************
     *  DECODE      Thread and Queue Initialization
     ************************************************************************/

    //  Loop through all IMPORT threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_DECODE;
         thread_id += 1 )
    {
        //  Allocate storage for a Thread Control Block
        decode_tcb[ thread_id ] = tcb_new( THREAD_NAME_DECODE,
                                           thread_id,
                                           MAX_QUEUE_DEPTH );

        //  Launch the decode thread
        thread_new( decode, decode_tcb[ thread_id ] );

        //  Wait for the thread to be initialized
        do
        {
            usleep( 100 );

            //  Loop until the thread is 'WAIT'ing for work
        }   while( decode_tcb[ thread_id ]->thread_state != TS_WAIT );
    }

    /************************************************************************
     *  ENCODE      Thread and Queue Initialization
     ************************************************************************/

    //  Loop through all IMPORT threads
    for( thread_id = 0;
         thread_id < THREAD_COUNT_ENCODE;
         thread_id += 1 )
    {
        //  Allocate storage for a Thread Control Block
        encode_tcb[ thread_id ] = tcb_new( THREAD_NAME_ENCODE,
                                           thread_id,
                                           MAX_QUEUE_DEPTH );

        //  Launch the decode thread
        thread_new( encode, encode_tcb[ thread_id ] );

        //  Wait for the thread to be initialized
        do
        {
            usleep( 100 );

            //  Loop until the thread is 'WAIT'ing for work
        }   while( encode_tcb[ thread_id ]->thread_state != TS_WAIT );
    }

    /************************************************************************
     *  Prepare input files for processing
     ************************************************************************/

    //  Are we processing a directory ?
    if ( in_dir_name_p != NULL )
    {
        /**
         *  @param  count           Number of files unzipped                */
        int                         count;

        do
        {

            //  Unzip all "*.zip" files
            count = file_unzip( in_dir_name_p );

            //  YES:    Build the file list
            file_ls( file_list_p, in_dir_name_p, NULL );

            //  Loop until there aren't any more files to unzip
        }   while( count != 0 );
    }
    else
    {
        //  Get the file information for the input file
        file_info_p = file_stat( in_file_name_p, in_file_name_p );

        //  Add the new file to the list
        list_put_last( file_list_p, file_info_p );
    }

    /************************************************************************
     *  MONITOR     Track all queue depths
     ************************************************************************/

    //  Allocate storage for a Thread Control Block
    monitor_tcb = tcb_new( THREAD_NAME_MONITOR, 0, MAX_QUEUE_DEPTH );

    //  Launch the import thread
    thread_new( monitor, monitor_tcb );

    /************************************************************************
     *  IMPORT  everything on the list
     ************************************************************************/

    //  Scan the list
    for( file_info_p = list_get_first( file_list_p );
         file_info_p != NULL;
         file_info_p = list_get_next( file_list_p, file_info_p ) )
    {
        //  Remove this file from the list.
        list_delete( file_list_p, file_info_p );

        //  Is there anything in the file ?
        if ( text_to_int( file_info_p->file_size ) >= 100 )
        {
            /**
             *  @param  rcb_p           Recipe Control block                    */
            struct  rcb_t           *   rcb_p;

            //  YES:    Allocate a new recipe control block
            rcb_p = rcb_new( NULL, RECIPE_FORMAT_NONE );

            //  Put the file info pointer into the recipe control block
            rcb_p->file_info_p = file_info_p;

            //  Set the display file name
            memset( rcb_p->display_name, '\0', sizeof( rcb_p->display_name ) );
            snprintf( rcb_p->display_name, sizeof( rcb_p->display_name ),
                      "%s/%s",
                      &file_info_p->dir_name[ strlen( in_dir_name_p ) + 1 ],
                      file_info_p->file_name );

            //  Set the base numbers
            id = 99999999;
            queue_size = 99999999;

            //  Locate the thread with the smallest queue depth
            for( thread_id = 0;
                 thread_id < THREAD_COUNT_IMPORT;
                 thread_id += 1 )
            {
                int                     queue_count;

                //  Get the queue count for this queue
                queue_count = queue_get_count( import_tcb[ thread_id ]->queue_id );

                //  Is it smaller than the current smallest ?
                if ( queue_count < queue_size )
                {
                    //  YES:    Use this queue
                    id = import_tcb[ thread_id ]->queue_id;
                    queue_size = queue_count;
                }
            }

            //  Put it in one of the IMPORT queue's
            queue_put_payload( id, rcb_p  );

            //  Progress report.
            log_write( MID_LOGONLY, "main",
                       "Q-%03d: Snd: FILE-ID: %s\n", id,
                       rcb_p->display_name );
        }
        else
        {
            //  NO:     Log the Empty or small file
            log_write( MID_INFO, "main",
                       "Skipping empty or small file: '%s/%s'\n",
                       &file_info_p->dir_name[ strlen( in_dir_name_p ) + 1 ],
                       file_info_p->file_name );

            //  Release the storage for this file
            mem_free( file_info_p );
        }
    }

    /************************************************************************
     *  Wait for all threads to complete
     ************************************************************************/

    //  DO - WHILE any thread is still working
    while( 1 )
    {
        /**
         *  @param  done_flag       TRUE when threads are done              */
        int                         done_flag;

        //  Sleep a few seconds before trying again.
        usleep( 500 );

        //  Is there any work running through the router ?
        done_flag = is_router_done( );

        if ( done_flag == true )
            done_flag = is_import_done( );

        if ( done_flag == true )
            done_flag = is_email_done( );

        if ( done_flag == true )
            done_flag = is_decode_done( );

        if ( done_flag == true )
            done_flag = is_encode_done( );

        //  DONE_FLAG can only be TRUE when EVERYTHING is done.
        if ( done_flag == true )
            break;
    }

    /************************************************************************
     *  Application Exit
     ************************************************************************/

    //  DONE!
    return( 0 );
}
/****************************************************************************/