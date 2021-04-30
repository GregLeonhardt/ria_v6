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
 *  library components of the 'encode_txt' library.
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

#define ALLOC_ENCODE_TXT        ( "ALLOCATE STORAGE FOR ENCODE_TXT" )

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
#include <encode_txt_api.h>     //  API for all encode_txt_*        PUBLIC
#include "encode_txt_lib.h"     //  API for all ENCODE_TXT__*       PRIVATE
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
 *  Encode the recipe in 'TXT' format
 *
 *  @param  rcb_p               Pointer to a Recipe Control Block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
encode_txt(
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
     *  Write TXT-XML to the file
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  TXT Recipe start tag
    asprintf( &write_data_p, "@@@@@\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  NAME
    asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->name );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  Section break:
    asprintf( &write_data_p, "\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  DESCRIPTION:
    if ( rcb_p->recipe_p->description != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->description );
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
    if ( list_query_count( rcb_p->recipe_p->ingredient ) > 0 )
    {
        for( auip_p = list_get_first( rcb_p->recipe_p->ingredient );
             auip_p != NULL;
             auip_p = list_get_next( rcb_p->recipe_p->ingredient, auip_p ) )
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
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->directions );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->directions, tmp_data_p ) )
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
    asprintf( &write_data_p, "AUTHOR: " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->author != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->author );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  SERVES:
    asprintf( &write_data_p, "SERVES: " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->serves != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->serves );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  TIME PREP:
    asprintf( &write_data_p, "TIME PREP: " );

    list_put_last( rcb_p->export_list_p, write_data_p );
    if ( rcb_p->recipe_p->time_prep != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_prep );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "0:00\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  TIME COOK:
    asprintf( &write_data_p, "TIME COOK: " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->time_cook != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_cook );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "0:00\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  YIELD:
    asprintf( &write_data_p, "YIELD: " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->makes != NULL )
    {
        asprintf( &write_data_p, "%s", rcb_p->recipe_p->makes );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //  Is there a unit of measurement ?
    if ( rcb_p->recipe_p->makes_unit != NULL )
    {
        //  YES:    Write it
        asprintf( &write_data_p, " %s\n", rcb_p->recipe_p->makes_unit );
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
    asprintf( &write_data_p, "SOURCE: " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->source != NULL )
    {
        asprintf( &write_data_p, "%s\n\n", rcb_p->recipe_p->source );
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

    if ( list_query_count( rcb_p->recipe_p->notes ) > 0 )
    {
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->notes );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->notes, tmp_data_p ) )
        {
            asprintf( &write_data_p, "%s\n", tmp_data_p );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
    }
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

        if ( list_query_count( rcb_p->recipe_p->cuisine ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->cuisine );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->cuisine, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", write_data_p );
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

        if ( list_query_count( rcb_p->recipe_p->occasion ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->occasion );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->occasion, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", write_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                }
            }
        }
        //  End-of-Line
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );

        //  ####################
        //  type=COURSE
        asprintf( &write_data_p, "    COURSE:     " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( list_query_count( rcb_p->recipe_p->course ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->course );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->course, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", write_data_p );
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

        if ( list_query_count( rcb_p->recipe_p->diet ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->diet );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->diet, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", write_data_p );
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

        if ( list_query_count( rcb_p->recipe_p->appliance ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->appliance );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->appliance, tmp_data_p ) )
            {
                if ( first_category == true )
                {
                    asprintf( &write_data_p, "%s", tmp_data_p );
                    list_put_last( rcb_p->export_list_p, write_data_p );
                    first_category = false;
                }
                else
                {
                    asprintf( &write_data_p, ", %s", write_data_p );
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

        if ( list_query_count( rcb_p->recipe_p->chapter ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( tmp_data_p = list_get_first( rcb_p->recipe_p->chapter );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->chapter, tmp_data_p ) )
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
    if ( rcb_p->recipe_p->copyright != NULL )
    {
        asprintf( &write_data_p, "COPYRIGHT:      %s\n", rcb_p->recipe_p->copyright );
    }
    else
    {
        asprintf( &write_data_p, "COPYRIGHT:      %s\n",
                  "CC BY-NC 2.0 https://creativecommons.org/licenses/by-nc/2.0/" );
    }
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  TIME WAIT:
    //  @ToDo: 3 This field isn't decoded yet!
    //          I'm using the resting time as a place holder.
    asprintf( &write_data_p, "TIME WAIT:      " );
    list_put_last( rcb_p->export_list_p, write_data_p );
    if ( rcb_p->recipe_p->time_rest != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_rest );
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
    if ( rcb_p->recipe_p->time_rest != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->time_rest );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "0:00\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  SKILL LEVEL:
    asprintf( &write_data_p, "SKILL LEVEL:    " );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( rcb_p->recipe_p->skill != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->skill );
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

    if ( rcb_p->recipe_p->rating != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->rating );
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
    asprintf( &write_data_p, "Recipe-ID:      %s\n", rcb_p->recipe_p->recipe_id );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  SOURCE FORMAT:
    if ( rcb_p->recipe_format == RECIPE_FORMAT_TXT )
    {
        asprintf( &write_data_p, "SOURCE FORMAT:  TXT\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    if ( rcb_p->recipe_format == RECIPE_FORMAT_MMF )
    {
        asprintf( &write_data_p, "SOURCE FORMAT:  Meal-Master\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
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
            asprintf( &write_data_p, "FileSize:       %s\n", rcb_p->file_info_p->file_size );
        list_put_last( rcb_p->export_list_p, write_data_p );
        }
        {
            asprintf( &write_data_p, "FileDateTime:   %s\n", rcb_p->file_info_p->date_time );
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
        asprintf( &write_data_p, "GROUP_SUBJET:   " );
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
        asprintf( &write_data_p, "GROUP_DATE:     " );
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
        //  eMAIL-SUBJECT:
        asprintf( &write_data_p, "eMAIL-SUBJECT:  " );
        list_put_last( rcb_p->export_list_p, write_data_p );

        if ( rcb_p->email_info_p->e_subject != NULL )
        {
            asprintf( &write_data_p, "%s", rcb_p->email_info_p->e_subject );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
        //  ####################
        //  eMAIL-DATE:
        asprintf( &write_data_p, "eMAIL-DATE:     " );
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
    if ( rcb_p->recipe_p->formatted_by != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->formatted_by );
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

    if ( rcb_p->recipe_p->edited_by != NULL )
    {
        asprintf( &write_data_p, "%s\n", rcb_p->recipe_p->edited_by );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  TXT Recipe end tag
    asprintf( &write_data_p, "\n@@@@@\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/