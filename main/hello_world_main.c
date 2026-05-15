#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/spi_master.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lcd_touch_cst816s.h"

#include "lvgl.h"

#define CONFIG_EXAMPLE_LCD_CONTROLLER_SH8601 1
#include "esp_lcd_sh8601.h"

#include "test_lvgl_editor.h"
#include "screens/screen1_gen.h"

static const char *TAG = "example";
static SemaphoreHandle_t lvgl_mux = NULL;

#define EXAMPLE_LCD_HOST                (SPI2_HOST)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL   1
#define EXAMPLE_PIN_NUM_LCD_CS          (GPIO_NUM_12)
#define EXAMPLE_PIN_NUM_LCD_PCLK        (GPIO_NUM_10)
#define EXAMPLE_PIN_NUM_LCD_DATA0       (GPIO_NUM_13)
#define EXAMPLE_PIN_NUM_LCD_DATA1       (GPIO_NUM_11)
#define EXAMPLE_PIN_NUM_LCD_DATA2       (GPIO_NUM_14)
#define EXAMPLE_PIN_NUM_LCD_DATA3       (GPIO_NUM_9)
#define EXAMPLE_PIN_NUM_LCD_RST         (GPIO_NUM_8)
#define EXAMPLE_PIN_NUM_BK_LIGHT        (GPIO_NUM_17)

#define EXAMPLE_LCD_H_RES   471
#define EXAMPLE_LCD_V_RES   466

#if CONFIG_LV_COLOR_DEPTH == 32
#define LCD_BIT_PER_PIXEL (24)
#elif CONFIG_LV_COLOR_DEPTH == 16
#define LCD_BIT_PER_PIXEL (16)
#endif

#define EXAMPLE_TOUCH_HOST          (I2C_NUM_0)
#define EXAMPLE_PIN_NUM_TOUCH_SCL   (GPIO_NUM_3)
#define EXAMPLE_PIN_NUM_TOUCH_SDA   (GPIO_NUM_1)
#define EXAMPLE_PIN_NUM_TOUCH_RST   (GPIO_NUM_2)
#define EXAMPLE_PIN_NUM_TOUCH_INT   (GPIO_NUM_4)
esp_lcd_touch_handle_t tp = NULL;

#define EXAMPLE_LVGL_BUFF_SIZE          (EXAMPLE_LCD_H_RES * 20)
#define EXAMPLE_LVGL_TICK_PERIOD_MS     2
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS  500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS  2
#define EXAMPLE_LVGL_TASK_STACK_SIZE    (6 * 1024)
#define EXAMPLE_LVGL_TASK_PRIORITY      2

static const sh8601_lcd_init_cmd_t lcd_init_cmds[] = {
    {0xFE, (uint8_t []){0x00}, 0, 0},
    {0xC4, (uint8_t []){0x80}, 1, 0},
    {0x3A, (uint8_t []){0x55}, 1, 0},
    {0x35, (uint8_t []){0x00}, 0, 10},
    {0x53, (uint8_t []){0x20}, 1, 10},
    {0x51, (uint8_t []){0xFF}, 1, 10},
    {0x63, (uint8_t []){0xFF}, 1, 10},
    {0x2A, (uint8_t []){0x00,0x06,0x01,0xDD}, 4, 0},
    {0x2B, (uint8_t []){0x00,0x00,0x01,0xD1}, 4, 0},
    {0x11, (uint8_t []){0x00}, 0, 60},
    {0x29, (uint8_t []){0x00}, 0, 0},
};

// ---- FLUSH CALLBACK ----
static void example_lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    ESP_LOGD(TAG, "Flush: x1=%d y1=%d x2=%d y2=%d", area->x1, area->y1, area->x2, area->y2);
    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
    lv_display_flush_ready(disp);
}

// ---- TOUCH ISR + CALLBACK ----
static SemaphoreHandle_t touch_mux = NULL;

static void example_touch_isr_cb(esp_lcd_touch_handle_t tp)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(touch_mux, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) portYIELD_FROM_ISR();
}

