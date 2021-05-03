/*******************************  COPYRIGHT  ********************************/
/**
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef EMAIL_API_H
#define EMAIL_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external
 *  library components of the 'email' library.
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
#define SOURCE_L                ( 1024 )
#define FROM_L                  ( 1024 )
#define DATETIME_L              ( 1024 )
#define SUBJECT_L               ( 1024 )
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
struct  email_info_t
{
    //  @ToDo: 2 Refactor buffer pointers with '_p'
    //------------------------------------------------------------------------
    /**
     *  @param  e_source        Group-From                                  */
    char                            g_from[      FROM_L      + 1 ];
    /**
     *  @param  e_subject       Group_Subject                               */
    char                            g_subject[   SUBJECT_L   + 1 ];
    /**
     *  @param  e_datetime      Group sent Date/Time                        */
    char                            g_datetime[  DATETIME_L  + 1 ];
    //------------------------------------------------------------------------
    /**
     *  @param  e_from          Who posted the e-Mail message               */
    char                            e_from[      FROM_L      + 1 ];
    /**
     *  @param  e_subject       Subject of the original e-Mail              */
    char                            e_subject[   SUBJECT_L   + 1 ];
    /**
     *  @param  e_datetime      When the e-Mail was originally posted       */
    char                            e_datetime[  DATETIME_L  + 1 ];
    //------------------------------------------------------------------------
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
email_is_group_break(
    char                        *   data_p
    );
//---------------------------------------------------------------------------
void
email(
    void                    *   void_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    EMAIL_API_H
