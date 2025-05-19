#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"
#include "ui/ui.h"
#include "ui/ui_helpers.h"
#include "lvgl.h"
#include "user_config.h"

#define REPORT_ID_CONSUMER_CONTROL 3 

#define APP_BUTTON (GPIO_NUM_0)
static const char *TAG = "app_features";

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID report descriptor
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL))
};

/**
 * @brief String descriptor
 */
const char* hid_string_descriptor[5] = {
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "TinyUSB",             // 1: Manufacturer
    "TinyUSB Device",      // 2: Product
    "123456",              // 3: Serials, should use chip ID
    "Example HID interface",  // 4: HID
};

/**
 * @brief Configuration descriptor
 */
static const uint8_t hid_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

/********* TinyUSB HID callbacks ***************/

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    return hid_report_descriptor;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
}

// Volume control parameters
static int volume_level = 50;          // Current volume (0-100%)
static const int VOLUME_STEP = 2;      // Change per knob tick
static bool volume_debounce = false;   // Simple debounce flag

/// NOTE:
/// FOR LATER, IF YOU WANNA ENHANCE DEBOUNCE, TRY TO REPLICATE (OR FIND IF ALREADY THERE)
/// PRESS() FUNCTION, IT LOOKS FOR AN EMPTY SLOT THE KEYBOARD TASK QUEUE,
/// AND THEN ADDS THE KEYCODE TO THE QUEUE. (I THINK)
/// READ CONSUMER::PRESS() IN CONSUMERAPI.HPP (HID-PROJECT.H)
/// 

/////////////////////profile 1/////////////////////
static void send_hid_macro(uint8_t code, uint8_t modifier)
{
    // Prepare keyboard report
    uint8_t keycode[6] = {0}; // Initialize all keys to 0
    
    keycode[0] = code;
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 
                            modifier,
                            keycode);
    
    // Small delay to ensure the key press is registered
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // Release all keys
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);
}

void profile_1_process_knob_event(void *event)
{
    // Get event type
    int event_type = (int)event;
    
    // Process volume control
    if (event_type == 0) { // KNOB_LEFT
        send_hid_macro(code_left, modifier_left);
    } 
    else if (event_type == 1) { // KNOB_RIGHT
        send_hid_macro(code_right, modifier_right);
    }
}

/////////////////////volume control/////////////////////

// Task handle for arc display updates
static TaskHandle_t arc_display_task_handle = NULL;

// Queue handle for volume updates
static QueueHandle_t volume_queue = NULL;

// Timer for auto-hiding the arc
static TimerHandle_t arc_hide_timer = NULL;

// Timer callback to hide the arc
static void arc_hide_timer_callback(TimerHandle_t xTimer) {
    lv_obj_add_flag(ui_Arc1, LV_OBJ_FLAG_HIDDEN);
}

static void arc_display_task(void *pvParameters) {
    int volume_level;
    while(1) {
        if(xQueueReceive(volume_queue, &volume_level, portMAX_DELAY)) {
            // Show arc and update value
            lv_obj_clear_flag(ui_Arc1, LV_OBJ_FLAG_HIDDEN);
            lv_arc_set_value(ui_Arc1, volume_level);
            lv_event_send(ui_Arc1, LV_EVENT_VALUE_CHANGED, 0);

            // Reset the hide timer
            xTimerReset(arc_hide_timer, 0);
        }
    }
}

static void update_arc_display(int volume_level) {
    // Send volume level to the queue
    xQueueSend(volume_queue, &volume_level, 0);
}

static void send_consumer_control(uint16_t usage_code, int volume_level){
    // Send the consumer control code
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &usage_code, 2);
    
    // Queue the arc update
    update_arc_display(volume_level);

    // Small delay to ensure the command is registered
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // Release the control
    uint16_t empty_key = 0;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
}

void volume_control_process_knob_event(void *event)
{
    // Simple debounce protection
    if (volume_debounce) {
        return;
    }
    
    // Get event type
    int event_type = (int)event;
    
    // Process volume control
    if (event_type == 0) { // KNOB_LEFT
        if (volume_level < 100) {
            volume_level += VOLUME_STEP;
            if (volume_level > 100) {
                volume_level = 100;
            }
            ESP_LOGI(TAG, "Volume UP - Level: %d%%", volume_level);
            send_consumer_control(HID_USAGE_CONSUMER_VOLUME_INCREMENT, volume_level);
        }
    } 
    else if (event_type == 1) { // KNOB_RIGHT
        if (volume_level > 0) {
            volume_level -= VOLUME_STEP;
            if (volume_level < 0) {
                volume_level = 0;
            }
            ESP_LOGI(TAG, "Volume DOWN - Level: %d%%", volume_level);
            send_consumer_control(HID_USAGE_CONSUMER_VOLUME_DECREMENT, volume_level);
        }
    }
    else if (event_type == 4) { // KNOB_ZERO
        send_consumer_control(HID_USAGE_CONSUMER_MUTE, volume_level);
        ESP_LOGI(TAG, "Volume MUTE");
    }
    
    // Apply debounce with optimized delay for smooth volume control
    volume_debounce = true;
    vTaskDelay(30 / portTICK_PERIOD_MS);  // 30ms debounce - fast enough for smooth control but prevents bounce
    volume_debounce = false;
}

esp_err_t app_features_init(void)
{
    // Initialize button that will trigger HID reports
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(APP_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    // Create queue for volume updates
    volume_queue = xQueueCreate(5, sizeof(int));
    if (volume_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create volume queue");
        return ESP_FAIL;
    }

    // Create timer for auto-hiding the arc
    arc_hide_timer = xTimerCreate(
        "arc_hide",           // Timer name
        pdMS_TO_TICKS(5000),  // 500ms period
        pdFALSE,             // One-shot timer
        0,                   // Timer ID
        arc_hide_timer_callback  // Callback function
    );
    if (arc_hide_timer == NULL) {
        ESP_LOGE(TAG, "Failed to create arc hide timer");
        return ESP_FAIL;
    }

    // Create task for arc display updates
    BaseType_t ret = xTaskCreate(
        arc_display_task,    // Task function
        "arc_display",       // Task name
        2048,               // Stack size
        NULL,               // Task parameters
        5,                  // Task priority
        &arc_display_task_handle  // Task handle
    );
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create arc display task");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
#if (TUD_OPT_HIGH_SPEED)
        .fs_configuration_descriptor = hid_configuration_descriptor,
        .hs_configuration_descriptor = hid_configuration_descriptor,
        .qualifier_descriptor = NULL,
#else
        .configuration_descriptor = hid_configuration_descriptor,
#endif // TUD_OPT_HIGH_SPEED
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialization DONE");

    return ESP_OK;
}
