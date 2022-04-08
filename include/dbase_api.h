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
 *  @note
 *      TABLE   ROW             TYPE
 *      -----   -----           ---------------------------------------------
 *      RECIPE
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
 *              RECIPE          BLOB
 *      INFO
 *              RECIPE-ID       TINYBLOB
 *              AUTHOR          TINYBLOB
 *              YIELD           TINYBLOB
 *              TIME-PREP       DATETIME
 *              TIME-COOK       DATETIME
 *              TIME-WAIT       DATETIME
 *              TIME-REST       DATETIME
 *              SOURCE          TINYBLOB
 *              COPYRIGHT       TINYBLOB
 *              SKILL           TINYINT
 *              RATING          TINYINT
 *      CUISINE
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
 *              NAME            TINYBLOB
 *      OCCASION
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
 *              NAME            TINYBLOB
 *      COURSE
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
 *              NAME            TINYBLOB
 *      DIET
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
 *              NAME            TINYBLOB
 *      APPLIANCE
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
 *              NAME            TINYBLOB
 *      CHAPTER
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
 *              NAME            TINYBLOB
 *      SOURCE
 *              RECIPE-ID       TINYBLOB        PRIMARY KEY
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
 *              RECIPE-ID       TINYBLOB
 *              USER-ID         TINYBLOB
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

/****************************************************************************/

#endif                      //    DBASE_API_H
