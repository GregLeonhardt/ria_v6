/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef DECODE_API_H
#define DECODE_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external
 *  library components of the 'decode' library.
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
#define APPLIANCE               "Appliance:"
#define APPLIANCE_L             strlen( APPLIANCE )
//----------------------------------------------------------------------------
#define DIET                    "Diet:"
#define DIET_L                  strlen( DIET )
//----------------------------------------------------------------------------
#define COURSE                  "Course:"
#define COURSE_L                strlen( COURSE )
//----------------------------------------------------------------------------
#define CUISINE                 "Cuisine:"
#define CUISINE_L               strlen( CUISINE )
//----------------------------------------------------------------------------
#define OCCASION                "Occasion:"
#define OCCASION_L              strlen( OCCASION )
//----------------------------------------------------------------------------
#define CATEGORY                "Category:"
#define CATEGORY_L              strlen( CATEGORY )
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
char    *
decode_is_tag(
    char                        *   data_p,
    char                        *   tag_p
    );
//---------------------------------------------------------------------------
void
decode_fmt_auip(
    struct   recipe_t       *   recipe_p,
    char                    *   auip_data_p,
    enum    recipe_format_e         recipe_format
    );
//---------------------------------------------------------------------------
void
decode_name_cleanup(
    char                    *   name_p,
    int                         name_l
    );
//---------------------------------------------------------------------------
void
decode_fmt_notes(
    struct   recipe_t       *   recipe_p,
    char                    *   data_p
    );
//---------------------------------------------------------------------------
void
decode_next_id(
    struct  recipe_t        *   recipe_p
    );
//---------------------------------------------------------------------------
void
decode_append(
    struct  list_base_t     *   list_p,
    char                    *   data_p
    );
//---------------------------------------------------------------------------
void
decode_add_instructions(
    struct   recipe_t       *   recipe_p,
    char                    *   data_p
    );
//---------------------------------------------------------------------------
void
decode_save_chapter(
    char                    *   data_p,
    struct   recipe_t       *   recipe_p
    );
//---------------------------------------------------------------------------
void
decode(
    void                    *   void_p
    );
//---------------------------------------------------------------------------

/****************************************************************************
 *  DECODE_POST_API
 ****************************************************************************/

//---------------------------------------------------------------------------
void
decode_post(
    struct  rcb_t           *   rcb_p
    );
//---------------------------------------------------------------------------

/****************************************************************************
 *  DECODE_MMF_API
 ****************************************************************************/

//---------------------------------------------------------------------------
int
mmf_is_start(
    char                    *   data_p
    );
//---------------------------------------------------------------------------
int
mmf_is_end(
    char                    *   data_p
    );
//----------------------------------------------------------------------------
void
decode_mmf(
    struct  rcb_t           *   rcb_p;
    );
//----------------------------------------------------------------------------

/****************************************************************************
 *  DECODE_MXP_API
 ****************************************************************************/

//---------------------------------------------------------------------------
int
mxp_is_start(
    char                    *   data_p
    );
//---------------------------------------------------------------------------
int
mxp_is_end(
    char                    *   data_p
    );
//---------------------------------------------------------------------------
void
decode_mxp(
    struct  rcb_t           *   rcb_p;
    );
//---------------------------------------------------------------------------

/****************************************************************************
 *  DECODE_BOF_API
 ****************************************************************************/

//---------------------------------------------------------------------------
int
bof_is_start(
    char                    *   data_p
    );
//---------------------------------------------------------------------------
int
bof_is_end(
    char                    *   data_p
    );
//---------------------------------------------------------------------------
void
decode_bof(
    struct  rcb_t           *   rcb_p;
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DECODE_API_H
