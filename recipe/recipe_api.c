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
 *  library components of the 'recipe' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_RECIPE          ( "ALLOCATE STORAGE FOR RECIPE" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <unistd.h>             //  Access to the POSIX operating system API
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
#include "email_api.h"          //  API for all email_*             PUBLIC
#include "decode_api.h"         //  API for all decode_*            PUBLIC
#include "xlate_api.h"          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include "recipe_api.h"         //  API for all recipe_*            PUBLIC
#include "recipe_lib.h"         //  API for all RECIPE__*           PRIVATE
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
 *  Create and initialize a new recipe structure.
 *
 *  @param  rcb_p               Pointer to a Recipe Control Block
 *  @param  recipe_format       The recipe format being decoded
 *
 *  @return recipe_p            Primary structure for a recipe.
 *
 *  @note
 *
 ****************************************************************************/

struct   recipe_t   *
recipe_new(
    struct  rcb_t           *   rcb_p,
    enum    recipe_format_e     recipe_format
    )
{
    /**
     *  @param recipe_t         Primary structure for a recipe              */
    struct   recipe_t       *   recipe_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Allocate storage for the structure
    recipe_p = mem_malloc( sizeof( struct recipe_t ) );

    log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

    //  Link the recipe structure to the Recipe Control Block
    recipe_p->rcb_p = rcb_p;

    //  Create the lists needed by the structure
    recipe_p->appliance_p  = list_new( );
    recipe_p->diet_p       = list_new( );
    recipe_p->course_p     = list_new( );
    recipe_p->cuisine_p    = list_new( );
    recipe_p->occasion_p   = list_new( );
    recipe_p->chapter_p    = list_new( );
    recipe_p->directions_p = list_new( );
    recipe_p->ingredient_p = list_new( );
    recipe_p->notes_p      = list_new( );

    //  Save the original recipe format
    switch( recipe_format )
    {
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MXP:
        {
            //  MasterCook eXport
            recipe_p->source_format_p = text_copy_to_new( "MXP" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MX2:
        {
            //  MasterCook eXport Ver. 2
            recipe_p->source_format_p = text_copy_to_new( "MX2" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MMF:
        {
            //  Meal-Master Format
            recipe_p->source_format_p = text_copy_to_new( "MMF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_RXF:
        {
            //  Recipe eXchange Format (Native)
            recipe_p->source_format_p = text_copy_to_new( "RXF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_NYC:
        {
            //  Now Your Cooking!
            recipe_p->source_format_p = text_copy_to_new( "NYC" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_ERD:
        {
            //  Easy Recipe Deluxe
            recipe_p->source_format_p = text_copy_to_new( "ERD" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_BOF:
        {
            //  Big Oven Format
            recipe_p->source_format_p = text_copy_to_new( "BOF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_CP2:
        {
            //  Cooken Pro 2.0
            recipe_p->source_format_p = text_copy_to_new( "CP2" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_GRF:
        {
            //  Generic Format #1     [[[[[
            recipe_p->source_format_p = text_copy_to_new( "GRF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_GF2:
        {
            //  Generic Format #2     @@@@@
            recipe_p->source_format_p = text_copy_to_new( "GF2" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_TXT:
        {
            //  Unformatted text data
            recipe_p->source_format_p = text_copy_to_new( "TXT" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        default:
        {
            //  For the unused case types.
        }
        //--------------------------------------------------------------------
    }

    //  Initialize the recipe decode stuff
    recipe_p->categories_scan_state = CSS_IDLE;
    recipe_p->auip_scan_state       = AUIPS_IDLE;
    recipe_p->direction_scan_state  = DSS_IDLE;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( recipe_p );
}

/****************************************************************************/
/**
 *  Remove an existing recipe structure.
 *
 *  @param  recipe              Primary structure for a recipe.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
recipe_kill(
    struct   recipe_t       *   recipe_p
    )
{
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   data_p;
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t          *   auip_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    if ( recipe_p->rcb_p != NULL)            recipe_p->rcb_p->recipe_p = NULL;
    if ( recipe_p->name_p != NULL)             mem_free( recipe_p->name_p );
    if ( recipe_p->recipe_id_p != NULL)        mem_free( recipe_p->recipe_id_p );
    if ( recipe_p->author_p != NULL)           mem_free( recipe_p->author_p );
    if ( recipe_p->source_format_p != NULL)    mem_free( recipe_p->source_format_p );
    if ( recipe_p->wine_p != NULL)             mem_free( recipe_p->wine_p );
    if ( recipe_p->copyright_p != NULL)        mem_free( recipe_p->copyright_p );
    if ( recipe_p->edited_by_p != NULL)        mem_free( recipe_p->edited_by_p );
    if ( recipe_p->formatted_by_p != NULL)     mem_free( recipe_p->formatted_by_p );
    if ( recipe_p->import_from_p != NULL)      mem_free( recipe_p->import_from_p );
    if ( recipe_p->serves_p != NULL)           mem_free( recipe_p->serves_p );
    if ( recipe_p->servings_size_p != NULL)    mem_free( recipe_p->servings_size_p );
    if ( recipe_p->makes_p != NULL)            mem_free( recipe_p->makes_p );
    if ( recipe_p->makes_unit_p != NULL)       mem_free( recipe_p->makes_unit_p );
    if ( recipe_p->time_prep_p != NULL)        mem_free( recipe_p->time_prep_p );
    if ( recipe_p->time_wait_p != NULL)        mem_free( recipe_p->time_wait_p );
    if ( recipe_p->time_cook_p != NULL)        mem_free( recipe_p->time_cook_p );
    if ( recipe_p->time_rest_p != NULL)        mem_free( recipe_p->time_rest_p );
    if ( recipe_p->time_total_p != NULL)       mem_free( recipe_p->time_total_p );
    if ( recipe_p->description_p != NULL)      mem_free( recipe_p->description_p );
    if ( recipe_p->based_on_p != NULL)         mem_free( recipe_p->based_on_p );
    if ( recipe_p->serve_with_p != NULL)       mem_free( recipe_p->serve_with_p );
    if ( recipe_p->rating_p != NULL)           mem_free( recipe_p->rating_p );
    if ( recipe_p->source_p != NULL)           mem_free( recipe_p->source_p );
    if ( recipe_p->skill_p != NULL)            mem_free( recipe_p->skill_p );
    if ( recipe_p->instructions_p != NULL )    mem_free( recipe_p->instructions_p );
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->appliance_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->appliance_p ) ) != NULL )
        {
            list_delete( recipe_p->appliance_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->appliance_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->appliance ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->cuisine_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->cuisine_p ) ) != NULL )
        {
            list_delete( recipe_p->cuisine_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->cuisine_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->cuisine ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->occasion_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->occasion_p ) ) != NULL )
        {
            list_delete( recipe_p->occasion_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->occasion_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->occasion ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->course_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->course_p ) ) != NULL )
        {
            list_delete( recipe_p->course_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->course_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->course ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->diet_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->diet_p ) ) != NULL )
        {
            list_delete( recipe_p->diet_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->diet_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->diet ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->chapter_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->chapter_p ) ) != NULL )
        {
            list_delete( recipe_p->chapter_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->chapter_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->chapter ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->ingredient_p ) > 0 )
    {
        while( ( auip_p = list_get_first( recipe_p->ingredient_p ) ) != NULL )
        {
            if ( auip_p->type_p != NULL )        mem_free( auip_p->type_p );
            if ( auip_p->amount_p != NULL )      mem_free( auip_p->amount_p );
            if ( auip_p->unit_p != NULL )        mem_free( auip_p->unit_p );
            if ( auip_p->ingredient_p != NULL )  mem_free( auip_p->ingredient_p );
            if ( auip_p->preparation_p != NULL ) mem_free( auip_p->preparation_p );
            if ( auip_p != NULL )                mem_free( auip_p );
            list_delete( recipe_p->ingredient_p, auip_p );
        }
    }
    if ( list_kill( recipe_p->ingredient_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->ingredient ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->directions_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->directions_p ) ) != NULL )
        {
            list_delete( recipe_p->directions_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->directions_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->directions ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->notes_p ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->notes_p ) ) != NULL )
        {
            list_delete( recipe_p->notes_p, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->notes_p ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->notes ) failed\n" );
    }
    //------------------------------------------------------------------------

    //  Clear the structure
    memset( (char*)recipe_p, 0x00, sizeof( struct recipe_t ) );

    //  Release the structure storage
    mem_free( recipe_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Analyze the data buffer for a possible recipe start.
 *
 *  @param  data_p              Pointer to a data buffer.
 *
 *  @return recipe_format       Decoded recipe format
 *
 *  @note
 *
 ****************************************************************************/

enum    recipe_format_e
recipe_is_start(
    char                    *   data_p
    )
{
    /**
     *  @param  recipe_format   See recipe_format_e for details             */
    enum    recipe_format_e     recipe_format;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set the default return code
    recipe_format = RECIPE_FORMAT_NONE;

    /************************************************************************
     *  Function
     ************************************************************************/

    //-------------------------------------------------------------------
    //  Big-Oven BOF
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( bof_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_BOF;
    }
#if 0   //  @ToDo: 3 cp2_is_start()
    //-------------------------------------------------------------------
    //  CookenPro 2.0
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( cp2_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_CP2;
    }
#endif
#if 0   //  @ToDo: 3 erd_is_start()
    //-------------------------------------------------------------------
    //  Easy Recipe Deluxe
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( erd_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_ERD;
    }
#endif
#if 0   //  @ToDo: 3 gf2_is_start()
    //-------------------------------------------------------------------
    //  Generic-Recipe-Format @@@@@
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( gf2_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_GF2;
    }
#endif
#if 0   //  @ToDo: 3 grf_is_start()
    //-------------------------------------------------------------------
    //  Generic-Recipe-Format [[[[[
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( grf_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_GRF;
    }
    //-------------------------------------------------------------------
    //  Meal-Master
    else
#endif
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( mmf_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_MMF;
    }
#if 0   //  @ToDo: 3 mx2_is_start()
    //-------------------------------------------------------------------
    //  MasterCook MX2
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( mx2_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_MX2;
    }
#endif
    //-------------------------------------------------------------------
    //  MasterCook MXP
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( mxp_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_MXP;
    }
#if 0   //  @ToDo: 3 nyc_is_start()
    //-------------------------------------------------------------------
    //  Now You're Cooking!
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( nyc_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_NYC;
    }
#endif

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( recipe_format );
}

/****************************************************************************/
/**
 *  Analyze the data and determine if it contains something that can be
 *  interpreted as the end of a recipe other than a normal termination line.
 *
 *  @param  recipe_format       The current recipe format
 *  @param  data_p              Pointer to the raw data category
 *
 *  @return recipe_rc           TRUE when the data is the end of a recipe
 *                              else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

int
recipe_is_end(
    enum    recipe_format_e         recipe_format,
    char                        *   data_p
    )
{
    /**
     *  @param  decode_rc       Return code from this function              */
    int                             decode_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that all tests will fail
    decode_rc = false;

    //  Remove any trailing white space from the data buffer
    text_strip_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  NOTE:
    //  When a MXP formatted recipe is embedded inside a MX2, this was
    //  producing a false positive for a recipe break.  If this creates a
    //  problem then I will have to pass the current recipe type for
    //  additional testing.

    //  NOTE:
    //  MX2 is a special case because it may contain an embedded MXP recipe
    //  inside it.

    //  Is this something that can end a recipe ?
    if (    ( mmf_is_end(           data_p ) == true )
         || ( bof_is_end(           data_p ) == true )
//  @ToDo: 3 cp2_is_end
//       || ( cp2_is_end(           data_p ) == true )
//  @ToDo: 3 gf2_is_end
//       || ( gf2_is_end(           data_p ) == true )
//  @ToDo: 3 grf_is_end
//       || ( grf_is_end(           data_p ) == true )
//  @ToDo: 3 mx2_is_end
//       || ( mx2_is_end(           data_p ) == true )
//       || (    ( recipe_format != RECIPE_FORMAT_MX2 )
//            && ( mxp_is_start(    data_p ) == true ) )
//  @ToDo: 3 nyc_is_end
//       || ( nyc_is_end(           data_p ) == true ) )
                                                        )
    {
        //  YES:    Change the return code
        decode_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( decode_rc );
}
/****************************************************************************/