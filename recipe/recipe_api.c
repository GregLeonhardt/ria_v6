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
#include "decode_mmf_api.h"     //  API for all mmf_*               PUBLIC
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
#define RECIPE_ID_L             ( 17 )
#define MMF_INGRED_L            ( 30 )
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
    recipe_p->appliance  = list_new( );
    recipe_p->diet       = list_new( );
    recipe_p->course     = list_new( );
    recipe_p->cuisine    = list_new( );
    recipe_p->occasion   = list_new( );
    recipe_p->chapter    = list_new( );
    recipe_p->directions = list_new( );
    recipe_p->ingredient = list_new( );
    recipe_p->notes      = list_new( );

    //  Save the original recipe format
    switch( recipe_format )
    {
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MXP:
        {
            //  MasterCook eXport
            recipe_p->source_format = text_copy_to_new( "MXP" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MX2:
        {
            //  MasterCook eXport Ver. 2
            recipe_p->source_format = text_copy_to_new( "MX2" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MMF:
        {
            //  Meal-Master Format
            recipe_p->source_format = text_copy_to_new( "MMF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_RXF:
        {
            //  Recipe eXchange Format (Native)
            recipe_p->source_format = text_copy_to_new( "RXF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_NYC:
        {
            //  Now Your Cooking!
            recipe_p->source_format = text_copy_to_new( "NYC" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_ERD:
        {
            //  Easy Recipe Deluxe
            recipe_p->source_format = text_copy_to_new( "ERD" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_BOF:
        {
            //  Big Oven Format
            recipe_p->source_format = text_copy_to_new( "BOF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_CP2:
        {
            //  Cooken Pro 2.0
            recipe_p->source_format = text_copy_to_new( "CP2" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_GRF:
        {
            //  Generic Format #1     [[[[[
            recipe_p->source_format = text_copy_to_new( "GRF" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_GF2:
        {
            //  Generic Format #2     @@@@@
            recipe_p->source_format = text_copy_to_new( "GF2" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_TXT:
        {
            //  Unformatted text data
            recipe_p->source_format = text_copy_to_new( "TXT" );
            log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        }   break;
        //--------------------------------------------------------------------
        default:
        {
            //  For the unused case types.
        }
        //--------------------------------------------------------------------
    }

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
    if ( recipe_p->name != NULL)             mem_free( recipe_p->name );
    if ( recipe_p->recipe_id != NULL)        mem_free( recipe_p->recipe_id );
    if ( recipe_p->author != NULL)           mem_free( recipe_p->author );
    if ( recipe_p->source_format != NULL)    mem_free( recipe_p->source_format );
    if ( recipe_p->wine != NULL)             mem_free( recipe_p->wine );
    if ( recipe_p->copyright != NULL)        mem_free( recipe_p->copyright );
    if ( recipe_p->edited_by != NULL)        mem_free( recipe_p->edited_by );
    if ( recipe_p->formatted_by != NULL)     mem_free( recipe_p->formatted_by );
    if ( recipe_p->import_from != NULL)      mem_free( recipe_p->import_from );
    if ( recipe_p->serves != NULL)           mem_free( recipe_p->serves );
    if ( recipe_p->servings_size != NULL)    mem_free( recipe_p->servings_size );
    if ( recipe_p->makes != NULL)            mem_free( recipe_p->makes );
    if ( recipe_p->makes_unit != NULL)       mem_free( recipe_p->makes_unit );
    if ( recipe_p->time_prep != NULL)        mem_free( recipe_p->time_prep );
    if ( recipe_p->time_wait != NULL)        mem_free( recipe_p->time_wait );
    if ( recipe_p->time_cook != NULL)        mem_free( recipe_p->time_cook );
    if ( recipe_p->time_rest != NULL)        mem_free( recipe_p->time_rest );
    if ( recipe_p->time_total != NULL)       mem_free( recipe_p->time_total );
    if ( recipe_p->description != NULL)      mem_free( recipe_p->description );
    if ( recipe_p->based_on != NULL)         mem_free( recipe_p->based_on );
    if ( recipe_p->serve_with != NULL)       mem_free( recipe_p->serve_with );
    if ( recipe_p->rating != NULL)           mem_free( recipe_p->rating );
    if ( recipe_p->source != NULL)           mem_free( recipe_p->source );
    if ( recipe_p->skill != NULL)            mem_free( recipe_p->skill );
    if ( recipe_p->instructions != NULL )    mem_free( recipe_p->instructions );
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->appliance ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->appliance ) ) != NULL )
        {
            list_delete( recipe_p->appliance, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->appliance ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->appliance ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->cuisine ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->cuisine ) ) != NULL )
        {
            list_delete( recipe_p->cuisine, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->cuisine ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->cuisine ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->occasion ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->occasion ) ) != NULL )
        {
            list_delete( recipe_p->occasion, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->occasion ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->occasion ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->course ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->course ) ) != NULL )
        {
            list_delete( recipe_p->course, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->course ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->course ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->diet ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->diet ) ) != NULL )
        {
            list_delete( recipe_p->diet, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->diet ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->diet ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->chapter ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->chapter ) ) != NULL )
        {
            list_delete( recipe_p->chapter, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->chapter ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->chapter ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->ingredient ) > 0 )
    {
        while( ( auip_p = list_get_first( recipe_p->ingredient ) ) != NULL )
        {
            if ( auip_p->type_p != NULL )        mem_free( auip_p->type_p );
            if ( auip_p->amount_p != NULL )      mem_free( auip_p->amount_p );
            if ( auip_p->unit_p != NULL )        mem_free( auip_p->unit_p );
            if ( auip_p->ingredient_p != NULL )  mem_free( auip_p->ingredient_p );
            if ( auip_p->preparation_p != NULL ) mem_free( auip_p->preparation_p );
            if ( auip_p != NULL )                mem_free( auip_p );
            list_delete( recipe_p->ingredient, auip_p );
        }
    }
    if ( list_kill( recipe_p->ingredient ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->ingredient ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->directions ) ) != NULL )
        {
            list_delete( recipe_p->directions, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->directions ) != true )
    {
        log_write( MID_FATAL, recipe_p->rcb_p->tcb_p->thread_name,
                      "list_kill( recipe_p->directions ) failed\n" );
    }
    //------------------------------------------------------------------------
    if ( list_query_count( recipe_p->notes ) > 0 )
    {
        while( ( data_p = list_get_first( recipe_p->notes ) ) != NULL )
        {
            list_delete( recipe_p->notes, data_p );
            mem_free( data_p );
        }
    }
    if ( list_kill( recipe_p->notes ) != true )
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

#if 0   //  @ToDo: 3 bof_is_start()
    //-------------------------------------------------------------------
    //  Big-Oven BOF
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( bof_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_BOF;
    }
#endif
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
#if 0   //  @ToDo: 2 mxp_is_start()
    //-------------------------------------------------------------------
    //  MasterCook MXP
    else
    if (    ( recipe_format == RECIPE_FORMAT_NONE )
         && ( mxp_is_start( data_p ) == true ) )
    {
        //  YES:    Set the format to use
        recipe_format = RECIPE_FORMAT_MXP;
    }
#endif
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
    //  When a MXP formatted recipe is embedded inside a MX2, this was producing
    //  a false positive for a recipe break.  If this creates a problem then
    //  I will have to pass the current recipe type for additional testing.

    //  NOTE:
    //  MX2 is a special case because it may contain an embedded MXP recipe
    //  inside it.

    //  Is this something that can end a recipe ?
    if (    ( email_is_group_break( data_p ) == true )
//       || ( bof_is_start(         data_p ) == true )
//       || ( cp2_is_start(         data_p ) == true )
//       || ( gf2_is_start(         data_p ) == true )
//       || ( grf_is_start(         data_p ) == true )
         || ( mmf_is_end(           data_p ) == true )
//       || ( mx2_is_start(         data_p ) == true )
//       || (    ( recipe_format != RECIPE_FORMAT_MX2 )
//            && ( mxp_is_start(    data_p ) == true ) )
//       || ( nyc_is_start(         data_p ) == true ) )
                                                        )
    {
        //  YES:    Change the return code
        decode_rc = true;
    }
    else
    //  Maybe we missed the recipe end.
    if ( recipe_is_start( data_p ) != RECIPE_FORMAT_NONE )
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
/**
 *  Format a Amount Unit Ingredient Preparation line and add the results to
 *  the recipe.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  auip_data_p         Pointer to an AUIP line of text
 *  @param  recipe_format       Indicator of the decode recipe format.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

//  @ToDo: 1 recipe_fmt_auip    Move to decode_fmt_auip

void
recipe_fmt_auip(
    struct   recipe_t       *   recipe_p,
    char                    *   auip_data_p,
    enum    recipe_format_e     recipe_format
    )
{
    /**
     *  @param  tmp_data_p      Pointer to the local input line buffer      */
    char                        *   save_data_p;
    char                        *   tmp_data_p;
    /**
     *  @param  local_amount    Local character buffer for the amount field */
    char                            local_amount[ SIZE_AMOUNT + 1 ];
    /**
     *  @param  local_unit      Unit field                                  */
    char                            local_unit[ SIZE_UNIT + 1 ];
    /**
     *  @param  local_ingred    Ingredient field                            */
    char                            local_ingred[ SIZE_INGREDIENT + 1 ];
    /**
     *  @param  local_prep      Preparation field                           */
    char                            local_prep[ SIZE_PREPARATION + 1 ];
    /**
     *  @param  tmp_ingredient  Temporary ingredient field                  */
    char                            tmp_ingredient[ MMF_INGRED_L + 1 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Skip over leading spaces and or tabs
    tmp_data_p = text_skip_past_whitespace( auip_data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    // Was this recipe constructed with two AUIP per line ?
    do
    {
        int                             mmf_length;

        //  Initialize the data buffers
        memset( local_amount,   '\0', sizeof( local_amount ) );
        memset( local_unit,     '\0', sizeof( local_unit ) );
        memset( local_ingred,   '\0', sizeof( local_ingred ) );
        memset( local_prep,     '\0', sizeof( local_prep ) );
        memset( tmp_ingredient, '\0', sizeof ( tmp_ingredient ) );

        /********************************************************************
         *  Meal-Master dual column length
         ********************************************************************/

        //  Set the maximum length
        if ( recipe_format == RECIPE_FORMAT_MMF )
        {
            //  For a MealMaster recipe
            mmf_length = MMF_INGRED_L;
        }
        else
        {
            //  For all other recipe formats.
            mmf_length = strlen( tmp_data_p );
        }

        /********************************************************************
         *  A - Amount
         ********************************************************************/

        //  Format the Amount field
        tmp_data_p = RECIPE__fmt_amount( tmp_data_p, local_amount, SIZE_AMOUNT );

        /********************************************************************
         *  U - Unit
         ********************************************************************/

        //  Format the Unit field
        if ( strlen( local_amount ) != 0 )
        {
            tmp_data_p = RECIPE__fmt_unit( tmp_data_p, local_unit, SIZE_UNIT );
        }

        /********************************************************************
         *  IP - Ingredient & Preparation
         ********************************************************************/

        if ( recipe_format == RECIPE_FORMAT_MMF )
        {
            //  Format the Ingredient field
            save_data_p = tmp_data_p;
            strncpy( tmp_ingredient, tmp_data_p, mmf_length );
            tmp_data_p = &(tmp_ingredient[ 0 ]);
            tmp_data_p = RECIPE__fmt_ingredient( tmp_data_p, local_ingred,
                                                 mmf_length, SIZE_INGREDIENT );
            mmf_length -= strlen( local_ingred );

            //  Format the Preparation field
            tmp_data_p = RECIPE__fmt_preparation( tmp_data_p, local_prep,
                                                  mmf_length, SIZE_PREPARATION );
            tmp_data_p = save_data_p + MMF_INGRED_L;
            if ( tmp_data_p > ( auip_data_p + strlen( auip_data_p ) ) )
            {
                tmp_data_p = ( auip_data_p + strlen( auip_data_p ) );
            }
        }
        else
        {
            //  Format the Ingredient field
            tmp_data_p = RECIPE__fmt_ingredient( tmp_data_p, local_ingred,
                                                 mmf_length, SIZE_INGREDIENT );
            mmf_length -= strlen( local_ingred );

            //  Format the Preparation field
            tmp_data_p = RECIPE__fmt_preparation( tmp_data_p, local_prep,
                                                  mmf_length, SIZE_PREPARATION );
        }

        /********************************************************************
         *  Everything to lowercase
         ********************************************************************/

        //  Amount
        if ( text_is_blank_line( local_amount ) == false )
        {
            text_to_lowercase( local_amount );
        }
        if ( text_is_blank_line( local_unit ) == false )
        {
            text_to_lowercase( local_unit );
        }
        if ( text_is_blank_line( local_ingred ) == false )
        {
            text_to_lowercase( local_ingred );
        }
        if ( text_is_blank_line( local_prep ) == false )
        {
            text_to_lowercase( local_prep );
        }

        /********************************************************************
         *  Attach the new AUIP to the recipe.
         ********************************************************************/

        //  Is there anything in any of the fields
        if (    ( text_is_blank_line( local_amount ) == false )
             || ( text_is_blank_line( local_unit )   == false )
             || ( text_is_blank_line( local_ingred ) == false )
             || ( text_is_blank_line( local_prep )   == false ) )
        {
            //  Add the new ingredient information to the recipe
            RECIPE__new_auip( recipe_p, local_amount, local_unit,
                              local_ingred, local_prep );

            log_write( MID_DEBUG_0, recipe_p->rcb_p->tcb_p->thread_name,
                          "A: '%s' U: '%s' I: '%s' P: '%s'\n",
                          local_amount, local_unit, local_ingred, local_prep );
        }
    }   while (    ( strlen( tmp_data_p )     >       0 )
                && ( recipe_format == RECIPE_FORMAT_MMF ) );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Cleanup a recipe name.
 *
 *  @param  name_p              Pointer to the recipe name
 *  @param  name_l              size of the name_p buffer
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

//  @ToDo: 1 recipe_name_cleanup     Move to decode_name_cleanup

void
recipe_name_cleanup(
    char                    *   name_p,
    int                         name_l
    )
{
    /**
     *  @param  ndx             An index into a buffer                      */
    int                             ndx;
    /**
     *  @param  altered         A flag showing the name was altered         */
    int                             altered;
    /**
     *  @param  left_paran_p    Pointer to a Left Parentheses               */
    char                        * left_paren_p;
    /**
     *  @param  right_paran_p   Pointer to a Right Parentheses              */
    char                        * right_paren_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Leading crap
     ************************************************************************/

    //  Is there unwanted stuff in the beginning of the name ?
    if (    ( name_p[ 0 ] == '*' )
         || ( name_p[ 0 ] == '"' ) )
    {
        //  YES:    Remove it
        text_remove( name_p, 0, 1 );
    }

    /************************************************************************
     *  Sequence number inside parentheses
     ************************************************************************/

    //  Find '(' and ')'
    left_paren_p  = strrchr( name_p, '(' );
    right_paren_p = strrchr( name_p, ')' );

    //  Did we find a good set of both ?
    if (    ( left_paren_p  == NULL )
         || ( right_paren_p == NULL ) )
    {
        //  NO:     '{' and '}'
        left_paren_p  = strrchr( name_p, '{' );
        right_paren_p = strrchr( name_p, '}' );

        //  Did we find a good set of both ?
        if (    ( left_paren_p  == NULL )
             || ( right_paren_p == NULL ) )
        {
            //  NO:     '[' and ']'
            left_paren_p  = strrchr( name_p, '[' );
            right_paren_p = strrchr( name_p, ']' );

            //  Did we find a good set of both ?
            if (    ( left_paren_p  == NULL )
                 || ( right_paren_p == NULL ) )
            {
                //  NO:     '<' and '>'
                left_paren_p  = strrchr( name_p, '<' );
                right_paren_p = strrchr( name_p, '>' );
            }
        }
    }

    //  Do we have both left and right ?
    if (    ( left_paren_p  !=        NULL )
         && ( right_paren_p !=        NULL )
         && ( left_paren_p < right_paren_p )  )
    {
        int                         ndx;
        int                         not_digit;

        //  Assume everything is a digit
        not_digit = false;

        //  YES:    Only numbers between ?
        for( ndx = 1;
             &(left_paren_p[ ndx ]) < right_paren_p;
             ndx += 1 )
        {
            //  Is this a digit or Roman numeral ?
            if (    ( isdigit( left_paren_p[ ndx ] ) ==  0  )
                 && ( isspace( left_paren_p[ ndx ] ) ==  0  )
                 && ( ispunct( left_paren_p[ ndx ] ) ==  0  )
                 && ( toupper( left_paren_p[ ndx ] ) != 'I' )
                 && ( toupper( left_paren_p[ ndx ] ) != 'V' )
                 && ( toupper( left_paren_p[ ndx ] ) != 'X' ) )
            {
                //  NO:     Set the flag
                not_digit = true;
            }
        }

        //  Were they all digits ?
        if ( not_digit == false )
        {
            //  Step through the numeric string
            for( ndx = 0;
                 &(left_paren_p[ ndx ]) <= right_paren_p;
                 ndx += 1 )
            {
                //  Change everything to spaces
                left_paren_p[ ndx ] = ' ';
            }
        }
    }

    /************************************************************************
     *  Trailing Crap
     ************************************************************************/

    for( ndx = ( name_l - 1 ); ndx > 0; ndx -= 1 )
    {
        //  Reset the altered flag
        altered = false;

        //  Is this a "JUNK" character ?
        if (    ( name_p[ ndx ] == ' ' )     //  Space
             || ( name_p[ ndx ] == '.' )     //  Period
             || ( name_p[ ndx ] == '-' )     //  Dash
             || ( name_p[ ndx ] == '*' )     //  Asterisk
             || ( name_p[ ndx ] == '=' )     //  Equal
             || ( name_p[ ndx ] == '#' )     //  Number Sign
             || ( name_p[ ndx ] == '0' )     //  0
             || ( name_p[ ndx ] == '1' )     //  1
             || ( name_p[ ndx ] == '2' )     //  2
             || ( name_p[ ndx ] == '3' )     //  3
             || ( name_p[ ndx ] == '4' )     //  4
             || ( name_p[ ndx ] == '5' )     //  5
             || ( name_p[ ndx ] == '6' )     //  6
             || ( name_p[ ndx ] == '7' )     //  7
             || ( name_p[ ndx ] == '8' )     //  8
             || ( name_p[ ndx ] == '9' ) )   //  9
        {
            //  YES:    Dump it.
            name_p[ ndx ] = '\0';
            altered = true;
        }

        //  Was anything altered during this pass ?
        if ( altered == false )
        {
            //  NO:     That is the end.
            break;
        }
    }

    /************************************************************************
     *  Trailing Roman Numerals
     ************************************************************************/

    for( ndx = ( name_l - 1 ); ndx > 0; ndx -= 1 )
    {
        //  Is this Roman Numeral ?
        if (    ( toupper( name_p[ ndx     ] ) == 'I' )
             || ( toupper( name_p[ ndx     ] ) == 'V' )
             || ( toupper( name_p[ ndx     ] ) == 'X' )
             || (          name_p[ ndx     ]   == '(' )
             || (          name_p[ ndx     ]   == ')' )
             || (          name_p[ ndx     ]   == '[' )
             || (          name_p[ ndx     ]   == ']' )
             || (          name_p[ ndx     ]   == '<' )
             || (          name_p[ ndx     ]   == '>' )
             || (          name_p[ ndx     ]   == ' ' ) )
        {
            //  YES:    Is this a 'Space' character ?
            if ( name_p[ ndx ] == ' ' )
            {
                //  YES:    Truncate the line here
                name_p[ ndx ] = '\0';
            }
        }
        else
        {
            //  NO:     Not a Roman Numeral, just exit.
            break;
        }
    }

    /************************************************************************
     *  Back-To-Back spaces
     ************************************************************************/

    for( ndx = ( name_l - 2 ); ndx > 0; ndx -= 1 )
    {
        //  Two spaces back-to-back ?
        if (    ( name_p[ ndx     ]   == ' ' )
             && ( name_p[ ndx + 1 ]   == ' ' ) )
        {
            /**
             * @param   shift_ndx       File list key                       */
            int                             shift_ndx;

            //  YES:    Delete the second one by left shifting
            for( shift_ndx = ndx + 1;
                 name_p[ shift_ndx ] != '\0';
                 shift_ndx += 1 )
            {
                name_p[ shift_ndx ] = name_p[ shift_ndx + 1 ];
            }
        }
    }

    /************************************************************************
     *  Quotation Marks (")
     ************************************************************************/

    for( ndx = 0; ndx < strlen( name_p ); ndx += 1 )
    {
        //  Two spaces back-to-back ?
        if ( name_p[ ndx ]   == '"' )
        {
            //  Is this the last character of the string ?
            if ( name_p[ ndx + 1 ] == '\0' )
            {
                //  YES:    Just delete it
                name_p[ ndx ] = '\0';
            }
            else
            {
                /**
                 * @param   shift_ndx       File list key                   */
                int                             shift_ndx;

                //  NO:     Delete the quotation mark by left shifting
                for( shift_ndx = ndx;
                     name_p[ shift_ndx + 1 ] != '\0';
                     shift_ndx += 1 )
                {
                    name_p[ shift_ndx ] = name_p[ shift_ndx + 1 ];
                }
                //  Shorten the line (Because of the left shift).
                name_p[ shift_ndx ] = '\0';
            }
        }
    }

    //  Change the recipe name to Title Case
    text_title_case( name_p, name_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Append a new entry to a list (if the entry is not already on the list.
 *
 *  @param  list_p              Pointer to the list
 *  @param  data_p              Pointer to the data to add to the list.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

//  @ToDo: 1 recipe_append     Move to decode_append

void
recipe_append(
    struct  list_base_t     *   list_p,
    char                    *   data_p
    )
{
    /**
     * @param   list_key        File list key                               */
    int                         list_key;
    /**
     * @param   table_data_p    Pointer to data from/to the table           */
    char                    *   table_data_p;
    /**
     *  @param  search_data     A buffer to hold the search string          */
    char                        search_data[ SIZE_CATEGORY + 1 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume there is no match (Only used for an empty list)
    table_data_p = NULL;

    //  Initialize the search buffer
    memset( search_data, ' ', sizeof( search_data ) );
    search_data[ sizeof( search_data ) ] = '\0';
    memcpy( search_data, data_p, strlen( data_p ) );

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Is the list empty ?
    if ( list_query_count( list_p ) > 0 )
    {
        //  NO:     Lock the list for fast(er) access
        list_key = list_user_lock( list_p );

        //  Scan the list
        for( table_data_p = list_fget_first( list_p, list_key );
             table_data_p != NULL;
             table_data_p = list_fget_next( list_p, table_data_p, list_key ) )
        {
            //  Is this a match for the new thing we are adding ?
            if ( strncmp( search_data, table_data_p, strlen( table_data_p ) ) == 0 )
            {
                //  YES:    This entry matches the new entry
                break;
            }
        }

        //  Release the lock.
        list_user_unlock( list_p, list_key );
    }

    //  Did we find a match on the list ?
    if ( table_data_p == NULL )
    {
        //  NO:     Cleanup the new entry before adding it to the list.
        memset( search_data, ' ', sizeof( search_data ) );
        search_data[ sizeof( search_data ) ] = '\0';
        memcpy( search_data, data_p, strlen( data_p ) );
        text_strip_whitespace( search_data );

        //  Copy the new data to a buffer
        table_data_p = text_copy_to_new( search_data );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        //  Add it to the list.
        list_put_last( list_p, table_data_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Append a new line of text to the directions data.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  data_p              Pointer to a directions line of text
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

//  @ToDo: 1 recipe_add_instructions     Move to decode_add_instructions

void
recipe_add_instructions(
    struct   recipe_t       *   recipe_p,
    char                    *   data_p
    )
{
    /**
     *  @param  new_data_p      Pointer to a new data buffer                */
    char                    *   new_data_p;
    /**
     *  @param  new_data_l      Size needed for the combined (old & new)    */
    int                         new_data_l;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the first thing for the instructions buffer ?
    if ( recipe_p->instructions == NULL )
    {
        //  YES:    Just use this buffer
        recipe_p->instructions = text_copy_to_new( data_p );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );
    }
    else
    {
        //  NO:     Figure out how big it needs to be.
        new_data_l = ( strlen( data_p ) + ( strlen( recipe_p->instructions ) ) + 2 );

        //  Create a new buffer with the old and the new
        new_data_p = mem_malloc( new_data_l );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        //  Merge the two buffers together.
        strncpy( new_data_p, recipe_p->instructions, new_data_l );
        strcat( new_data_p, " " );
        strncat( new_data_p, data_p, new_data_l - strlen( new_data_p ) );

        //  Release the (now) unused buffers
        mem_free( recipe_p->instructions );

        //  Update the instructions pointer
        recipe_p->instructions = new_data_p;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Append a new line of text to the notes data.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  data_p              Pointer to a directions line of text
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

//  @ToDo: 1 recipe_fmt_notes     Move to decode_fmt_notes

void
recipe_fmt_notes(
    struct   recipe_t       *   recipe_p,
    char                    *   data_p
    )
{
    /**
     *  @param  scan_buffer_p   A local pointer to the input buffer         */
    char                    *   scan_buffer_p;
    /**
     *  @param  formatted_text  Temp buffer - formatted directions line     */
    char                        formatted_text[ MAX_LINE_L + 1 ];
    /**
     *  @param  next_word       A local bucket for the next directions word */
    char                        next_word[ MAX_LINE_L ];
    /**
     *  @param  index           Scan index                                  */
    int                         index;
    /**
     *  @param  fwos            First Word of Sentence                      */
    static
    int                         fwos;
    /**
     *  @param  fwol            First Word of Line                          */
    static
    int                         fwol;
    /**
     *  @param  tmp_p           Pointer to a temporary buffer               */
    char                    *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Are we starting the directions for a new recipe ?
    if ( list_query_count( recipe_p->notes ) == 0 )
    {
        //  YES:    Reset the first word flags
        fwos = true;
        fwol = true;
    }

    //  Is the first word of a new line ?
    if ( fwol == true )
    {
        //  YES:    Clear the formatted line buffer
        memset( formatted_text, '\0', sizeof( formatted_text ) );
    }
    else
    if ( list_query_count( recipe_p->notes ) != 0 )
    {
        //  NO:     Get the saved partial line of text from the list.
        tmp_p = list_get_last( recipe_p->notes );

        //  Move the data to the scan formatted text buffer
        strncpy( formatted_text, tmp_p, ( sizeof( formatted_text ) - 1 ) );

        //  Remove the last line of text from the list.
        list_delete( recipe_p->notes, tmp_p );

        //  Release the storage used by the temporary buffer
        mem_free( tmp_p );
    }

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Did we just get passed a blank line ?
    if ( text_is_blank_line( data_p ) == false )
    {
        //  NO:     Clean the next word buffer
        memset( next_word, '\0', sizeof( next_word ) );

        //  Process all words in the input buffer
        for( scan_buffer_p = text_next_word( data_p, next_word );
             scan_buffer_p != NULL;
             scan_buffer_p = text_next_word( scan_buffer_p, next_word ) )
        {

            /****************************************************************
             *  Remove format bars of all dashes.
             ****************************************************************/

            for( index = 0;
                 index < strlen( next_word );
                 index++ )
            {
                if ( next_word[ index ] != '-' )
                {
                    break;
                }
            }
            //  Was it just a line of dashes ?
            if ( index == strlen( next_word ) )
            {
                //  YES:    Ignore it..
                continue;
            }

            /****************************************************************
             *  Special transformation of a numbered directions list.
             ****************************************************************/

             // Is this a numbered directions sequence ?
            if (    ( fwos == true )
                 && ( isdigit( next_word[ 0 ] ) !=  0  )
                 && (    ( next_word[ 1 ]   == '.' )
                      || ( next_word[ 1 ]   == ')' )
                      || ( next_word[ 1 ]   == ']' )
                      || ( next_word[ 1 ]   == ';' )
                      || ( next_word[ 1 ]   == ':' ) ) )
            {
                //  YES:    Throw it away
                next_word[ 0 ] = '\0';

                //  That also makes whatever is next the First-Word-of-Line
                fwol = true;
            }

            /****************************************************************
             *  Anything that starts with a asterisk ( * ) is also the
             *  start of a new paragraph.
             ****************************************************************/

            if ( next_word[ 0 ] == '*' )
            {
                fwos = true;
                fwol = false;
            }

            /****************************************************************
             *  If the next string causes the formatted line to be to large,
             *  write it and start a new line.
             ****************************************************************/

            if (   ( strlen( formatted_text ) + strlen( next_word ) + 1 )
                 >= MAX_LINE_L )
            {

                tmp_p = text_copy_to_new( formatted_text );
                log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

                //  Add it to the list.
                list_put_last( recipe_p->notes, tmp_p );

                //  Clear the formatted text buffer.
                memset( formatted_text, '\0', sizeof( formatted_text ) );
            }

            /****************************************************************
             *  First Word of Sentence and First-Word-of-Line
             ****************************************************************/

            if (    ( fwos == true )
                 && ( fwol == true )
                 && ( strlen( next_word ) > 0 ) )
            {
                strncpy( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                formatted_text[ 0 ] = toupper( formatted_text[ 0 ] );
                fwos = false;
                fwol = false;
            }

            /****************************************************************
             *  First Word of Sentence AND NOT First-Word-of-Line
             ****************************************************************/

            else
            if (    ( fwos == true  )
                 && ( fwol == false )
                 && ( strlen( next_word ) > 0 ) )
            {
                //  This is the first word of a new sentence but not
                //  the first word of a new output line.  Test the
                //  word for a paragraph starter.
                if ( xlate_paragraph_starters( next_word ) )
                {
                    //  Make sure there is something in the current line
                    if ( text_is_blank_line( formatted_text ) == false )
                    {
                        //  This word marks the start of a new sentence.
                        //  Write the current output line and start a new one
                        tmp_p = text_copy_to_new( formatted_text );
                        log_write( MID_DEBUG_1, "recipe_api.c",
                                "Line: %d\n", __LINE__ );

                        //  Add it to the list.
                        list_put_last( recipe_p->notes, tmp_p );

                        //  Clear the formatted text buffer.
                        memset( formatted_text, '\0', sizeof( formatted_text ) );
                    }
                }
                strncat( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                formatted_text[ 0 ] = toupper( formatted_text[ 0 ] );
                fwos = false;
                fwol = false;
            }

            /****************************************************************
             *  NOT First-Word-of-Sentence AND First-Word-of-Line
             ****************************************************************/

            else
            if (    ( fwos == false )
                 && ( fwol == true  )
                 && ( strlen( next_word ) > 0 ) )
            {
                //  Append the new word to an existing sentence.
                strncat( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ", MAX_LINE_L - strlen( formatted_text ) );
                fwos = false;
                fwol = false;
            }

            /****************************************************************
             *  NOT First-Word-of-Sentence and NOT First-Word-of-Line
             ****************************************************************/

            else
            if (    ( fwos == false )
                 && ( fwol == false )
                 && ( strlen( next_word ) > 0 ) )
            {
                //  Append the new word to an existing sentence.
                strncat( formatted_text, next_word, MAX_LINE_L );
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                fwos = false;
                fwol = false;
            }
            //  Is this the end of a sentence ?
            if (    ( next_word[ strlen( next_word ) - 1 ] == '.' )
                 || ( next_word[ strlen( next_word ) - 1 ] == ':' ) )
            {
                //  YES:    Insert an extra space in the formatted text.
                strncat( formatted_text, " ",
                         MAX_LINE_L - strlen( formatted_text ) );
                fwos = true;
            }
            //  Is this a special ?
            if (    ( fwos == true )
                 && ( formatted_text[ 0 ] == '*' ) )
            {
                //  YES:    Make sure there is something in the current line
                if ( text_is_blank_line( formatted_text ) == false )
                {
                    //  This word marks the start of a new sentence.
                    //  Write the current output line and start a new one
                    tmp_p = text_copy_to_new( formatted_text );
                    log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

                    //  Add it to the list.
                    list_put_last( recipe_p->notes, tmp_p );

                    //  Clear the formatted text buffer.
                    memset( formatted_text, '\0', sizeof( formatted_text ) );
                    fwol = true;
                }
            }

            //  Clean the next word buffer
            memset( next_word, '\0', sizeof( next_word ) );
        }
    }
    else
    {
        //  YES:    Whatever (if anything else) is next will start a new line.
        fwos = true;
        fwol = true;
    }

    /************************************************************************
     *  We are done with the input line of text.  Save it on the list.
     ************************************************************************/

    //  Is there something in the formatted text buffer ?
    if ( strlen( formatted_text ) > 0 )
    {
        //  YES:    Copy it to a temporary buffer.
        tmp_p = text_copy_to_new( formatted_text );
        log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

        //  Add it to the list.
        list_put_last( recipe_p->notes, tmp_p );

        //  Clear the formatted text buffer.
        memset( formatted_text, '\0', sizeof( formatted_text ) );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Create a new unique recipe.
 *
 *  @param  recipe_p            Primary structure for a recipe.
 *  @param  recipe_format       Indicator of the decode recipe format.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

//  @ToDo: 1 recipe_next_id     Move to decode_next_id

void
recipe_next_id(
    struct  recipe_t        *   recipe_p
    )
{
    /**
     *  @param  recipe_id       Temporary data buffer for the recipe id     */
    unsigned char               recipe_id[ SHA1_DIGEST_SIZE + 2 ];
    /**
     *  @param  id_string       Recipe-ID string to identify a recipe       */
    char                        id_string[ ( SHA1_DIGEST_SIZE * 2 ) + 4 ];
    /**
     *  @param  store_value_p   Pointer to data get                         */
    char                    *   store_value_p;
    /**
     *  @param  recipe_id_val   Value of the stored recipe_id               */
    int                         recipe_id_val;
    /**
     *  @param  context         Control structure used by SHA1 functions    */
    SHA1_CTX                    context;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set an initialization value
    store_value_p = recipe_id_p;

    //  Convert the recipe-ID to an integer
    recipe_id_val = atoi( recipe_id_p );

    //  And increment it
    recipe_id_val += 1;

    /************************************************************************
     *  RECIPE-ID
     ************************************************************************/

    //  Initialize SHA1
    sha1_init( &context );

    //  Build SHA1 version of the recipe id
    sha1_update( &context, store_value_p, ( SHA1_DIGEST_SIZE ) );

    //  Finalize the SHA1 operation
    sha1_final( &context, (char*)recipe_id );

    //  Format the Recipe-ID as a hex string
    snprintf( id_string, sizeof( id_string ),
              "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
              "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
              recipe_id[  0 ], recipe_id[  1 ], recipe_id[  2 ], recipe_id[  3 ], recipe_id[  4 ],
              recipe_id[  5 ], recipe_id[  6 ], recipe_id[  7 ], recipe_id[  8 ], recipe_id[  9 ],
              recipe_id[ 10 ], recipe_id[ 11 ], recipe_id[ 12 ], recipe_id[ 13 ], recipe_id[ 14 ],
              recipe_id[ 15 ], recipe_id[ 16 ], recipe_id[ 17 ], recipe_id[ 18 ], recipe_id[ 19 ],
              0x00 );

    //  Add it to the recipe
    recipe_p->recipe_id = text_copy_to_new( id_string );
    log_write( MID_DEBUG_1, "recipe_api.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Integer back to ASCII
    snprintf( recipe_id_p, RECIPE_ID_L, "%016d", recipe_id_val );

    //  DONE!
}
/****************************************************************************/