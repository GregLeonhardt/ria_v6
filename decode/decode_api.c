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
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE          ( "ALLOCATE STORAGE FOR DECODE" )

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
                                //*******************************************
#include "decode_mmf_api.h"     //  API for all mmf_*               PUBLIC
#include "decode_post_api.h"    //  API for all post_*              PUBLIC
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
            recipe_append( recipe_p->appliance,
                           ( data_p + APPLIANCE_L ) );
        }
        //  Diet ?
        else
        if ( strncmp( DIET, data_p, DIET_L ) == 0 )
        {
            recipe_append( recipe_p->diet,
                           ( data_p + DIET_L ) );
        }
        //  Course ?
        else
        if ( strncmp( COURSE, data_p, COURSE_L ) == 0 )
        {
            recipe_append( recipe_p->course,
                           ( data_p + COURSE_L ) );
        }
        //  Cuisine ?
        else
        if ( strncmp( CUISINE, data_p, CUISINE_L ) == 0 )
        {
            recipe_append( recipe_p->cuisine,
                           ( data_p + CUISINE_L ) );
        }
        //  Occasion ?
        else
        if ( strncmp( OCCASION, data_p, OCCASION_L ) == 0 )
        {
            recipe_append( recipe_p->occasion,
                           ( data_p + OCCASION_L ) );
        }
        //  Category ?
        else
        if ( strncmp( CATEGORY, data_p, CATEGORY_L ) == 0 )
        {
            recipe_append( recipe_p->chapter,
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
                   "Q-%03d: Rcv: FILE-ID: %s\n",
                   tcb_p->queue_id, rcb_p->file_path );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

        /********************************************************************
         *  Decoder selection
         ********************************************************************/

        //  Based on the recipe format of the recipe start
        switch( rcb_p->recipe_format )
        {
            case    RECIPE_FORMAT_MXP:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'MXP' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_MX2:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'MX2' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_MMF:
            {
                //  Decode a Meal-Master recipe
                decode_mmf( rcb_p );
            }   break;
            case    RECIPE_FORMAT_RXF:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'RXF' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_NYC:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'NYC' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_BOF:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'BOF' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_CP2:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'CP2' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_GRF:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'GRF' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_GF2:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'GF2' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_ERD:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'ERD' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            case    RECIPE_FORMAT_TXT:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Decode for recipe format 'TXT' is not available\n" );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }   break;
            default:
            {
                //  OOPS.
                log_write( MID_WARNING, tcb_p->thread_name,
                           "Q-%03d: Rcv: UNKNOWN recipe format (%d)\n",
                           tcb_p->queue_id, rcb_p->recipe_format );
                //  Clean out the recipe control block
                rcb_kill( rcb_p );
            }
        }

        //  Post recipe format specific decode processing
        decode_post( rcb_p );

        //  Set the RCB destination
        rcb_p->dst_thread = DST_ENCODE;

        //  Put it in one of the IMPORT queue's
        queue_put_payload( router_queue_id, rcb_p  );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

}
/****************************************************************************/