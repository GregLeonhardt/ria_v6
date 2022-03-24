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
 *  library components of the 'encode_xml' library.
 *
 *  @note
 *      <?xml version="1.0" encoding="UTF-8"?>
 *      <!DOCTYPE PepperAndSalt>
 *      <PepperAndSalt>
 *          <title>The Recipe Name</title>
 *          <description>A one line recipe description.</description>
 *          <auip-list>
 *              <auip>					# One (1) to many
 *                  <type></type>                       # I - Ingredient
 *                                                      # A - Alternant ingredient
 *							# R - Recipe
 *                  <amount></amount>                   # Numbers and fractions only
 *                  <unit></unit>                       # Unit of mesurement
 *                  <ingredient></ingredient>           # The ingredient
 *                  <preperation></preperation>         # ingredient preperation
 *		</auip>
 *	</auip-list>
 *	<directions>
 *          This is how the recipe is prepared.
 *          The minimum number of text lines is one.
 *          The maximum number is unlimeted.
 *          The text lines have a maximum length of 1MB.
 *
 *          Blank lines are acceptable.
 *	</directions>
 *	<notes>
 *		Formatted the same as the directions above.
 *	</notes>
 *	<recipe-data>
 *          <author></author>                           # Who created the recipe
 *          <yield>
 *              <serves></serves>                       # Number of portions the recipe makes
 *              <makes-amount></makes-amount>           # How many of something the recipe makes
 *              <makes-units></makes-units>             # what the recipe makes
 *          </yield>
 *          <time>
 *              <prep>0:00</prep>                       # Preperation time
 *              <cook>0:00</cook>                       # Cooking time
 *              <wait>0:00</wait>                       # Marinade time, etc.
 *              <rest>0:00</rest>                       # Resting time
 *          </time>
 *          <source></source>			        # Where the recipe was published.
 *          <categories>
 *              <course></course>                       # Zero (0) to many
 *              <diet></diet>                           # Zero (0) to many
 *              <diet></diet>
 *              <cuisine></cuisine>
 *              <occasion></occasion>                   # Typically going to be some sort of holiday.
 *              <appliance></appliance>
 *              <chapter></chapter>
 *          </categories>
 *          <copyright></copyright>
 *          <skill-level></skill-level>                 # A number from one (1) to five (5)
 *          <rating></rating>                           # A number from one (1) to five (5)
 *      </recipe-data>
 *      <source-info>
 *          <recipe-id></recipe-id>
 *          <source-format></source-formt>              # Recipe format of the imported recipe.
 *          <file-name></file-name>                     # The file the recipe was imported from.
 *          <file-size></file-size>                     # Number of bytes in the source file.
 *          <file-data-time><file-dte-time>
 *          <group-name></group-name>
 *          <group-subject></group-subject>
 *          <group-date-time></group-date-time>
 *          <email-name></email-name>
 *          <email-subject></email-subject>
 *          <email-date-time></email-date-time>
 *          <edited-by></edited-by>                     # Zero (0) to many
 *      </source-info>
 *  </PepperAndSalt>
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE_XML        ( "ALLOCATE STORAGE FOR ENCODE_XML" )

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
#include "encode_xml_lib.h"     //  API for all ENCODE_XML__*       PRIVATE
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
 *  Encode the recipe in 'XML' format
 *
 *  @param  rcb_p               Pointer to a Recipe Control Block
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
encode_xml(
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
     *  Write XML-XML to the file
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  XML Recipe start tag
    asprintf( &write_data_p, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    asprintf( &write_data_p, "<!DOCTYPE PepperAndSalt>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    asprintf( &write_data_p, "<PepperAndSalt>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <TITLE><?TITLE>
    asprintf( &write_data_p, "    <title>%s</title>\n", 
                rcb_p->recipe_p->name_p );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <DESCRIPTION></DESCRIPTION>
    if ( rcb_p->recipe_p->description_p != NULL )
    {
        asprintf( &write_data_p, "    <description>%s</description>\n", 
                    rcb_p->recipe_p->description_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "    <description></description>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <AUIP>

    //  Start tag
    asprintf( &write_data_p, "    <auip-list>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    //  AUIP list
    if ( list_query_count( rcb_p->recipe_p->ingredient_p ) > 0 )
    {
        for( auip_p = list_get_first( rcb_p->recipe_p->ingredient_p );
             auip_p != NULL;
             auip_p = list_get_next( rcb_p->recipe_p->ingredient_p, auip_p ) )
        {
            //-----------------------------------------------------------------
            //  <TYPE></TYPE>
            if ( auip_p->type_p != NULL )
            {
                asprintf( &write_data_p, "        <type>?%s</type>\n", auip_p->type_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            else
            {
                asprintf( &write_data_p, "        <type>I</type>\n" );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //-----------------------------------------------------------------
            //  <AMOUNT></AMOUNT>
            if ( auip_p->amount_p != NULL )
            {
                asprintf( &write_data_p, "        <amount>%s</amount>\n", auip_p->amount_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            else
            {
                asprintf( &write_data_p, "        <amount></amount>\n" );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //-----------------------------------------------------------------
            //  <UNIT></UNIT>
            if ( auip_p->unit_p != NULL )
            {
                asprintf( &write_data_p, "        <unit>%s</unit>\n", auip_p->unit_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            else
            {
                asprintf( &write_data_p, "        <unit></unit>\n" );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //-----------------------------------------------------------------
            //  <INGREDIENT></INGREDIENT>
            if ( auip_p->ingredient_p != NULL )
            {
                asprintf( &write_data_p, "        <ingredient>%s</ingredient>\n", auip_p->ingredient_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            else
            {
                asprintf( &write_data_p, "        <ingredient></ingredient>\n" );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            //-----------------------------------------------------------------
            //  <PREPERATION></PREPERATION>
            if ( auip_p->preparation_p != NULL )
            {
                asprintf( &write_data_p, "        <preperation>%s</preperation>\n", auip_p->preparation_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
            else
            {
                asprintf( &write_data_p, "        <preperation></preperation>\n" );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
    }

    //  Ending tag
    asprintf( &write_data_p, "    </auip-list>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    //-----------------------------------------------------------------------
    //  <DIRECTIONS></DIRECTIONS>

    //  Start tag
    asprintf( &write_data_p, "    <directions>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

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
                asprintf( &write_data_p, "        %s\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
    }

    //  Ending tag
    asprintf( &write_data_p, "    </directions>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <NOTES></NOTES>

    //  Start tag
    asprintf( &write_data_p, "    <notes>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    if ( list_query_count( rcb_p->recipe_p->notes_p ) > 0 )
    {
        for( tmp_data_p = list_get_first( rcb_p->recipe_p->notes_p );
             tmp_data_p != NULL;
             tmp_data_p = list_get_next( rcb_p->recipe_p->notes_p, tmp_data_p ) )
        {
            asprintf( &write_data_p, "        %s\n", tmp_data_p );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
    }

    //  Ending tag
    asprintf( &write_data_p, "    </notes>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    //-----------------------------------------------------------------------
    //  <RECIPE-DATA>

    //  Start tag
    asprintf( &write_data_p, "    <recipe-data>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    //-----------------------------------------------------------------------
    //  <AUTHOR></AUTHOR>
    if ( rcb_p->recipe_p->author_p != NULL )
    {
        asprintf( &write_data_p, "        <author>%s</author>\n", rcb_p->recipe_p->author_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <author></author>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <YIELD>
    asprintf( &write_data_p, "        <yield>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    //-----------------------------------------------------------------------
    //  <SERVES></SERVES>
    if ( rcb_p->recipe_p->serves_p != NULL )
    {
        asprintf( &write_data_p, "            <serves>%s</serves>\n", rcb_p->recipe_p->serves_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "            <serves></serves>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <MAKES-AMOUNT></MAKES-AMOUNT>
    if ( rcb_p->recipe_p->makes_p != NULL )
    {
        asprintf( &write_data_p, "            <makes-amount>%s</makes-amount>\n", rcb_p->recipe_p->makes_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "            <makes-amount></makes-amount>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <MAKES-UNITS></MAKES-UNITS>
    if ( rcb_p->recipe_p->makes_unit_p != NULL )
    {
        asprintf( &write_data_p, "            <makes-units>%s</makes-units>\n", rcb_p->recipe_p->makes_unit_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "            <makes-units></makes-units>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  </YIELD>
    asprintf( &write_data_p, "        </yield>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <TIME>
    asprintf( &write_data_p, "        <time>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <PREP></PREP>
    if ( rcb_p->recipe_p->time_prep_p != NULL )
    {
        asprintf( &write_data_p, "            <prep>%s</prep>\n", rcb_p->recipe_p->time_prep_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "            <prep>0:00</prep>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <COOK></COOK>
    if ( rcb_p->recipe_p->time_cook_p != NULL )
    {
        asprintf( &write_data_p, "            <cook>%s</cook>\n", rcb_p->recipe_p->time_cook_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "            <cook>0:00</cook>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <WAIT></WAIT>
    //  @ToDo: 3 This field isn't decoded yet!
    //          I'm using the resting time as a place holder.
    //          --->>>  What is the difference between WAIT and REST ?
    if ( rcb_p->recipe_p->time_rest_p != NULL )
    {
        asprintf( &write_data_p, "            <wait>%s</wait>\n", rcb_p->recipe_p->time_wait_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "            <wait>0:00</wait>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <REST></REST>
    if ( rcb_p->recipe_p->time_rest_p != NULL )
    {
        asprintf( &write_data_p, "            <rest>%s</rest>\n", rcb_p->recipe_p->time_rest_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "            <rest>0:00</rest>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  </TIME>
    asprintf( &write_data_p, "        </time>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <SOURCE></SOURCE>
    if ( rcb_p->recipe_p->source_p != NULL )
    {
        asprintf( &write_data_p, "        <source>%s</source>\n", rcb_p->recipe_p->source_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <source></source>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <CATEGORIES></CATEGORIES>
    asprintf( &write_data_p, "    <categories>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    {
        //  ####################
        //  <CUISINE></CUISINE>
        if ( list_query_count( rcb_p->recipe_p->cuisine_p ) != 0 )
        {
            for( tmp_data_p = list_get_first( rcb_p->recipe_p->cuisine_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->cuisine_p, tmp_data_p ) )
            {
                asprintf( &write_data_p, "        <cuisine>%s<c/uisine>\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
        else
        {
            asprintf( &write_data_p, "        <cuisine></cuisine>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }

        //  ####################
        //  <OCCASION></OCCASION>
        if ( list_query_count( rcb_p->recipe_p->occasion_p ) != 0 )
        {
            for( tmp_data_p = list_get_first( rcb_p->recipe_p->occasion_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->occasion_p, tmp_data_p ) )
            {
                asprintf( &write_data_p, "        <occasion>%s<?occasion>\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
        else
        {
            asprintf( &write_data_p, "        <occasion></occasion>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }

        //  ####################
        //  <COURSE></COURSE>
        if ( list_query_count( rcb_p->recipe_p->course_p ) != 0 )
        {
            for( tmp_data_p = list_get_first( rcb_p->recipe_p->course_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->course_p, tmp_data_p ) )
            {
                asprintf( &write_data_p, "        <course>%s</course>\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
        else
        {
            asprintf( &write_data_p, "        <course></course>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }

        //  ####################
        //  <DIET></DIET>
        if ( list_query_count( rcb_p->recipe_p->diet_p ) != 0 )
        {
            for( tmp_data_p = list_get_first( rcb_p->recipe_p->diet_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->diet_p, tmp_data_p ) )
            {
                asprintf( &write_data_p, "        <diet>%s</diet>\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
        else
        {
            asprintf( &write_data_p, "        <diet></diet>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }

        //  ####################
        //  <APPLIANCE></APPLIANCE>
        if ( list_query_count( rcb_p->recipe_p->appliance_p ) != 0 )
        {
            for( tmp_data_p = list_get_first( rcb_p->recipe_p->appliance_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->appliance_p, tmp_data_p ) )
            {
                asprintf( &write_data_p, "        <appliance>%s</appliance>\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
        else
        {
            asprintf( &write_data_p, "        <appliance></appliance>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }

        //  ####################
        //  <CHAPTER></CHAPTER>
        if ( list_query_count( rcb_p->recipe_p->chapter_p ) != 0 )
        {
            for( tmp_data_p = list_get_first( rcb_p->recipe_p->chapter_p );
                 tmp_data_p != NULL;
                 tmp_data_p = list_get_next( rcb_p->recipe_p->chapter_p, tmp_data_p ) )
            {
                asprintf( &write_data_p, "        <chapter>%s</chapter>\n", tmp_data_p );
                list_put_last( rcb_p->export_list_p, write_data_p );
            }
        }
        else
        {
            asprintf( &write_data_p, "        <chapter></chapter>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  </CATEGORIES>
    asprintf( &write_data_p, "    </categories>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <COPYRIGHT></COPYRIGHT>
    if ( rcb_p->recipe_p->copyright_p != NULL )
    {
        asprintf( &write_data_p, "        <copyright>%s</copyright>\n", rcb_p->recipe_p->copyright_p );
    }
    else
    {
        asprintf( &write_data_p, "        <copyright>%s</copyright>\n",
                  "CC BY-NC 2.0 https://creativecommons.org/licenses/by-nc/2.0/" );
    }
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <SKILL LEVEL></SKILL LEVEL>
    if ( rcb_p->recipe_p->skill_p != NULL )
    {
        asprintf( &write_data_p, "        <skill-level>%s</skill-level>\n", rcb_p->recipe_p->skill_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <skill-level></skill-level>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <RATING></RATING>
    if ( rcb_p->recipe_p->rating_p != NULL )
    {
        asprintf( &write_data_p, "        <rating>%s</rating>\n", rcb_p->recipe_p->rating_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <rating></rating>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }

    //-----------------------------------------------------------------------
    //  </RECIPE-DATA>
    asprintf( &write_data_p, "    </recipe-data>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    //-----------------------------------------------------------------------
    //  <SOURCE-INFO>
    asprintf( &write_data_p, "    <source-info>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <RECIPE-ID></RECIPE-ID>
    asprintf( &write_data_p, "        <recipe-id>%s</recipe-id>\n", rcb_p->recipe_p->recipe_id_p );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  <SOURCE-FORMAT></SOURCE-FORMAT>
    switch ( rcb_p->recipe_format )
    {
        case    RECIPE_FORMAT_BOF:
        {
            asprintf( &write_data_p, "        <source-format>Big Oven Format</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_CP2:
        {
            asprintf( &write_data_p, "        <source-format>Cooken Pro 2.0</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_ERD:
        {
            asprintf( &write_data_p, "        <source-format>Easy Recipe Deluxe</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_GRF:
        {
            asprintf( &write_data_p, "        <source-format>Generic Format #1     [[[[[</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_GF2:
        {
            asprintf( &write_data_p, "        <source-format>Generic Format #2     @@@@@</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_MMF:
        {
            asprintf( &write_data_p, "        <source-format>MealMaster Format</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_MXP:
        {
            asprintf( &write_data_p, "        <source-format>MasterCook eXport</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_MX2:
        {
            asprintf( &write_data_p, "        <source-format>MasterCook XML</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_NYC:
        {
            asprintf( &write_data_p, "        <source-format>Now You're Cooking!</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_RXF:
        {
            asprintf( &write_data_p, "        <source-format>Recipe eXchange Format</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_TXT:
        {
            asprintf( &write_data_p, "        <source-format>Unformatted text data</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        case    RECIPE_FORMAT_XML:
        {
            asprintf( &write_data_p, "        <source-format>Recipe XML Format</source-format>\n" );
            list_put_last( rcb_p->export_list_p, write_data_p );
        }   break;
        default :
        {
            log_write( MID_WARNING, "encode_xml",
                       "Unknown recipe format ( %d ) detected @ Line %d\n",
                       rcb_p->recipe_format, __LINE__ );
        }   break;
    }
    //-----------------------------------------------------------------------
    //  <FILE-NAME></FILE-NAME>
    if ( rcb_p->file_info_p->dir_name != NULL )
    {
        asprintf( &write_data_p, "        <file-name>%s</file-name>\n", rcb_p->file_path );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <file-name></file-name>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <FILE-SIZE></FILE-SIZE>
    if ( rcb_p->file_info_p->file_size        != NULL )
    {
        asprintf( &write_data_p, "        <file-size>%s</file-size>\n",
                      rcb_p->file_info_p->file_size );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <file-size></file-size>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <FILE-DATE-TIME></FILE-DATE-TIME>
    if ( rcb_p->file_info_p->date_time   != NULL )
    {
        asprintf( &write_data_p, "        <file-date-time>%s</file-date-time>\n",
                  rcb_p->file_info_p->date_time );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <GROUP-NAME></GROUP-NAME>
    if ( rcb_p->email_info_p->g_from != NULL )
    {
        asprintf( &write_data_p, "        <group-name>%s</group-name>\n", rcb_p->email_info_p->g_from );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <group-name></group-name>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <GROUP-SUBJECT></GROUP-SUBJECT>
    if ( rcb_p->email_info_p->g_subject != NULL )
    {
        asprintf( &write_data_p, "        <group-subject>%s</group-subject>\n", rcb_p->email_info_p->g_subject );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <group-subject></group-subject>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <GROUP-DATE></GROUP-DATE>
    if ( rcb_p->email_info_p->g_datetime != NULL )
    {
        asprintf( &write_data_p, "        <group-date>%s</group-date>\n", rcb_p->email_info_p->g_datetime );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <group-date></group-date>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <EMAIL-NAME></EMAIL-NAME>
    if ( rcb_p->email_info_p->e_from != NULL )
    {
        asprintf( &write_data_p, "        <<email-name>%s</email-name>\n", rcb_p->email_info_p->e_from );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <email-name></email-name>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <EMAIL-SUBJECT></EMAIL-SUBJECT>
    if ( rcb_p->email_info_p->e_subject != NULL )
    {
        asprintf( &write_data_p, "        <email-subject>%s</email-subject>\n", rcb_p->email_info_p->e_subject );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <email-subject></email-subject>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <EMAIL-DATE-TIME></EMAIL-DATE-TIME>
    if ( rcb_p->email_info_p->e_datetime != NULL )
    {
        asprintf( &write_data_p, "        <email-date-time>%s</email-date-time>\n", rcb_p->email_info_p->e_datetime );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <email-date-time></email-date-time>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  <EDITED-BY></EDITED-BY>
    if ( rcb_p->recipe_p->edited_by_p != NULL )
    {
        asprintf( &write_data_p, "        <edited-by>%s</edited-by>\n", rcb_p->recipe_p->edited_by_p );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    else
    {
        asprintf( &write_data_p, "        <edited-by></edited-by>\n" );
        list_put_last( rcb_p->export_list_p, write_data_p );
    }
    //-----------------------------------------------------------------------
    //  </SOURCE-INFO>

    //  Start tag
    asprintf( &write_data_p, "    </source-info>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );
    //-----------------------------------------------------------------------
    //  XML Recipe end tag
    asprintf( &write_data_p, "</PepperAndSalt>\n" );
    list_put_last( rcb_p->export_list_p, write_data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/