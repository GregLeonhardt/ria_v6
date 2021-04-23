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
 *  library components of the 'post' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_POST          ( "ALLOCATE STORAGE FOR POST" )

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
#include "libtools_api.h"       //  My Tools Library
                                //*******************************************
#include "tcb_api.h"            //  API for all tcb_*               PUBLIC
#include "rcb_api.h"            //  API for all rcb_*               PUBLIC
                                //*******************************************
#include "decode_post_api.h"    //  API for all post_*              PUBLIC
#include "decode_post_lib.h"    //  API for all POST__*             PRIVATE
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
decode_post(
    struct  rcb_t           *   rcb_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Recipe Title analysis
    DECODE_POST__title_information( rcb_p );

    if ( rcb_p->recipe_p->instructions != NULL )
    {
        //  Directions  analysis
        DECODE_POST__directions_cleanup( rcb_p );

        //  Directions format
        DECODE_POST__fmt_directions( rcb_p );

        //  Find "FROM:" in the directions
        DECODE_POST__directions_from( rcb_p );

        //  Find "SOURCE:" in the directions
        DECODE_POST__directions_source( rcb_p );

        //  Find "COPYRIGHT:" in the directions
        DECODE_POST__directions_copyright( rcb_p );

        //  Find "DESCRIPTION:" in the directions
        DECODE_POST__directions_description( rcb_p );

        //  Find "MAKES:" in the directions
        DECODE_POST__directions_makes( rcb_p );

        //  Find "TIME-wxyz:" in the directions
        DECODE_POST__directions_time( rcb_p );

        //  Find "S(Imported From):" in the directions
        DECODE_POST__directions_import_from( rcb_p );

        //  @ToDo: 3 Ratings:

        //  Find "NOTES:" in the directions
        DECODE_POST__directions_notes( rcb_p );
    }

    //  Compute the recipe checksum (Recipe-ID)
    DECODE_POST__recipe_id( rcb_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/