static void example_lvgl_touch_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    esp_lcd_touch_handle_t tp = lv_indev_get_user_data(indev);
    assert(tp);
    uint16_t tp_x, tp_y;
    uint8_t tp_cnt = 0;
    if (xSemaphoreTake(touch_mux, 0) == pdTRUE) {
        esp_lcd_touch_read_data(tp);
    }
    bool tp_pressed = esp_lcd_touch_get_coordinates(tp, &tp_x, &tp_y, NULL, &tp_cnt, 1);
    if (tp_pressed && tp_cnt > 0) {
        data->point.x = tp_x;
        data->point.y = tp_y;
        data->state = LV_INDEV_STATE_PRESSED;
        ESP_LOGI(TAG, "Touch PRESSED at x=%d y=%d", tp_x, tp_y);
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// ---- LVGL TICK + TASK ----
static void example_increase_lvgl_tick(void *arg)
{
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

static bool example_lvgl_lock(int timeout_ms)
{
    assert(lvgl_mux);
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTake(lvgl_mux, timeout_ticks) == pdTRUE;
}

static void example_lvgl_unlock(void)
{
    assert(lvgl_mux);
    xSemaphoreGive(lvgl_mux);
}

static void example_lvgl_port_task(void *arg)
{
    ESP_LOGI(TAG, "LVGL task started on core %d", xPortGetCoreID());
    uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    while (1) {
        if (example_lvgl_lock(-1)) {
            task_delay_ms = lv_timer_handler();
            example_lvgl_unlock();
        }
        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS) task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS) task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

// ---- APP MAIN ----
void app_main(void)
{
    ESP_LOGI(TAG, "=== app_main start ===");

    // ---- BACKLIGHT ----
    ESP_LOGI(TAG, "[1/10] Configuring backlight GPIO %d", EXAMPLE_PIN_NUM_BK_LIGHT);
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    ESP_LOGI(TAG, "[1/10] Backlight GPIO configured OK");

    // ---- SPI BUS ----
    ESP_LOGI(TAG, "[2/10] Initializing SPI bus on host %d", EXAMPLE_LCD_HOST);
    const spi_bus_config_t buscfg = SH8601_PANEL_BUS_QSPI_CONFIG(
        EXAMPLE_PIN_NUM_LCD_PCLK,
        EXAMPLE_PIN_NUM_LCD_DATA0, EXAMPLE_PIN_NUM_LCD_DATA1,
        EXAMPLE_PIN_NUM_LCD_DATA2, EXAMPLE_PIN_NUM_LCD_DATA3,
        EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * LCD_BIT_PER_PIXEL / 8);
    ESP_ERROR_CHECK(spi_bus_initialize(EXAMPLE_LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_LOGI(TAG, "[2/10] SPI bus initialized OK");

    // ---- PANEL IO ----
    ESP_LOGI(TAG, "[3/10] Installing panel IO (QSPI, CS=GPIO%d)", EXAMPLE_PIN_NUM_LCD_CS);
    esp_lcd_panel_io_handle_t io_handle = NULL;
    sh8601_vendor_config_t vendor_config = {
        .init_cmds = lcd_init_cmds,
        .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(sh8601_lcd_init_cmd_t),
        .flags = { .use_qspi_interface = 1 },
    };
    const esp_lcd_panel_io_spi_config_t io_config = SH8601_PANEL_IO_QSPI_CONFIG(
        EXAMPLE_PIN_NUM_LCD_CS, NULL, NULL);
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_HOST, &io_config, &io_handle));
    ESP_LOGI(TAG, "[3/10] Panel IO installed OK");

    // ---- LCD PANEL ----
    ESP_LOGI(TAG, "[4/10] Installing SH8601 LCD driver (RST=GPIO%d, %dbpp)", EXAMPLE_PIN_NUM_LCD_RST, LCD_BIT_PER_PIXEL);
    esp_lcd_panel_handle_t panel_handle = NULL;
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = LCD_BIT_PER_PIXEL,
        .vendor_config = &vendor_config,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_sh8601(io_handle, &panel_config, &panel_handle));
    ESP_LOGI(TAG, "[4/10] SH8601 panel created OK");

    ESP_LOGI(TAG, "[4/10] Resetting panel...");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_LOGI(TAG, "[4/10] Panel reset OK");

    ESP_LOGI(TAG, "[4/10] Initializing panel...");
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_LOGI(TAG, "[4/10] Panel init OK");

    ESP_LOGI(TAG, "[4/10] Turning display on...");
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    ESP_LOGI(TAG, "[4/10] Display on OK");

    // ---- I2C BUS (new master API) ----
    ESP_LOGI(TAG, "[5/10] Initializing I2C master bus (SDA=GPIO%d, SCL=GPIO%d)",
             EXAMPLE_PIN_NUM_TOUCH_SDA, EXAMPLE_PIN_NUM_TOUCH_SCL);
    i2c_master_bus_config_t i2c_bus_cfg = {
        .i2c_port = EXAMPLE_TOUCH_HOST,
        .sda_io_num = EXAMPLE_PIN_NUM_TOUCH_SDA,
        .scl_io_num = EXAMPLE_PIN_NUM_TOUCH_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t i2c_bus_handle = NULL;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_cfg, &i2c_bus_handle));
    ESP_LOGI(TAG, "[5/10] I2C master bus created OK");

    // ---- TOUCH IO ----
    ESP_LOGI(TAG, "[6/10] Attaching CST816S touch IO to I2C bus");
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_handle, &tp_io_config, &tp_io_handle));
    ESP_LOGI(TAG, "[6/10] Touch IO attached OK");

    touch_mux = xSemaphoreCreateBinary();
    assert(touch_mux);
    ESP_LOGI(TAG, "[6/10] Touch semaphore created OK");

    // ---- TOUCH CONTROLLER ----
    ESP_LOGI(TAG, "[7/10] Initializing CST816S (RST=GPIO%d, INT=GPIO%d)",
             EXAMPLE_PIN_NUM_TOUCH_RST, EXAMPLE_PIN_NUM_TOUCH_INT);
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = EXAMPLE_PIN_NUM_TOUCH_RST,
        .int_gpio_num = EXAMPLE_PIN_NUM_TOUCH_INT,
        .levels = { .reset = 0, .interrupt = 0 },
        .flags = { .swap_xy = 0, .mirror_x = 0, .mirror_y = 0 },
        .interrupt_callback = example_touch_isr_cb,
    };
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, &tp));
    ESP_LOGI(TAG, "[7/10] CST816S touch controller initialized OK");

    // ---- BACKLIGHT ON ----
    ESP_LOGI(TAG, "[7/10] Turning on backlight");
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);

    // ---- LVGL INIT ----
    ESP_LOGI(TAG, "[8/10] Initializing LVGL");
    lv_init();
    ESP_LOGI(TAG, "[8/10] LVGL lv_init() OK");

    ESP_LOGI(TAG, "[8/10] Creating LVGL display (%dx%d)", EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES);
    lv_display_t *disp = lv_display_create(EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES);
    lv_display_set_user_data(disp, panel_handle);
    lv_display_set_flush_cb(disp, example_lvgl_flush_cb);
    ESP_LOGI(TAG, "[8/10] LVGL display created OK");

    ESP_LOGI(TAG, "[8/10] Allocating draw buffers (%d pixels each)", EXAMPLE_LVGL_BUFF_SIZE);
    void *buf1 = heap_caps_malloc(EXAMPLE_LVGL_BUFF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    void *buf2 = heap_caps_malloc(EXAMPLE_LVGL_BUFF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    if (!buf1 || !buf2) {
        ESP_LOGE(TAG, "FATAL: Failed to allocate draw buffers! Free heap: %lu bytes",
                 (unsigned long)heap_caps_get_free_size(MALLOC_CAP_DMA));
        assert(false);
    }
    ESP_LOGI(TAG, "[8/10] Draw buffers allocated OK (buf1=%p, buf2=%p)", buf1, buf2);
    lv_display_set_buffers(disp, buf1, buf2, EXAMPLE_LVGL_BUFF_SIZE * sizeof(lv_color_t),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
    ESP_LOGI(TAG, "[8/10] LVGL buffers set OK");

    // ---- TOUCH INPUT DEVICE ----
    ESP_LOGI(TAG, "[9/10] Registering touch input device with LVGL");
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, example_lvgl_touch_cb);
    lv_indev_set_user_data(indev, tp);
    ESP_LOGI(TAG, "[9/10] Touch input device registered OK");

    // ---- LVGL TICK TIMER ----
    ESP_LOGI(TAG, "[9/10] Starting LVGL tick timer (%dms period)", EXAMPLE_LVGL_TICK_PERIOD_MS);
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));
    ESP_LOGI(TAG, "[9/10] LVGL tick timer started OK");

    // ---- LVGL TASK ----
    ESP_LOGI(TAG, "[9/10] Creating LVGL task (stack=%d, priority=%d)",
             EXAMPLE_LVGL_TASK_STACK_SIZE, EXAMPLE_LVGL_TASK_PRIORITY);
    lvgl_mux = xSemaphoreCreateMutex();
    assert(lvgl_mux);
    BaseType_t ret = xTaskCreate(example_lvgl_port_task, "LVGL",
                                  EXAMPLE_LVGL_TASK_STACK_SIZE, NULL,
                                  EXAMPLE_LVGL_TASK_PRIORITY, NULL);
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "FATAL: Failed to create LVGL task! ret=%d", ret);
        assert(false);
    }
    ESP_LOGI(TAG, "[9/10] LVGL task created OK");

    // ---- LOAD UI ----
    ESP_LOGI(TAG, "[10/10] Acquiring LVGL lock to load UI...");
    if (example_lvgl_lock(-1)) {
        ESP_LOGI(TAG, "[10/10] Lock acquired, calling test_lvgl_editor_init()...");
        test_lvgl_editor_init(NULL);
        ESP_LOGI(TAG, "[10/10] test_lvgl_editor_init() done, calling screen1_create()...");
        lv_obj_t *screen = screen1_create();
        ESP_LOGI(TAG, "[10/10] screen1_create() returned %p, loading screen...", screen);
        lv_screen_load(screen);
        ESP_LOGI(TAG, "[10/10] Screen loaded OK");
        example_lvgl_unlock();
        ESP_LOGI(TAG, "[10/10] LVGL lock released");
    } else {
        ESP_LOGE(TAG, "FATAL: Could not acquire LVGL lock!");
    }

    ESP_LOGI(TAG, "=== app_main complete — UI should be visible ===");
}