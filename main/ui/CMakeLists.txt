SET(SOURCES components/ui_comp_arc2.c
    components/ui_comp_clockgroup.c
    components/ui_comp.c
    screens/ui_Screen1.c
    screens/ui_Screen2.c
    screens/ui_Screen3.c
    screens/ui_Screen4.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_bg_png.c
    images/ui_img_clock_sec_png.c
    images/ui_img_clock_min_png.c
    images/ui_img_clock_hour_png.c
    images/ui_img_bg1_png.c
    images/ui_img_gpu_png.c
    images/ui_img_445945451.c
    fonts/ui_font_H1.c
    fonts/ui_font_Number_big.c
    fonts/ui_font_Number_extra.c
    fonts/ui_font_Subtitle.c
    fonts/ui_font_Title.c)

add_library(ui ${SOURCES})
