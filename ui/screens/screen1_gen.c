/**
 * @file screen1_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen1_gen.h"
#include "../ui.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * screen1_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");

    static lv_style_t scr_bg;

    static bool style_inited = false;

    if (!style_inited) {
        lv_style_init(&scr_bg);
        lv_style_set_bg_color(&scr_bg, lv_color_hex(0x000000));

        style_inited = true;
    }

    lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
    lv_obj_set_name_static(lv_obj_0, "screen1_#");

    lv_obj_add_style(lv_obj_0, &scr_bg, 0);
    lv_obj_t * lbl_prev_profile = lv_label_create(lv_obj_0);
    lv_obj_set_name(lbl_prev_profile, "lbl_prev_profile");
    lv_label_set_text(lbl_prev_profile, "ZOOM");
    lv_obj_set_align(lbl_prev_profile, LV_ALIGN_CENTER);
    lv_obj_set_y(lbl_prev_profile, -90);
    lv_obj_set_style_text_color(lbl_prev_profile, lv_color_hex(0x333333), 0);
    lv_obj_set_style_text_font(lbl_prev_profile, press_start_24, 0);
    
    lv_obj_t * lbl_active_profile = lv_label_create(lv_obj_0);
    lv_obj_set_name(lbl_active_profile, "lbl_active_profile");
    lv_label_set_text(lbl_active_profile, "VOLUME");
    lv_obj_set_align(lbl_active_profile, LV_ALIGN_CENTER);
    lv_obj_set_y(lbl_active_profile, 0);
    lv_obj_set_style_text_color(lbl_active_profile, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_active_profile, press_start_36, 0);
    
    lv_obj_t * lbl_next_profile = lv_label_create(lv_obj_0);
    lv_obj_set_name(lbl_next_profile, "lbl_next_profile");
    lv_label_set_text(lbl_next_profile, "SCROLL");
    lv_obj_set_align(lbl_next_profile, LV_ALIGN_CENTER);
    lv_obj_set_y(lbl_next_profile, 90);
    lv_obj_set_style_text_color(lbl_next_profile, lv_color_hex(0x333333), 0);
    lv_obj_set_style_text_font(lbl_next_profile, press_start_24, 0);
    
    lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_1, 360);
    lv_obj_set_height(lv_obj_1, 2);
    lv_obj_set_align(lv_obj_1, LV_ALIGN_CENTER);
    lv_obj_set_y(lv_obj_1, -50);
    lv_obj_set_style_bg_color(lv_obj_1, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_1, 0, 0);
    
    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_2, 360);
    lv_obj_set_height(lv_obj_2, 2);
    lv_obj_set_align(lv_obj_2, LV_ALIGN_CENTER);
    lv_obj_set_y(lv_obj_2, 50);
    lv_obj_set_style_bg_color(lv_obj_2, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_2, 0, 0);
    
    lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_0, "PRESS TO SELECT");
    lv_obj_set_align(lv_label_0, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(lv_label_0, -40);
    lv_obj_set_style_text_color(lv_label_0, lv_color_hex(0xFFB300), 0);
    lv_obj_set_style_text_font(lv_label_0, press_start_16, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

