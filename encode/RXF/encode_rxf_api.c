/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'encode_rxf' library.
 *
 *  @note
 *          @@@@@
 *          Jules Chicken Curry
 *                                                          (blank line)
 *          note (such as, this is a favorite)
 *                                                          (blank line)
 *          1 chicken
 *          1 jar Pataks mild curry sauce
 *          6 sprigs cilantro for garnish
 *                                                          (blank line)
 *          Prepare chicken, pour curry sauce over.
 *
 *          Bake until done, garnishing with cilantro.
 *
 *          Serves 4-6
 *                                                          (blank line)
 *          @@@@@
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE_RXF        ( "ALLOCATE STORAGE FOR ENCODE_RXF" )

#define _GNU_SOURCE             // See feature_test_macros(7)

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include "global.h"             //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include "tcb_api.h"            //  API for all tcb_*               PUBLIC
#include "rcb_api.h"            //  API for all rcb_*               PUBLIC
#include "email_api.h"          //  API for all email_*             PUBLIC
                                //*******************************************
#include <encode_api.h>         //  API for all encode_*            PUBLIC
#include "encode_rxf_lib.h"     //  API for all ENCODE_RXF__*       PRIVATE
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
 *  Encode the recipe in 'RXF' format
 *
 *  @param  rcb_p               Pointer to a Recipe Control Block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
