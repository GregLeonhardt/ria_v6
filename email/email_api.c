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
 *  library components of the 'email' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_EMAIL          ( "ALLOCATE STORAGE FOR EMAIL" )

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
#include "recipe_api.h"         //  API for all recipe_*            PUBLIC
                                //*******************************************
#include "email_api.h"          //  API for all email_*             PUBLIC
#include "email_lib.h"          //  API for all EMAIL__*            PRIVATE
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
 *  Test the text line to see if it contains an e-Mail group break.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

int
email_is_group_break(
    char                        *   data_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( EMAIL__is_group_break( data_p ) );
}

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
email(
    void                    *   void_p
    )
{
    /**
     *  @param  tcb_p           Pointer to a Thread Control Block           */
    struct  tcb_t           *   tcb_p;
    /**
     *  @param  rcb_p           Pointer to a Recipe Control Block           */
    struct  rcb_t           *   rcb_p;
    /**
     *  @param  new_rcb_p       Pointer to a Recipe Control Block           */
    struct  rcb_t           *   new_rcb_p;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                    *   list_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                         list_lock_key;
    /**
     * @param email_flag        A mark on the wall if wr are doing an e-Mail*/
    int                         email_flag;
    /**
     * @param content_type      e-Mail content type                         */
    enum    content_type_e      content_type;
    /**
     * @param encoding_type     e-Mail encoding type                        */
    enum    encoding_type_e     encoding_type;
    /**
     * @param boundary_type     e-Mail boundary type                        */
//  enum    boundary_type_e     boundary_type;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set the pointer
    tcb_p = void_p;

    //  Progress report.
    log_write( MID_INFO, tcb_p->thread_name,
               "Initialization complete.\n" );

    //  Change execution state to "WAIT"ing for work.
    tcb_p->thread_state = TS_WAIT;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    while ( 1 )
    {

        /********************************************************************
         *  Get a new file
         ********************************************************************/

        //  Get the current File-ID.
        rcb_p = queue_get_payload( tcb_p->queue_id );

        //  Progress report.
        log_write( MID_LOGONLY, tcb_p->thread_name,
                   "Q-%03d: Rcv: FILE-ID: %s\n",
                   tcb_p->queue_id, rcb_p->display_name );

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WORKING;

        /********************************************************************
         *  FUNCTIONAL CODE FOR THIS THREAD GOES HERE
         ********************************************************************/

        //  Lock the list for fast(er) access
        list_lock_key = list_user_lock( rcb_p->import_list_p );

        //  Is this the start of a new e-Mail ?
        list_data_p = list_fget_first( rcb_p->import_list_p, list_lock_key );

        //  Is there anything to scan ?
        if ( list_data_p == NULL )
            continue;

        if ( EMAIL__is_start( list_data_p ) == true )
        {
            //  YES:    Set a flag so we can track it.
            email_flag = true;

            //  Reset the content types.
            content_type   = CT_NONE;
            encoding_type  = CTE_NONE;

            log_write( MID_DEBUG_0, tcb_p->thread_name,
                          "Start      %p - '%.80s'\n", list_data_p, list_data_p );

            //  Done with this text line
            list_fdelete( rcb_p->import_list_p, list_data_p, list_lock_key );
            mem_free( list_data_p );
        }

        //  Scan the list
        for( list_data_p = list_fget_first( rcb_p->import_list_p, list_lock_key );
             list_data_p != NULL;
             list_data_p = list_fget_next( rcb_p->import_list_p, list_data_p, list_lock_key ) )
        {
            /**
             * @param tmp_c_type        e-Mail content type                 */
            enum    content_type_e          tmp_c_type;
            /**
             * @param tmp_e_type        e-Mail encoding type                */
            enum    encoding_type_e         tmp_e_type;

            //  Remove the data from the level 1 list
            list_fdelete( rcb_p->import_list_p, list_data_p, list_lock_key );

            tmp_c_type = EMAIL__find_content( list_data_p );
            tmp_e_type = EMAIL__find_encoding( list_data_p );
#if 0
            /****************************************************************/
            //  Content-Type: text/plain; charset="utf-8"
            /****************************************************************/

            //  Do we have a new content type ?
            if (    ( email_flag == true    )
                 && ( tmp_c_type != CT_NONE ) )
            {
                //  YES:    Set the new encoding type
                content_type = tmp_c_type;

                //  Just in case it is on, turn the boundary search flag off
                boundary_type = BT_NOT_SEARCHING;

                //  YES:    Were we decoding HTML ?
                if ( content_type == CT_TEXT_HTML  )
                {
                    //  YES:    Insert a '<html>' tag so the decoder will work.
                    list_put_last( level2_list_p, text_copy_to_new( "<html>\0" ) );

                    //  The input data will interfere with HTML decode.
                    list_data_p[ 0 ] = '\0';
                }

                //  YES:    Is this the content type 'multipart/alternative' ?
                if ( strcasestr( list_data_p, "multipart/alternative" ) != NULL )
                {
                    //  YES:    Set the searching for boundary flag.
                    boundary_type = BT_SEARCHING;
                }
            }

            /********************************************************************/
            //  Content-Transfer-Encoding: base64
            /********************************************************************/

            //  Do we have a new encoding type ?
            else
            if (    ( email_flag == true     )
                 && ( tmp_e_type != CTE_NONE ) )
            {
                //  YES:    Set the new encoding type
                encoding_type = tmp_e_type;

                //  Are we decoding HTML ?
                if ( content_type == CT_TEXT_HTML  )
                {
                    //  YES:    The input data will interfere with HTML decode.
                    list_data_p[ 0 ] = '\0';
                }
            }

            /********************************************************************/
            //  -- End of Boundary-Content
            /********************************************************************/

            //  Is this the end of a multipart message ?
            else
            if (    ( email_flag                              == true     )
                 && ( content_type                            != CT_NONE  )
                 && ( encoding_type                           != CTE_NONE )
                 && ( email_is_multipart_break( list_data_p ) == true     ) )
            {
                //  YES:    Were we decoding text/html ?
                if ( content_type == CT_TEXT_HTML  )
                {
                    //  YES:    HACK:   End html
                    list_put_last( level2_list_p, text_copy_to_new( "</html>\0" ) );

                    //  Convert HTML to TEXT
                    decode_html( level2_list_p, source_info_p );
                }
                //  Were we decoding content type text/plain ?
                if (    ( content_type  == CT_TEXT    )
                     && ( encoding_type == CTE_BASE64 ) )
                {
                    //  YES:    Clean up the text.
                    level2_list_p = decode_txt_cleanup( level2_list_p );
                }

                //  Reset the content types.
                content_type   = CT_NONE;
                encoding_type  = CTE_NONE;
                boundary_type  = BT_NOT_SEARCHING;
            }

            /********************************************************************/
            //  Decode BASE64 data
            /********************************************************************/

            //  Is it time to decode base64 data ?
            else
            if ( encoding_type == CTE_BASE64 )
            {
                //  YES:    Is this plain text or html ?
                if (    ( content_type == CT_TEXT )
                     || ( content_type == CT_TEXT_HTML  ) )
                {
                    /**
                     * @param tmp_data_p        Pointer to decoded data         */
                    char                        *   tmp_data_p;

                    //  YES:    Decode the current text string
                    tmp_data_p = base64_decode( list_data_p );

                    //  Do we have a good decode ?
                    if ( tmp_data_p != NULL )
                    {
                        //  YES:    Swap the read data for the decoded data.
                        mem_free( list_data_p );
                        list_data_p = tmp_data_p;
                    }
                }
            }

            /********************************************************************/
            //  Group-From:
            /********************************************************************/

            else
            //  Did we locate e-Mail source data ?
            if (    ( email_flag                                        == true )
                 && ( ( tmp_data_p = email_find_source( list_data_p ) ) != NULL )
                 && ( strlen( source_info_p->g_from )                   ==    0 ) )
            {
                //  YES:    Log what we found
                log_write( MID_DEBUG_0, "decodel1_parse",
                              "Source:    '%.60s'\n", tmp_data_p );

                //  Is there already something here ?
                if ( strlen( source_info_p->g_from ) == 0 )
                {
                    //  NO:     Will the e-Mail source information fit in the buffer ?
                    if ( sizeof( source_info_p->g_from ) > strlen( tmp_data_p ) )
                    {
                        //  YES:    Overwrite the e-Mail 'source' data
                        memset( source_info_p->g_from, '\0',
                                sizeof( source_info_p->g_from ) );
                        strncpy( source_info_p->g_from, tmp_data_p, SOURCE_L );
                    }
                }
            }

            /********************************************************************/
            //  Email-From:
            /********************************************************************/

            //  Did we locate e-Mail from data ?
            else
            if (    ( email_flag                                      == true )
                 && ( ( tmp_data_p = email_find_from( list_data_p ) ) != NULL )
                 && ( strlen( source_info_p->e_from )                 ==    0 ) )
            {
                //  YES:    Log what we found
                log_write( MID_DEBUG_0, "decodel1_parse",
                              "From:      '%.60s'\n", tmp_data_p );

                //  Is there already something here ?
                if ( strlen( source_info_p->e_from ) == 0 )
                {
                    //  NO:     Will the e-Mail from information fit in the buffer ?
                    if ( sizeof( source_info_p->e_from ) > strlen( tmp_data_p ) )
                    {
                        //  YES:    Overwrite the e-Mail 'from' data
                        memset( source_info_p->e_from, '\0',
                                sizeof( source_info_p->e_from ) );
                        strncpy( source_info_p->e_from, tmp_data_p, FROM_L );
                    }
                }
            }

            /********************************************************************/
            //  Group-DateTime:
            /********************************************************************/

            //  Did we locate e-Mail from data ?
            else
            if (    ( email_flag                                           == true )
                 && ( ( tmp_data_p = email_find_datetime( list_data_p ) )  != NULL ) )
            {
                //  YES:    Log what we found
                log_write( MID_DEBUG_0, "decodel1_parse",
                              "DataTime   '%.60s'\n", tmp_data_p );

                //  Is there already something here ?
                if ( strlen( source_info_p->g_datetime ) == 0 )
                {
                    //  NO:     Will the e-Mail date/time information fit in the buffer ?
                    if ( sizeof( source_info_p->g_datetime ) > strlen( tmp_data_p ) )
                    {
                        //  YES:    Write the e-Mail 'group date/time' data
                        memset( source_info_p->g_datetime, '\0',
                                sizeof( source_info_p->g_datetime ) );
                        strncpy( source_info_p->g_datetime, tmp_data_p, DATETIME_L );
                    }
                }
                //  Will the e-Mail date/time information fit in the buffer ?
                if ( sizeof( source_info_p->g_datetime ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Write the e-Mail 'date/time' data
                    memset( source_info_p->e_datetime, '\0',
                            sizeof( source_info_p->e_datetime ) );
                    strncpy( source_info_p->e_datetime, tmp_data_p, DATETIME_L );
                }
            }

            /********************************************************************/
            //  Group-Subject:
            /********************************************************************/

            //  Did we locate the e-Mail subject ?
            else
            if (    ( email_flag                                         == true )
                 && ( ( tmp_data_p = email_find_subject( list_data_p ) ) != NULL ) )
            {
                //  YES:    Log what we found
                log_write( MID_DEBUG_0, "decodel1_parse",
                              "Subject:   '%.60s'\n", tmp_data_p );

                //  Is there something is the group subject field ?
                if ( strlen( source_info_p->g_subject ) == 0 )
                {
                    //  NO:     Will the e-Mail subject information fit in the buffer ?
                    if ( sizeof( source_info_p->g_subject ) > strlen( tmp_data_p ) )
                    {
                        //  YES:    Write the e-Mail group subject data
                        memset( source_info_p->g_subject, '\0',
                                sizeof( source_info_p->g_subject ) );
                        strncpy( source_info_p->g_subject, tmp_data_p, SUBJECT_L );
                    }
                }
                //  Will the e-Mail subject information fit in the buffer ?
                if ( sizeof( source_info_p->e_subject ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Write the e-Mail subject data
                    memset( source_info_p->e_subject, '\0',
                            sizeof( source_info_p->e_subject ) );
                    strncpy( source_info_p->e_subject, tmp_data_p, SUBJECT_L );
                }
            }

            /********************************************************************/
            //  Are we looking for the boundary identifier ?
            if ( boundary_type == BT_NO_IDENTIFIER )
            {
                //  See if this is the boundary identifier.
                boundary_type = email_is_boundary_identifier( list_data_p );
            }

            /********************************************************************/
            //  Are we looking for boundary= ?
            if ( boundary_type == BT_SEARCHING )
            {
                //  We are looking for a multi-part boundary
                boundary_type = email_is_boundary( list_data_p );
            }

            /********************************************************************/
            //  Is this the start of a new e-Mail message ?
            if (    ( email_flag                          == true )
                 && ( email_is_group_break( list_data_p ) == true ) )
            {
                //  YES:    Log what we found
                log_write( MID_DEBUG_0, "decodel1_parse",
                              "GroupBreak'\n" );

                //  Remove HTML tags before passing the list to the next level.
                decode_html( level2_list_p, source_info_p );

                //  Pass the data to level 2 processing
                decodel2_parse( level2_list_p, source_info_p );

                //  Wipe clean the old information
                memset( source_info_p->e_from,     '\0', sizeof( source_info_p->e_from     ) );
                memset( source_info_p->e_datetime, '\0', sizeof( source_info_p->e_datetime ) );
                memset( source_info_p->e_subject,  '\0', sizeof( source_info_p->e_subject  ) );

                if ( list_query_count( level2_list_p ) != 0 )
                {
                    log_write( MID_FATAL, "decodel1_parse",
                               "There is still something on the list\n" );
                }
            }

            /********************************************************************/
            else
            {
                log_write( MID_DEBUG_0, "decodel1_parse",
                              "Else       %p - %4d - '%.60s'\n",
                              list_data_p, list_query_count( level2_list_p ), list_data_p );
            }
            //  Add the line to the level 2 list
            list_put_last( level2_list_p, list_data_p );

#endif
            //  Did we find the start of a recipe ?
            if ( rcb_p->recipe_format == RECIPE_FORMAT_NONE )
            {
                //  NO:     Maybe this is a recipe start.
                rcb_p->recipe_format = recipe_is_start( list_data_p );

                //  Is this the start of a recipe ?
                if ( rcb_p->recipe_format != RECIPE_FORMAT_NONE )
                {
                    //  YES:    Prepare for the new recipe
                    new_rcb_p = rcb_new( rcb_p, rcb_p->recipe_format );
                }
                else
                {
                    //  This is trash data so throw it away
                    mem_free( list_data_p );
                }
            }
            else
            {
                //  YES:    Add this data buffer to the recipe list.
                list_put_last( new_rcb_p->import_list_p, list_data_p );

                //  Is this the end of the recipe
                if ( recipe_is_end( rcb_p->recipe_format, list_data_p ) )
                {
                    //  YES:    Set the packet destination
                    rcb_p->dst_thread = DST_DECODE;

                    //  Put it in one of the ROUTER queue
                    queue_put_payload( router_queue_id, rcb_p  );
                }
            }



            //  @ToDo:  This is only here to avoid compile warnings
            if (    ( email_flag    ==     true )
                 && ( tmp_c_type    ==  CT_NONE )
                 && ( tmp_e_type    == CTE_NONE )
                 && ( content_type  ==  CT_NONE )
                 && ( encoding_type == CTE_NONE ) )
            {
                //  YES:
                tmp_c_type = content_type;
            }

        }

        //  Change execution state to "INITIALIZED" for work.
        tcb_p->thread_state = TS_WAIT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}
/****************************************************************************/