/**
 * @file ui.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "ui.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void ui_init(const char * asset_path)
{
    ui_init_gen(asset_path);
    lv_obj_t *screen = screen1_create();
    lv_screen_load(screen);

    /* Add your own custom code here if needed */
}

/**********************
 *   STATIC FUNCTIONS
 **********************/