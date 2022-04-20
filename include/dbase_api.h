/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef DBASE_API_H
#define DBASE_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external 
 *  library components of the 'dbase' library.
 *
 * DATETIME is stored as 'YYYY-MM-DD hh:mm:ss'
 * 
 *  @note
 *      TABLE   ROW             TYPE
 *      -----   -----           ---------------------------------------------
 *  #   TITLE   RECIPE_ID       VARCHAR(50)     PRIMARY KEY
 *              TITLE           TINYBLOB
 *  #   RECIPE
 *              RECIPE-ID       VARCHAR(50)     PRIMARY KEY
 *              RECIPE          BLOB
 *  #   INFO
 *              RECIPE-ID       VARCHAR(50)     PRIMARY KEY
 *              AUTHOR          TINYBLOB
 *              YIELD           TINYBLOB
 *              TIME-PREP       VARCHAR(10)
 *              TIME-COOK       VARCHAR(10)
 *              TIME-WAIT       VARCHAR(10)
 *              TIME-REST       VARCHAR(10)
 *              SOURCE          TINYBLOB
 *              COPYRIGHT       TINYBLOB
 *              SKILL           TINYINT
 *              RATING          TINYINT
 *      CUISINE
 *              RECIPE-ID       VARCHAR(50)
 *              NAME            VARCHAR(35)
 *      OCCASION
 *              RECIPE-ID       VARCHAR(50)
 *              NAME            VARCHAR(35)
 *      COURSE
 *              RECIPE-ID       VARCHAR(50)
 *              NAME            VARCHAR(35)
 *      DIET
 *              RECIPE-ID       VARCHAR(50)
 *              NAME            VARCHAR(35)
 *      APPLIANCE
 *              RECIPE-ID       VARCHAR(50)
 *              NAME            VARCHAR(35)
 *      CHAPTER
 *              RECIPE-ID       VARCHAR(50)
 *              NAME            VARCHAR(35)
 *      SOURCE
 *              RECIPE-ID       VARCHAR(50)
 *              SRC-FORMAT      TINYBLOB
 *              FILE-NAME       TINYBLOB
 *              FILE-SIZE       INTEGER
 *              FILE-DATE-TIME  DATETIME
 *              GROUP-NAME      TINYBLOB
 *              GROUP-SUBJECT   TINYBLOB
 *              GROUP-DATE-TIME DATETIME
 *              EMAIL-NAME      TINYBLOB
 *              EMAIL-SUBJECT   TINYBLOB
 *              EMAIL-DATE-TIME DATETIME
 *      EDITED-BY
 *              RECIPE-ID       VARCHAR(50)
 *              USER-ID         VARCHAR(50)
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
#define RECIPE_ID_L                 ( 45 )
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
struct  recipe_table
{
    char                            recipe_id[ RECIPE_ID_L ];
    char                        *   recipe_data;
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
int
dbase_init(
    void
    );
//---------------------------------------------------------------------------
int
dbase_insert(
    struct  rcb_t           *   rcb_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DBASE_API_H
