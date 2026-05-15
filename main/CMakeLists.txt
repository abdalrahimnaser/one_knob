idf_component_register(
    SRCS "hello_world_main.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES spi_flash
    REQUIRES
        lvgl
        esp_timer
        esp_lcd_touch
        driver
        esp_lcd_sh8601
        idf_wrapper_component
)