encode_rxf(
    struct   rcb_t          *   rcb_p
    )
{
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t          *   auip_p;
    /**
     *  @param  tmp_data_p      Pointer to a temporary data buffer          */
    char                    *   tmp_data_p;
    /**
     *  @param  write_data_p    Pointer to a write data buffer              */
    char                    *   write_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Write RXF-XML to the file
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  RXF Recipe start tag
    asprintf( &write_data_p, "-----  Recipe Import Assist Start -----\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  NAME
    asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->name_p );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  Section break:
    asprintf( &write_data_p, "\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  DESCRIPTION:
    if ( rcb_p->recipe_p->description_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->description_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "Description: ?\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  Section break:
    asprintf( &write_data_p, "\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <AUIP>
    if ( list_query_count( rcb_p->recipe_p->ingredient_p ) > 0 )
    {
        for( auip_p = list_get_first( rcb_p->recipe_p->ingredient_p );
             auip_p != NULL;
             auip_p = list_get_next( rcb_p->recipe_p->ingredient_p, auip_p ) )
        {
            //  Is there an amount ?
            if ( auip_p->amount_p != NULL )
            {
                asprintf( &write_data_p, "%-8s",     auip_p->amount_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            else
            {
                asprintf( &write_data_p, "        " );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                asprintf( &write_data_p, "%-12s",     auip_p->unit_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            else
            {
                asprintf( &write_data_p, "            " );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                asprintf( &write_data_p, "%s",     auip_p->ingredient_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //  Is there a preparation method ?
            if ( auip_p->preparation_p != NULL )
            {
                asprintf( &write_data_p, " %s",     auip_p->preparation_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //  End-of-Line
            asprintf( &write_data_p, "\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
    }
    else
    {
        //  No AUIP
        asprintf( &write_data_p, "** TEXT ONLY - NO INGREDIENTS **\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  Section break:
    asprintf( &write_data_p, "\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  DIRECTIONS
    if ( list_query_count( rcb_p->recipe_p->directions_p ) > 0 )
    {
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->directions_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->directions_p, tmp_data_p ) )
        {
            //  Is there something to write ?
            if ( text_is_blank_line( tmp_data_p ) != true )
            {
                //  YES:    Write the directions text.
                asprintf( &write_data_p, "%s\n\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
    }
    //-----------------------------------------------------------------------
    //  Recipe Data:
    asprintf( &write_data_p, "\n----- Recipe Data -----\n\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  AUTHOR:
    asprintf( &write_data_p, "AUTHOR:         " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->author_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->author_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  SERVES:
    asprintf( &write_data_p, "SERVES:         " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->serves_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->serves_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  TIME PREP:
    asprintf( &write_data_p, "TIME PREP:      " );

    list_put_last( rcb_p->export_list_p, write_data_p );
    if ( rcb_p->recipe_p->time_prep_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_prep_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "0:00\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  TIME COOK:
    asprintf( &write_data_p, "TIME COOK:      " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->time_cook_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_cook_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "0:00\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  TIME WAIT:
    //  @ToDo: 3 This field isn't decoded yet!
    //          I'm using the resting time as a place holder.
    //          --->>>  What is the difference between WAIT and REST ?
    asprintf( &write_data_p, "TIME WAIT:      " );
    list_put_last( rcb_p->export_list_p, write_data_p );
    if ( rcb_p->recipe_p->time_rest_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_wait_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "0:00\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  TIME REST:
    asprintf( &write_data_p, "TIME REST:      " );
    list_put_last( rcb_p->export_list_p, write_data_p );
    if ( rcb_p->recipe_p->time_rest_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_rest_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "0:00\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  YIELD:
    asprintf( &write_data_p, "YIELD:          " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->makes_p != NULL )
    {
        asprintf( &write_data_p, "%s", rcb_p->recipe_p->makes_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //  Is there a unit of measurement ?
    if ( rcb_p->recipe_p->makes_unit_p != NULL )
    {
        //  YES:    Write it
        asprintf( &write_data_p, " %s\n", rcb_p->recipe_p->makes_unit_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        //  NO:     End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  SOURCE:
    asprintf( &write_data_p, "SOURCE:         " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->source_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->source_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  NOTES:
    asprintf( &write_data_p, "NOTES:\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( list_query_count( rcb_p->recipe_p->notes_p ) > 0 )
    {
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->notes_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->notes_p, tmp_data_p ) )
        {
            asprintf( &write_data_p, "%s\n", tmp_data_p );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
    }
    //  Put a blank line following the notes.
    asprintf( &write_data_p, "\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  CATEGORIES
    asprintf( &write_data_p, "CATEGORIES:\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    {
        int                             first_category;

        //  ####################
        //  type=CUISINE
        asprintf( &write_data_p, "    CUISINE:    " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( list_query_count( rcb_p->recipe_p->cuisine_p ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->cuisine_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->cuisine_p, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                }
            }
        }
        //  End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );

        //  ####################
        //  type=OCCASION
        asprintf( &write_data_p, "    OCCASION:   " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( list_query_count( rcb_p->recipe_p->occasion_p ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->occasion_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->occasion_p, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                }
            }
        }
        //  End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );

        //  ####################
        //  type=MEAL
        asprintf( &write_data_p, "    MEAL:     " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( list_query_count( rcb_p->recipe_p->meal_p ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->meal_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->meal_p, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                }
            }
        }
        //  End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );

        //  ####################
        //  type=DIET
        asprintf( &write_data_p, "    DIET:       " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( list_query_count( rcb_p->recipe_p->diet_p ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->diet_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->diet_p, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                }
            }
        }
        //  End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );

        //  ####################
        //  type=APPLIANCE
        asprintf( &write_data_p, "    APPLIANCE:  " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( list_query_count( rcb_p->recipe_p->appliance_p ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->appliance_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->appliance_p, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                }
            }
        }
        //  End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );

        //  ####################
        //  type=CHAPTER
        asprintf( &write_data_p, "    CHAPTER:    " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( list_query_count( rcb_p->recipe_p->chapter_p ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->chapter_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->chapter_p, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                }
            }
        }
        //  End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  COPYRIGHT:
    if ( rcb_p->recipe_p->copyright_p != NULL )
    {
        asprintf( &write_data_p, "COPYRIGHT:      %s\n", rcb_p->recipe_p->copyright_p );
    }
    else
    {
        asprintf( &write_data_p, "COPYRIGHT:      %s\n",
                  "CC BY-NC 2.0 https://creativecommons.org/licenses/by-nc/2.0/" );
    }
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  SKILL LEVEL:
    asprintf( &write_data_p, "SKILL LEVEL:    " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->skill_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->skill_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "*\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  RATING:
    asprintf( &write_data_p, "RATING:         " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->rating_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->rating_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "*\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  Source Info:
    asprintf( &write_data_p, "\n----- Source Info -----\n\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  Recipe-Id
    asprintf( &write_data_p, "Recipe-ID:      %s\n", rcb_p->recipe_p->recipe_id_p );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  SOURCE FORMAT:
    switch ( rcb_p->recipe_format )
    {
        case    RECIPE_FORMAT_BOF:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Big Oven Format\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_CP2:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Cooken Pro 2.0\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_ERD:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Easy Recipe Deluxe\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_GRF:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Generic Format #1     [[[[[\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_GF2:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Generic Format #2     @@@@@\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_MMF:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  MealMaster Format\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_MXP:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  MasterCook eXport\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_MX2:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  MasterCook XML\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_NYC:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Now You're Cooking!\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_RXF:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Recipe eXchange Format\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_TXT:
        {
            asprintf( &write_data_p, "SOURCE FORMAT:  Unformatted text data\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        default :
        {
            log_write( MID_WARNING, "encode_rxf",
                       "Unknown recipe format ( %d ) detected @ Line %d\n",
                       rcb_p->recipe_format, __LINE__ );
        }   break;
    }
    //-----------------------------------------------------------------------
    //  <IMPORTED_FROM>
    if (    ( rcb_p->file_info_p->dir_name         != NULL )
         || ( rcb_p->file_info_p->file_name        != NULL )
         || ( rcb_p->file_info_p->file_size        != NULL )
         || ( rcb_p->file_info_p->date_time   != NULL ) )
    {
        if ( rcb_p->file_path != NULL )
        {
            asprintf( &write_data_p, "FileName:       %s\n", rcb_p->file_path );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        if ( rcb_p->file_info_p->date_time != NULL )
        {
            asprintf( &write_data_p, "FileSize:       %s\n",
                      rcb_p->file_info_p->file_size );
        list_put_last( rcb_p->export_list_p, write_data_p );
        }
        {
            asprintf( &write_data_p, "FileDateTime:   %s\n",
                      rcb_p->file_info_p->date_time );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <E-MAIL>
    //  ####################
    //  GROUP-NAME:
    if ( rcb_p->email_info_p != NULL )
    {
        asprintf( &write_data_p, "GROUP-NAME:     " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( rcb_p->email_info_p->g_from != NULL )
        {
            asprintf( &write_data_p, "%s", rcb_p->email_info_p->g_from );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
        //  ####################
        //  GROUP-SUBJECT:
        asprintf( &write_data_p, "GROUP-SUBJECT:  " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( rcb_p->email_info_p->g_subject != NULL )
        {
            asprintf( &write_data_p, "%s", rcb_p->email_info_p->g_subject );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
        //  ####################
        //  GROUP-DATE:
        asprintf( &write_data_p, "GROUP-DATE:     " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( rcb_p->email_info_p->g_datetime != NULL )
        {
            asprintf( &write_data_p, "%s", rcb_p->email_info_p->g_datetime );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
        //  ####################
        //  eMail-FROM:
        asprintf( &write_data_p, "eMail-FROM:     " );
        list_put_last( rcb_p->export_list_p, write_data_p );
        if ( rcb_p->email_info_p->e_from != NULL )
        {
            asprintf( &write_data_p, "%s", rcb_p->email_info_p->e_from );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
        //  ####################
        //  eMail-SUBJECT:
        asprintf( &write_data_p, "eMail-SUBJECT:  " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( rcb_p->email_info_p->e_subject != NULL )
        {
            asprintf( &write_data_p, "%s", rcb_p->email_info_p->e_subject );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
        //  ####################
        //  eMail-DATE:
        asprintf( &write_data_p, "eMail-DATE:     " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( rcb_p->email_info_p->e_datetime != NULL )
        {
            asprintf( &write_data_p, "%s", rcb_p->email_info_p->e_datetime );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  Formatted By:
    asprintf( &write_data_p, "FormattedBy:    " );
        list_put_last( rcb_p->export_list_p, write_data_p );
    if ( rcb_p->recipe_p->formatted_by_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->formatted_by_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "Recipe Import Assistant\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  Edited By:
    asprintf( &write_data_p, "EditedBy:       " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->edited_by_p != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->edited_by_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  RXF Recipe end tag
    asprintf( &write_data_p, "-----  Recipe Import Assist End -----\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/