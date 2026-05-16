/**
 * @file screen2_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen2_gen.h"
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

lv_obj_t * screen2_create(void)
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
    lv_obj_set_name_static(lv_obj_0, "screen2_#");

    lv_obj_add_style(lv_obj_0, &scr_bg, 0);
    lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_1, 460);
    lv_obj_set_height(lv_obj_1, 460);
    lv_obj_set_align(lv_obj_1, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(lv_obj_1, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_color(lv_obj_1, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_1, 3, 0);
    lv_obj_set_style_radius(lv_obj_1, 230, 0);
    
    lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_0, "12");
    lv_obj_set_align(lv_label_0, LV_ALIGN_CENTER);
    lv_obj_set_y(lv_label_0, -180);
    lv_obj_set_style_text_color(lv_label_0, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lv_label_0, press_start_24, 0);
    
    lv_obj_t * lv_label_1 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_1, "3");
    lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_1, 168);
    lv_obj_set_y(lv_label_1, 0);
    lv_obj_set_style_text_color(lv_label_1, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lv_label_1, press_start_24, 0);
    
    lv_obj_t * lv_label_2 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_2, "6");
    lv_obj_set_align(lv_label_2, LV_ALIGN_CENTER);
    lv_obj_set_y(lv_label_2, 160);
    lv_obj_set_style_text_color(lv_label_2, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lv_label_2, press_start_24, 0);
    
    lv_obj_t * lv_label_3 = lv_label_create(lv_obj_0);
    lv_label_set_text(lv_label_3, "9");
    lv_obj_set_align(lv_label_3, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_label_3, -185);
    lv_obj_set_y(lv_label_3, 0);
    lv_obj_set_style_text_color(lv_label_3, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lv_label_3, press_start_24, 0);
    
    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_2, 4);
    lv_obj_set_height(lv_obj_2, 24);
    lv_obj_set_align(lv_obj_2, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_2, 0);
    lv_obj_set_y(lv_obj_2, -210);
    lv_obj_set_style_bg_color(lv_obj_2, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_2, 0, 0);
    
    lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_3, 24);
    lv_obj_set_height(lv_obj_3, 4);
    lv_obj_set_align(lv_obj_3, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_3, 210);
    lv_obj_set_y(lv_obj_3, 0);
    lv_obj_set_style_bg_color(lv_obj_3, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_3, 0, 0);
    
    lv_obj_t * lv_obj_4 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_4, 4);
    lv_obj_set_height(lv_obj_4, 24);
    lv_obj_set_align(lv_obj_4, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_4, 0);
    lv_obj_set_y(lv_obj_4, 210);
    lv_obj_set_style_bg_color(lv_obj_4, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_4, 0, 0);
    
    lv_obj_t * lv_obj_5 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_5, 24);
    lv_obj_set_height(lv_obj_5, 4);
    lv_obj_set_align(lv_obj_5, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_5, -210);
    lv_obj_set_y(lv_obj_5, 0);
    lv_obj_set_style_bg_color(lv_obj_5, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_5, 0, 0);
    
    lv_obj_t * lv_obj_6 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_6, 8);
    lv_obj_set_height(lv_obj_6, 8);
    lv_obj_set_align(lv_obj_6, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_6, 104);
    lv_obj_set_y(lv_obj_6, -182);
    lv_obj_set_style_bg_color(lv_obj_6, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_6, 0, 0);
    lv_obj_set_style_radius(lv_obj_6, 4, 0);
    
    lv_obj_t * lv_obj_7 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_7, 8);
    lv_obj_set_height(lv_obj_7, 8);
    lv_obj_set_align(lv_obj_7, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_7, 182);
    lv_obj_set_y(lv_obj_7, -104);
    lv_obj_set_style_bg_color(lv_obj_7, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_7, 0, 0);
    lv_obj_set_style_radius(lv_obj_7, 4, 0);
    
    lv_obj_t * lv_obj_8 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_8, 8);
    lv_obj_set_height(lv_obj_8, 8);
    lv_obj_set_align(lv_obj_8, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_8, 182);
    lv_obj_set_y(lv_obj_8, 104);
    lv_obj_set_style_bg_color(lv_obj_8, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_8, 0, 0);
    lv_obj_set_style_radius(lv_obj_8, 4, 0);
    
    lv_obj_t * lv_obj_9 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_9, 8);
    lv_obj_set_height(lv_obj_9, 8);
    lv_obj_set_align(lv_obj_9, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_9, 104);
    lv_obj_set_y(lv_obj_9, 182);
    lv_obj_set_style_bg_color(lv_obj_9, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_9, 0, 0);
    lv_obj_set_style_radius(lv_obj_9, 4, 0);
    
    lv_obj_t * lv_obj_10 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_10, 8);
    lv_obj_set_height(lv_obj_10, 8);
    lv_obj_set_align(lv_obj_10, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_10, -104);
    lv_obj_set_y(lv_obj_10, 182);
    lv_obj_set_style_bg_color(lv_obj_10, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_10, 0, 0);
    lv_obj_set_style_radius(lv_obj_10, 4, 0);
    
    lv_obj_t * lv_obj_11 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_11, 8);
    lv_obj_set_height(lv_obj_11, 8);
    lv_obj_set_align(lv_obj_11, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_11, -182);
    lv_obj_set_y(lv_obj_11, 104);
    lv_obj_set_style_bg_color(lv_obj_11, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_11, 0, 0);
    lv_obj_set_style_radius(lv_obj_11, 4, 0);
    
    lv_obj_t * lv_obj_12 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_12, 8);
    lv_obj_set_height(lv_obj_12, 8);
    lv_obj_set_align(lv_obj_12, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_12, -182);
    lv_obj_set_y(lv_obj_12, -104);
    lv_obj_set_style_bg_color(lv_obj_12, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_12, 0, 0);
    lv_obj_set_style_radius(lv_obj_12, 4, 0);
    
    lv_obj_t * lv_obj_13 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_13, 8);
    lv_obj_set_height(lv_obj_13, 8);
    lv_obj_set_align(lv_obj_13, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_13, -104);
    lv_obj_set_y(lv_obj_13, -182);
    lv_obj_set_style_bg_color(lv_obj_13, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(lv_obj_13, 0, 0);
    lv_obj_set_style_radius(lv_obj_13, 4, 0);
    
    lv_obj_t * hand_hour = lv_obj_create(lv_obj_0);
    lv_obj_set_name(hand_hour, "hand_hour");
    lv_obj_set_width(hand_hour, 8);
    lv_obj_set_height(hand_hour, 120);
    lv_obj_set_align(hand_hour, LV_ALIGN_CENTER);
    lv_obj_set_x(hand_hour, 0);
    lv_obj_set_y(hand_hour, -70);
    lv_obj_set_style_bg_color(hand_hour, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(hand_hour, 0, 0);
    lv_obj_set_style_radius(hand_hour, 4, 0);
    
    lv_obj_t * hand_minute = lv_obj_create(lv_obj_0);
    lv_obj_set_name(hand_minute, "hand_minute");
    lv_obj_set_width(hand_minute, 5);
    lv_obj_set_height(hand_minute, 170);
    lv_obj_set_align(hand_minute, LV_ALIGN_CENTER);
    lv_obj_set_x(hand_minute, 0);
    lv_obj_set_y(hand_minute, -95);
    lv_obj_set_style_bg_color(hand_minute, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(hand_minute, 0, 0);
    lv_obj_set_style_radius(hand_minute, 2, 0);
    
    lv_obj_t * hand_second = lv_obj_create(lv_obj_0);
    lv_obj_set_name(hand_second, "hand_second");
    lv_obj_set_width(hand_second, 3);
    lv_obj_set_height(hand_second, 200);
    lv_obj_set_align(hand_second, LV_ALIGN_CENTER);
    lv_obj_set_x(hand_second, 0);
    lv_obj_set_y(hand_second, -110);
    lv_obj_set_style_bg_color(hand_second, lv_color_hex(0xFFB300), 0);
    lv_obj_set_style_border_width(hand_second, 0, 0);
    lv_obj_set_style_radius(hand_second, 0, 0);
    
    lv_obj_t * lv_obj_14 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_14, 20);
    lv_obj_set_height(lv_obj_14, 20);
    lv_obj_set_align(lv_obj_14, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(lv_obj_14, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(lv_obj_14, 0, 0);
    lv_obj_set_style_radius(lv_obj_14, 10, 0);
    
    lv_obj_t * lv_obj_15 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_15, 120);
    lv_obj_set_height(lv_obj_15, 36);
    lv_obj_set_align(lv_obj_15, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_15, 0);
    lv_obj_set_y(lv_obj_15, 60);
    lv_obj_set_style_bg_color(lv_obj_15, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_color(lv_obj_15, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_15, 2, 0);
    lv_obj_set_style_radius(lv_obj_15, 4, 0);
    
    lv_obj_t * lbl_date = lv_label_create(lv_obj_0);
    lv_obj_set_name(lbl_date, "lbl_date");
    lv_label_set_text(lbl_date, "SAT 16");
    lv_obj_set_align(lbl_date, LV_ALIGN_CENTER);
    lv_obj_set_x(lbl_date, 0);
    lv_obj_set_y(lbl_date, 60);
    lv_obj_set_style_text_color(lbl_date, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_text_font(lbl_date, press_start_16, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

