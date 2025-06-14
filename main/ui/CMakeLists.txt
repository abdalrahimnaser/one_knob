SET(SOURCES components/ui_comp_roller_styled.c
    components/ui_comp.c
    screens/ui_Screen1.c
    screens/ui_Screen2.c
    screens/ui_Screen3.c
    screens/ui_Screen4.c
    screens/ui_Screen5.c
    screens/ui_Screen6.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_bh2_png.c
    images/ui_img_clock_sec_png.c
    images/ui_img_clock_min_png.c
    images/ui_img_clock_hour_png.c
    images/ui_img_bg1_png.c
    images/ui_img_gpu_png.c
    images/ui_img_445945451.c
    images/ui_img_gif_frames_frame_00_png.c
    images/ui_img_gif_frames_frame_08_png.c
    images/ui_img_gif_frames_frame_01_png.c
    images/ui_img_gif_frames_frame_02_png.c
    images/ui_img_gif_frames_frame_03_png.c
    images/ui_img_gif_frames_frame_04_png.c
    images/ui_img_gif_frames_frame_05_png.c
    images/ui_img_gif_frames_frame_06_png.c
    images/ui_img_gif_frames_frame_07_png.c
    images/ui_img_gif_frames_frame_09_png.c
    images/ui_img_gif_frames_frame_10_png.c
    images/ui_img_gif_frames_frame_11_png.c
    images/ui_img_gif_frames_frame_12_png.c
    fonts/ui_font_H1.c
    fonts/ui_font_Number_big.c
    fonts/ui_font_Number_extra.c
    fonts/ui_font_Subtitle.c
    fonts/ui_font_Title.c)

add_library(ui ${SOURCES})
