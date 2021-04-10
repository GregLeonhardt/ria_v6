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
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_MAIN              ( "ALLOCATE STORAGE FOR MAIN" )

//  Generate default command line parameters
#define DEBUG_CMD_LINE          ( 1 )

//  Version Numbers
#define VER_MAJ                 4
#define VER_MIN                 0
#define VER_TMP                 0

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include "global.h"             //  Global stuff for this application
#include "libtools_api.h"       //  My tools library
                                //*******************************************
#include "xlate_api.h"          //  API for all xlate_*             PUBLIC
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
 * @param in_file_name_p    Pointer to the input file name                  */
char                        *   in_file_name_p;
//----------------------------------------------------------------------------
/**
 * @param in_dir_name_p     Pointer to the input directory name             */
char                        *   in_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param out_dir_name_p     Pointer to the output directory name           */
char                        *   out_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param delete_flag       Delete input file when true                     */
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
    int                             argc,
    char                        *   argv[ ]
    )
{
    /**
     * @param main_rc           Return code from called functions.          */
    enum    queue_rc_e              main_rc;
    /**
     *  @param  file_list       Pointer to a list of files                  */
    struct  list_base_t         *   file_list_p;
    /**
     *  @param  file_info_p     Pointer to a file information structure     */
    struct  file_info_t         *   file_info_p;
    /**
     *  @param  queue_rc        Return code from queue management           */
    enum    queue_rc_e              queue_rc;

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
     *  Thread Initialization
     ************************************************************************/

    //  Initialize queue management
    queue_rc = queue_init( );

    //  Verify initialization was successful
    if ( queue_rc != QUEUE_RC_SUCCESS )
    {
        //  ToDo
    }

    //  Encode thread
//  thread_new( encode_thread, NULL );

    /************************************************************************
     *  Command line processing
     ************************************************************************/

    //  Process the command line parameters
    command_line( argc, argv );

    //  Create the file-list
    file_list_p = list_new( );

    /************************************************************************
     *  Prepare input files for processing
     ************************************************************************/

    //  Are we processing a directory ?
    if ( in_dir_name_p != NULL )
    {
        //  Unzip all "*.zip" files
        file_unzip( in_dir_name_p );

        //  YES:    Build the file list
        file_ls( file_list_p, in_dir_name_p, NULL );
    }
    else
    {
        //  Get the file information for the input file
        file_info_p = file_stat( in_file_name_p, in_file_name_p );

        //  Add the new file to the list
        list_put_last( file_list_p, file_info_p );
    }

    /************************************************************************
     *  Application Code
     ************************************************************************/



    /************************************************************************
     *  Application Exit
     ************************************************************************/

    //  DONE!
    return( 0 );
}
/****************************************************************************/