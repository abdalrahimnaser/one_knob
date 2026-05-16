/**
 * @file screen3_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "screen3_gen.h"
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

lv_obj_t * screen3_create(void)
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
    lv_obj_set_name_static(lv_obj_0, "screen3_#");

    lv_obj_add_style(lv_obj_0, &scr_bg, 0);
    lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_1, 12);
    lv_obj_set_height(lv_obj_1, 12);
    lv_obj_set_align(lv_obj_1, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_1, -6);
    lv_obj_set_y(lv_obj_1, -216);
    lv_obj_set_style_bg_color(lv_obj_1, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_1, 0, 0);
    lv_obj_set_style_radius(lv_obj_1, 6, 0);
    
    lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_2, 12);
    lv_obj_set_height(lv_obj_2, 12);
    lv_obj_set_align(lv_obj_2, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_2, 38);
    lv_obj_set_y(lv_obj_2, -211);
    lv_obj_set_style_bg_color(lv_obj_2, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_2, 0, 0);
    lv_obj_set_style_radius(lv_obj_2, 6, 0);
    
    lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_3, 12);
    lv_obj_set_height(lv_obj_3, 12);
    lv_obj_set_align(lv_obj_3, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_3, 79);
    lv_obj_set_y(lv_obj_3, -198);
    lv_obj_set_style_bg_color(lv_obj_3, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_3, 0, 0);
    lv_obj_set_style_radius(lv_obj_3, 6, 0);
    
    lv_obj_t * lv_obj_4 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_4, 12);
    lv_obj_set_height(lv_obj_4, 12);
    lv_obj_set_align(lv_obj_4, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_4, 117);
    lv_obj_set_y(lv_obj_4, -176);
    lv_obj_set_style_bg_color(lv_obj_4, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_4, 0, 0);
    lv_obj_set_style_radius(lv_obj_4, 6, 0);
    
    lv_obj_t * lv_obj_5 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_5, 12);
    lv_obj_set_height(lv_obj_5, 12);
    lv_obj_set_align(lv_obj_5, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_5, 150);
    lv_obj_set_y(lv_obj_5, -147);
    lv_obj_set_style_bg_color(lv_obj_5, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_5, 0, 0);
    lv_obj_set_style_radius(lv_obj_5, 6, 0);
    
    lv_obj_t * lv_obj_6 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_6, 12);
    lv_obj_set_height(lv_obj_6, 12);
    lv_obj_set_align(lv_obj_6, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_6, 176);
    lv_obj_set_y(lv_obj_6, -111);
    lv_obj_set_style_bg_color(lv_obj_6, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_6, 0, 0);
    lv_obj_set_style_radius(lv_obj_6, 6, 0);
    
    lv_obj_t * lv_obj_7 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_7, 12);
    lv_obj_set_height(lv_obj_7, 12);
    lv_obj_set_align(lv_obj_7, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_7, 194);
    lv_obj_set_y(lv_obj_7, -71);
    lv_obj_set_style_bg_color(lv_obj_7, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_7, 0, 0);
    lv_obj_set_style_radius(lv_obj_7, 6, 0);
    
    lv_obj_t * lv_obj_8 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_8, 12);
    lv_obj_set_height(lv_obj_8, 12);
    lv_obj_set_align(lv_obj_8, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_8, 203);
    lv_obj_set_y(lv_obj_8, -28);
    lv_obj_set_style_bg_color(lv_obj_8, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_8, 0, 0);
    lv_obj_set_style_radius(lv_obj_8, 6, 0);
    
    lv_obj_t * lv_obj_9 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_9, 12);
    lv_obj_set_height(lv_obj_9, 12);
    lv_obj_set_align(lv_obj_9, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_9, 203);
    lv_obj_set_y(lv_obj_9, 16);
    lv_obj_set_style_bg_color(lv_obj_9, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_9, 0, 0);
    lv_obj_set_style_radius(lv_obj_9, 6, 0);
    
    lv_obj_t * lv_obj_10 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_10, 12);
    lv_obj_set_height(lv_obj_10, 12);
    lv_obj_set_align(lv_obj_10, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_10, 194);
    lv_obj_set_y(lv_obj_10, 59);
    lv_obj_set_style_bg_color(lv_obj_10, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_10, 0, 0);
    lv_obj_set_style_radius(lv_obj_10, 6, 0);
    
    lv_obj_t * lv_obj_11 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_11, 12);
    lv_obj_set_height(lv_obj_11, 12);
    lv_obj_set_align(lv_obj_11, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_11, 176);
    lv_obj_set_y(lv_obj_11, 99);
    lv_obj_set_style_bg_color(lv_obj_11, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_11, 0, 0);
    lv_obj_set_style_radius(lv_obj_11, 6, 0);
    
    lv_obj_t * lv_obj_12 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_12, 12);
    lv_obj_set_height(lv_obj_12, 12);
    lv_obj_set_align(lv_obj_12, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_12, 150);
    lv_obj_set_y(lv_obj_12, 135);
    lv_obj_set_style_bg_color(lv_obj_12, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_12, 0, 0);
    lv_obj_set_style_radius(lv_obj_12, 6, 0);
    
    lv_obj_t * lv_obj_13 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_13, 12);
    lv_obj_set_height(lv_obj_13, 12);
    lv_obj_set_align(lv_obj_13, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_13, 117);
    lv_obj_set_y(lv_obj_13, 164);
    lv_obj_set_style_bg_color(lv_obj_13, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_13, 0, 0);
    lv_obj_set_style_radius(lv_obj_13, 6, 0);
    
    lv_obj_t * lv_obj_14 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_14, 12);
    lv_obj_set_height(lv_obj_14, 12);
    lv_obj_set_align(lv_obj_14, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_14, 79);
    lv_obj_set_y(lv_obj_14, 186);
    lv_obj_set_style_bg_color(lv_obj_14, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_14, 0, 0);
    lv_obj_set_style_radius(lv_obj_14, 6, 0);
    
    lv_obj_t * lv_obj_15 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_15, 12);
    lv_obj_set_height(lv_obj_15, 12);
    lv_obj_set_align(lv_obj_15, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_15, 38);
    lv_obj_set_y(lv_obj_15, 199);
    lv_obj_set_style_bg_color(lv_obj_15, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_15, 0, 0);
    lv_obj_set_style_radius(lv_obj_15, 6, 0);
    
    lv_obj_t * lv_obj_16 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_16, 12);
    lv_obj_set_height(lv_obj_16, 12);
    lv_obj_set_align(lv_obj_16, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_16, -6);
    lv_obj_set_y(lv_obj_16, 204);
    lv_obj_set_style_bg_color(lv_obj_16, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_16, 0, 0);
    lv_obj_set_style_radius(lv_obj_16, 6, 0);
    
    lv_obj_t * lv_obj_17 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_17, 12);
    lv_obj_set_height(lv_obj_17, 12);
    lv_obj_set_align(lv_obj_17, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_17, -50);
    lv_obj_set_y(lv_obj_17, 199);
    lv_obj_set_style_bg_color(lv_obj_17, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_17, 0, 0);
    lv_obj_set_style_radius(lv_obj_17, 6, 0);
    
    lv_obj_t * lv_obj_18 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_18, 12);
    lv_obj_set_height(lv_obj_18, 12);
    lv_obj_set_align(lv_obj_18, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_18, -91);
    lv_obj_set_y(lv_obj_18, 186);
    lv_obj_set_style_bg_color(lv_obj_18, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_18, 0, 0);
    lv_obj_set_style_radius(lv_obj_18, 6, 0);
    
    lv_obj_t * lv_obj_19 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_19, 12);
    lv_obj_set_height(lv_obj_19, 12);
    lv_obj_set_align(lv_obj_19, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_19, -129);
    lv_obj_set_y(lv_obj_19, 164);
    lv_obj_set_style_bg_color(lv_obj_19, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_19, 0, 0);
    lv_obj_set_style_radius(lv_obj_19, 6, 0);
    
    lv_obj_t * lv_obj_20 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_20, 12);
    lv_obj_set_height(lv_obj_20, 12);
    lv_obj_set_align(lv_obj_20, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_20, -162);
    lv_obj_set_y(lv_obj_20, 135);
    lv_obj_set_style_bg_color(lv_obj_20, lv_color_hex(0x00FFD1), 0);
    lv_obj_set_style_border_width(lv_obj_20, 0, 0);
    lv_obj_set_style_radius(lv_obj_20, 6, 0);
    
    lv_obj_t * lv_obj_21 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_21, 12);
    lv_obj_set_height(lv_obj_21, 12);
    lv_obj_set_align(lv_obj_21, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_21, -188);
    lv_obj_set_y(lv_obj_21, 99);
    lv_obj_set_style_bg_color(lv_obj_21, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_21, 0, 0);
    lv_obj_set_style_radius(lv_obj_21, 6, 0);
    
    lv_obj_t * lv_obj_22 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_22, 12);
    lv_obj_set_height(lv_obj_22, 12);
    lv_obj_set_align(lv_obj_22, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_22, -206);
    lv_obj_set_y(lv_obj_22, 59);
    lv_obj_set_style_bg_color(lv_obj_22, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_22, 0, 0);
    lv_obj_set_style_radius(lv_obj_22, 6, 0);
    
    lv_obj_t * lv_obj_23 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_23, 12);
    lv_obj_set_height(lv_obj_23, 12);
    lv_obj_set_align(lv_obj_23, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_23, -215);
    lv_obj_set_y(lv_obj_23, 16);
    lv_obj_set_style_bg_color(lv_obj_23, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_23, 0, 0);
    lv_obj_set_style_radius(lv_obj_23, 6, 0);
    
    lv_obj_t * lv_obj_24 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_24, 12);
    lv_obj_set_height(lv_obj_24, 12);
    lv_obj_set_align(lv_obj_24, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_24, -215);
    lv_obj_set_y(lv_obj_24, -28);
    lv_obj_set_style_bg_color(lv_obj_24, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_24, 0, 0);
    lv_obj_set_style_radius(lv_obj_24, 6, 0);
    
    lv_obj_t * lv_obj_25 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_25, 12);
    lv_obj_set_height(lv_obj_25, 12);
    lv_obj_set_align(lv_obj_25, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_25, -206);
    lv_obj_set_y(lv_obj_25, -71);
    lv_obj_set_style_bg_color(lv_obj_25, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_25, 0, 0);
    lv_obj_set_style_radius(lv_obj_25, 6, 0);
    
    lv_obj_t * lv_obj_26 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_26, 12);
    lv_obj_set_height(lv_obj_26, 12);
    lv_obj_set_align(lv_obj_26, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_26, -188);
    lv_obj_set_y(lv_obj_26, -111);
    lv_obj_set_style_bg_color(lv_obj_26, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_26, 0, 0);
    lv_obj_set_style_radius(lv_obj_26, 6, 0);
    
    lv_obj_t * lv_obj_27 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_27, 12);
    lv_obj_set_height(lv_obj_27, 12);
    lv_obj_set_align(lv_obj_27, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_27, -162);
    lv_obj_set_y(lv_obj_27, -147);
    lv_obj_set_style_bg_color(lv_obj_27, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_27, 0, 0);
    lv_obj_set_style_radius(lv_obj_27, 6, 0);
    
    lv_obj_t * lv_obj_28 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_28, 12);
    lv_obj_set_height(lv_obj_28, 12);
    lv_obj_set_align(lv_obj_28, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_28, -129);
    lv_obj_set_y(lv_obj_28, -176);
    lv_obj_set_style_bg_color(lv_obj_28, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_28, 0, 0);
    lv_obj_set_style_radius(lv_obj_28, 6, 0);
    
    lv_obj_t * lv_obj_29 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_29, 12);
    lv_obj_set_height(lv_obj_29, 12);
    lv_obj_set_align(lv_obj_29, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_29, -91);
    lv_obj_set_y(lv_obj_29, -198);
    lv_obj_set_style_bg_color(lv_obj_29, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_29, 0, 0);
    lv_obj_set_style_radius(lv_obj_29, 6, 0);
    
    lv_obj_t * lv_obj_30 = lv_obj_create(lv_obj_0);
    lv_obj_set_width(lv_obj_30, 12);
    lv_obj_set_height(lv_obj_30, 12);
    lv_obj_set_align(lv_obj_30, LV_ALIGN_CENTER);
    lv_obj_set_x(lv_obj_30, -50);
    lv_obj_set_y(lv_obj_30, -211);
    lv_obj_set_style_bg_color(lv_obj_30, lv_color_hex(0x1d3532), 0);
    lv_obj_set_style_border_width(lv_obj_30, 0, 0);
    lv_obj_set_style_radius(lv_obj_30, 6, 0);
    
    lv_obj_t * lbl_countdown = lv_label_create(lv_obj_0);
    lv_obj_set_name(lbl_countdown, "lbl_countdown");
    lv_label_set_text(lbl_countdown, "25:00");
    lv_obj_set_align(lbl_countdown, LV_ALIGN_CENTER);
    lv_obj_set_y(lbl_countdown, -60);
    lv_obj_set_style_text_color(lbl_countdown, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_text_font(lbl_countdown, press_start_36, 0);
    
    lv_obj_t * dot_1 = lv_obj_create(lv_obj_0);
    lv_obj_set_name(dot_1, "dot_1");
    lv_obj_set_width(dot_1, 14);
    lv_obj_set_height(dot_1, 14);
    lv_obj_set_align(dot_1, LV_ALIGN_CENTER);
    lv_obj_set_x(dot_1, -30);
    lv_obj_set_y(dot_1, 20);
    lv_obj_set_style_bg_color(dot_1, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(dot_1, 0, 0);
    lv_obj_set_style_radius(dot_1, 7, 0);
    
    lv_obj_t * dot_2 = lv_obj_create(lv_obj_0);
    lv_obj_set_name(dot_2, "dot_2");
    lv_obj_set_width(dot_2, 14);
    lv_obj_set_height(dot_2, 14);
    lv_obj_set_align(dot_2, LV_ALIGN_CENTER);
    lv_obj_set_x(dot_2, -10);
    lv_obj_set_y(dot_2, 20);
    lv_obj_set_style_bg_color(dot_2, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(dot_2, 0, 0);
    lv_obj_set_style_radius(dot_2, 7, 0);
    
    lv_obj_t * dot_3 = lv_obj_create(lv_obj_0);
    lv_obj_set_name(dot_3, "dot_3");
    lv_obj_set_width(dot_3, 14);
    lv_obj_set_height(dot_3, 14);
    lv_obj_set_align(dot_3, LV_ALIGN_CENTER);
    lv_obj_set_x(dot_3, 10);
    lv_obj_set_y(dot_3, 20);
    lv_obj_set_style_bg_color(dot_3, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(dot_3, 0, 0);
    lv_obj_set_style_radius(dot_3, 7, 0);
    
    lv_obj_t * dot_4 = lv_obj_create(lv_obj_0);
    lv_obj_set_name(dot_4, "dot_4");
    lv_obj_set_width(dot_4, 14);
    lv_obj_set_height(dot_4, 14);
    lv_obj_set_align(dot_4, LV_ALIGN_CENTER);
    lv_obj_set_x(dot_4, 30);
    lv_obj_set_y(dot_4, 20);
    lv_obj_set_style_bg_color(dot_4, lv_color_hex(0x333333), 0);
    lv_obj_set_style_border_width(dot_4, 0, 0);
    lv_obj_set_style_radius(dot_4, 7, 0);
    
    lv_obj_t * lbl_hint = lv_label_create(lv_obj_0);
    lv_obj_set_name(lbl_hint, "lbl_hint");
    lv_label_set_text(lbl_hint, "PRESS TO START");
    lv_obj_set_align(lbl_hint, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(lbl_hint, -120);
    lv_obj_set_style_text_color(lbl_hint, lv_color_hex(0xFFB300), 0);
    lv_obj_set_style_text_font(lbl_hint, press_start_16, 0);

    LV_TRACE_OBJ_CREATE("finished");

    return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

