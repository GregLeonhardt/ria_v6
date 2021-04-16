/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019,2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'encode_txt' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE_TXT        ( "ALLOCATE STORAGE FOR ENCODE_TXT" )

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
#define DIR_FORMATTED           "Recipes_Formatted"
//----------------------------------------------------------------------------
//  @ToDo:  SUBDIRECTORY_L ?
#define SUBDIRECTORY_L          ( 4 )
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
     * @param out_name          Encoded output file name                    */
    char                        out_name[ FILE_NAME_L + 1 ];
    /**
     * @param subdirectory      Sub Directory name based on the Recipe-ID   */
    char                        subdirectory[ SUBDIRECTORY_L + 1 ];
    /**
     * @param input_file_fp     Output File pointer                         */
    FILE                    *   out_file_fp;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   data_p;
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t          *   auip_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Build the subdirectory name
    snprintf( subdirectory, sizeof( subdirectory ),
              "%s", rcb_p->recipe_p->recipe_id );

    /************************************************************************
     *  Open the output file
     ************************************************************************/

    //  Start building the output name
    snprintf( out_name, sizeof( out_name ),
              "%s", out_dir_name_p );

    //  If the directory does not already exist, create it.
    file_dir_exist( out_name, true );

    //  Append DIR_FORMATTED to the output name
    snprintf( out_name, sizeof( out_name ),
              "%s/%s", out_dir_name_p, DIR_FORMATTED );

    //  If the directory does not already exist, create it.
    file_dir_exist( out_name, true );

    //  Append the subdirectory to the output name
    snprintf( out_name, sizeof( out_name ),
              "%s/%s/%s", out_dir_name_p, DIR_FORMATTED, subdirectory );

    //  If the directory does not already exist, create it.
    file_dir_exist( out_name, true );

    //  Append the file name to the output name
    snprintf( out_name, sizeof( out_name ),
              "%s/%s/%s/%s.txt", out_dir_name_p, DIR_FORMATTED,
              subdirectory, rcb_p->recipe_p->recipe_id );

    /************************************************************************
     *  Open the output file
     ************************************************************************/

    //  Is this a recipe with no instructions ?
    if ( strncmp( rcb_p->recipe_p->recipe_id, "0000000000", 10 ) == 0 )
    {
        //  YES:    Append to whatever is already there
        out_file_fp = file_open_append( out_name );
    }
    else
    {
        //  NO:     Open a unique file for the output.
        out_file_fp = file_open_write_no_dup( out_name, "duplicate_" );
    }

    /************************************************************************
     *  Write TXT-XML to the file
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  TXT Recipe start tag
    fprintf( out_file_fp, "@@@@@\n" );
    //-----------------------------------------------------------------------
    //  NAME
    fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->name );
    //-----------------------------------------------------------------------
    //  Section break:
    fprintf( out_file_fp, "|\n" );
    //-----------------------------------------------------------------------
    //  DESCRIPTION:
    if ( rcb_p->recipe_p->description != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->description );
    }
    //-----------------------------------------------------------------------
    //  AUTHOR:
    fprintf( out_file_fp, "Author: " );
    if ( rcb_p->recipe_p->author != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->author );
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  Serves:
    fprintf( out_file_fp, "Serves: " );
    if ( rcb_p->recipe_p->serves != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->serves );
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  TIME PREP:
    fprintf( out_file_fp, "Prep Time: " );
    if ( rcb_p->recipe_p->time_prep != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->time_prep );
    }
    else
    {
        fprintf( out_file_fp, "0:00\n" );
    }
    //-----------------------------------------------------------------------
    //  TIME COOK:
    fprintf( out_file_fp, "Cook Time: " );
    if ( rcb_p->recipe_p->time_cook != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->time_cook );
    }
    else
    {
        fprintf( out_file_fp, "0:00\n" );
    }
    //-----------------------------------------------------------------------
    //  Yield:
    fprintf( out_file_fp, "Yield: " );

    if ( rcb_p->recipe_p->makes != NULL )
    {
        fprintf( out_file_fp, "%s", rcb_p->recipe_p->makes );
    }
    //  Is there a unit of measurement ?
    if ( rcb_p->recipe_p->makes_unit != NULL )
    {
        //  YES:    Write it
        fprintf( out_file_fp, " %s\n", rcb_p->recipe_p->makes_unit );
    }
    else
    {
        //  NO:     End-of-Line
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  Section break:
    fprintf( out_file_fp, "|\n" );
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
                fprintf( out_file_fp, "%-8s",     auip_p->amount_p );
            }
            else
            {
                fprintf( out_file_fp, "        " );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                fprintf( out_file_fp, "%-12s",     auip_p->unit_p );
            }
            else
            {
                fprintf( out_file_fp, "            " );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                fprintf( out_file_fp, "%s",     auip_p->ingredient_p );
            }
            //  Is there a preparation method ?
            if ( auip_p->preparation_p != NULL )
            {
                fprintf( out_file_fp, " %s",     auip_p->preparation_p );
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
    }
    else
    {
        //  No AUIP
        fprintf( out_file_fp, "** TEXT ONLY - NO INGREDIENTS **\n" );
    }
    //-----------------------------------------------------------------------
    //  Section break:
    fprintf( out_file_fp, "|\n" );
    //-----------------------------------------------------------------------
    //  DIRECTIONS
    if ( list_query_count( rcb_p->recipe_p->directions ) > 0 )
    {
        for( data_p = list_get_first( rcb_p->recipe_p->directions );
             data_p != NULL;
             data_p = list_get_next( rcb_p->recipe_p->directions, data_p ) )
        {
            //  Is there something to write ?
            if ( text_is_blank_line( data_p ) != true )
            {
                //  YES:    Write the directions text.
                fprintf( out_file_fp, "%s\n\n", data_p );
            }
        }
    }
    //-----------------------------------------------------------------------
    //  Source:
    fprintf( out_file_fp, "Source: " );
    if ( rcb_p->recipe_p->source != NULL )
    {
        fprintf( out_file_fp, "%s\n\n", rcb_p->recipe_p->source );
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  Extra Stuff:
    fprintf( out_file_fp, "\n----------------------------------------\n\n" );
    //-----------------------------------------------------------------------
    //  Recipe-Id
    fprintf( out_file_fp, "Recipe-ID:      %s\n", rcb_p->recipe_p->recipe_id );
    //-----------------------------------------------------------------------
    //  NOTES:
    fprintf( out_file_fp, "NOTES:\n" );
    if ( list_query_count( rcb_p->recipe_p->notes ) > 0 )
    {
        for( data_p = list_get_first( rcb_p->recipe_p->notes );
             data_p != NULL;
             data_p = list_get_next( rcb_p->recipe_p->notes, data_p ) )
        {
            fprintf( out_file_fp, "%s\n", data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  CATEGORIES
    fprintf( out_file_fp, "Categories:\n" );
    {
        int                             first_category;

        //  ####################
        //  type=CUISINE
        fprintf( out_file_fp, "    CUISINE:    " );
        if ( list_query_count( rcb_p->recipe_p->cuisine ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( data_p = list_get_first( rcb_p->recipe_p->cuisine );
                 data_p != NULL;
                 data_p = list_get_next( rcb_p->recipe_p->cuisine, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
        }
        //  End-of-Line
        fprintf( out_file_fp, "\n" );

        //  ####################
        //  type=OCCASION
        fprintf( out_file_fp, "    OCCASION:   " );
        if ( list_query_count( rcb_p->recipe_p->occasion ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( data_p = list_get_first( rcb_p->recipe_p->occasion );
                 data_p != NULL;
                 data_p = list_get_next( rcb_p->recipe_p->occasion, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
        }
        //  End-of-Line
        fprintf( out_file_fp, "\n" );

        //  ####################
        //  type=COURSE
        fprintf( out_file_fp, "    COURSE:     " );
        if ( list_query_count( rcb_p->recipe_p->course ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( data_p = list_get_first( rcb_p->recipe_p->course );
                 data_p != NULL;
                 data_p = list_get_next( rcb_p->recipe_p->course, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
        }
        //  End-of-Line
        fprintf( out_file_fp, "\n" );

        //  ####################
        //  type=DIET
        fprintf( out_file_fp, "    DIET:       " );
        if ( list_query_count( rcb_p->recipe_p->diet ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( data_p = list_get_first( rcb_p->recipe_p->diet );
                 data_p != NULL;
                 data_p = list_get_next( rcb_p->recipe_p->diet, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
        }
        //  End-of-Line
        fprintf( out_file_fp, "\n" );

        //  ####################
        //  type=APPLIANCE
        fprintf( out_file_fp, "    APPLIANCE:  " );
        if ( list_query_count( rcb_p->recipe_p->appliance ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( data_p = list_get_first( rcb_p->recipe_p->appliance );
                 data_p != NULL;
                 data_p = list_get_next( rcb_p->recipe_p->appliance, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
        }
        //  End-of-Line
        fprintf( out_file_fp, "\n" );

        //  ####################
        //  type=CHAPTER
        fprintf( out_file_fp, "    CHAPTER:    " );
        if ( list_query_count( rcb_p->recipe_p->chapter ) != 0 )
        {
            //  Set the flag
            first_category = true;

            for( data_p = list_get_first( rcb_p->recipe_p->chapter );
                 data_p != NULL;
                 data_p = list_get_next( rcb_p->recipe_p->chapter, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
        }
        //  End-of-Line
        fprintf( out_file_fp, "\n" );

    }
    //-----------------------------------------------------------------------
    //  COPYRIGHT:
    fprintf( out_file_fp, "Copyright:      %s\n", rcb_p->recipe_p->copyright );
    //-----------------------------------------------------------------------
    //  TIME WAIT:
    //  @TODO:  This field isn't decoded yet!
    //          I'm using the resting time as a place holder.
    fprintf( out_file_fp, "Time Wait:      " );
    if ( rcb_p->recipe_p->time_rest != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->time_rest );
    }
    else
    {
        fprintf( out_file_fp, "0:00\n" );
    }
    //-----------------------------------------------------------------------
    //  TIME REST:
    fprintf( out_file_fp, "Time Rest:      " );
    if ( rcb_p->recipe_p->time_rest != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->time_rest );
    }
    else
    {
        fprintf( out_file_fp, "0:00\n" );
    }
    //-----------------------------------------------------------------------
    //  SKILL LEVEL:
    fprintf( out_file_fp, "Skill Level:    " );
    if ( rcb_p->recipe_p->skill != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->skill );
    }
    else
    {
        fprintf( out_file_fp, "*\n" );
    }
    //-----------------------------------------------------------------------
    //  RATING:
    fprintf( out_file_fp, "Rating:         " );
    if ( rcb_p->recipe_p->rating != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->rating );
    }
    else
    {
        fprintf( out_file_fp, "*\n" );
    }
    //-----------------------------------------------------------------------
    //  FORMAT:
    if ( rcb_p->recipe_p->source_format == NULL )
    {
        fprintf( out_file_fp, "Format:         TXT\n" );
    }
    else
    {
        fprintf( out_file_fp, "Format:         %s\n", rcb_p->recipe_p->source_format );
    }
    //-----------------------------------------------------------------------
    //  <IMPORTED_FROM>
    if (    ( rcb_p->recipe_p->dir_name         != NULL )
         || ( rcb_p->recipe_p->file_name        != NULL )
         || ( rcb_p->recipe_p->file_size        != NULL )
         || ( rcb_p->recipe_p->file_date_time   != NULL ) )
    {
        if ( rcb_p->recipe_p->dir_name != NULL )
        {
            fprintf( out_file_fp, "FileDirectory:  %s\n", rcb_p->recipe_p->dir_name );
        }
        if ( rcb_p->recipe_p->file_name != NULL )
        {
            fprintf( out_file_fp, "FileName:       %s\n", rcb_p->recipe_p->file_name );
        }
        if ( rcb_p->recipe_p->file_date_time != NULL )
        {
            fprintf( out_file_fp, "FileSize:       %s\n", rcb_p->recipe_p->file_size );
        }
        {
            fprintf( out_file_fp, "FileDateTime:   %s\n", rcb_p->recipe_p->file_date_time );
        }
    }
    //-----------------------------------------------------------------------
    //  <E-MAIL>
    //  ####################
    //  GROUP-NAME:
    fprintf( out_file_fp, "G-Name:         " );
    if ( rcb_p->recipe_p->group_from != NULL )
    {
        fprintf( out_file_fp, "%s", rcb_p->recipe_p->group_from );
    }
    fprintf( out_file_fp, "\n" );
    //  ####################
    //  GROUP-SUBJECT:
    fprintf( out_file_fp, "G-Subject:      " );
    if ( rcb_p->recipe_p->group_subject != NULL )
    {
        fprintf( out_file_fp, "%s", rcb_p->recipe_p->group_subject );
    }
    fprintf( out_file_fp, "\n" );
    //  ####################
    //  GROUP-DATE:
    fprintf( out_file_fp, "G-Date:         " );
    if ( rcb_p->recipe_p->group_date != NULL )
    {
        fprintf( out_file_fp, "%s", rcb_p->recipe_p->group_date );
    }
    fprintf( out_file_fp, "\n" );
    //  ####################
    //  POSTED-BY:
    fprintf( out_file_fp, "P-By:           " );
    if ( rcb_p->recipe_p->posted_by != NULL )
    {
        fprintf( out_file_fp, "%s", rcb_p->recipe_p->posted_by );
    }
    fprintf( out_file_fp, "\n" );
    //  ####################
    //  POSTED-SUBJECT:
    fprintf( out_file_fp, "P-Subject:      " );
    if ( rcb_p->recipe_p->posted_subject != NULL )
    {
        fprintf( out_file_fp, "%s", rcb_p->recipe_p->posted_subject );
    }
    fprintf( out_file_fp, "\n" );
    //  ####################
    //  POSTED-DATE:
    fprintf( out_file_fp, "P-Date:         " );
    if ( rcb_p->recipe_p->posted_date != NULL )
    {
        fprintf( out_file_fp, "%s", rcb_p->recipe_p->posted_date );
    }
    fprintf( out_file_fp, "\n" );
    //-----------------------------------------------------------------------
    //  Formatted By:
    fprintf( out_file_fp, "FormattedBy:    " );
    if ( rcb_p->recipe_p->formatted_by != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->formatted_by );
    }
    else
    {
        fprintf( out_file_fp, "Recipe Import Assistant\n" );
    }
    //-----------------------------------------------------------------------
    //  Edited By:
    fprintf( out_file_fp, "EditedBy:       " );
    if ( rcb_p->recipe_p->edited_by != NULL )
    {
        fprintf( out_file_fp, "%s\n", rcb_p->recipe_p->edited_by );
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  TXT Recipe end tag
    fprintf( out_file_fp, "_____\n" );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Close the output file.
    file_close( out_file_fp );

    //  DONE!
}
/****************************************************************************/