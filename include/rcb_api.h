/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef RCB_API_H
#define RCB_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external
 *  library components of the 'rcb' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  rcb                 Recipe Control Block                        */
struct  rcb_t
{
    /**
     *  @param  file_info_p     Pointer to a file information structure     */
    struct  file_info_t     *   file_info_p;
    /**
     *  @param  display_name    Display file name                           */
    char                        display_name[ FILENAME_MAX ];
    /**
     *  @param  destination     Destination Thread                          */
    enum    destination_e       dst_thread;
    /**
     * @param file_ll_p         Pointer to a list where the file is stored  */
    struct  list_base_t     *   import_list_p;
    /**
     *  @param  file_p          File pointer for the import file            */
    FILE                    *   file_p;
    /**
     *  @param  recipe_p        Pointer to a recipe structure               */
    struct  recipe_t        *   recipe_p;
    /**
     *  @param  email_info_p    e-Mail from, date, time etc                 */
    struct  source_info_t   *   email_info_p;
    /**
     *  @param  recipe_format   Format code for this recipe                 */
    enum    recipe_format_e     recipe_format;
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Storage Allocation
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
void
rcb_kill(
    struct  rcb_t           *   rcb_p
    );
//---------------------------------------------------------------------------
struct  rcb_t   *
rcb_new(
    struct  rcb_t           *   rcb_p,
    enum    recipe_format_e     recipe_format
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    RCB_API_H
