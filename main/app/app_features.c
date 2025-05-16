/**
 * @file app_features.c
 * @brief Implementation of application features and UI interactions
 */

/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

#define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
static const char *TAG = "app_features";

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

/**
 * @brief HID report descriptor
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD))
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
static const int VOLUME_STEP = 5;      // Change per knob tick
static bool volume_debounce = false;   // Simple debounce flag
static uint32_t last_event_time = 0;   // Track last event time
static const uint32_t MIN_EVENT_INTERVAL = 50;  // Minimum time between events (ms)

static void send_hid_macro(uint8_t code)
{
    // Prepare keyboard report
    uint8_t keycode[6] = {0}; // Initialize all keys to 0
    
    // Send code with Ctrl+Shift modifiers
    keycode[0] = code;
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 
                          KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT, 
                          keycode);
    
    // Small delay to ensure the key press is registered
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // Release all keys
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);
}

void app_process_knob_event(void *event)
{
    // Only process volume on main screen (screen_id 1)
    extern uint8_t HF_ui_screen_id;
    if (HF_ui_screen_id != 1) {
        return;
    }
    
    // Get current time
    uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
    
    // Check if enough time has passed since last event
    if (current_time - last_event_time < MIN_EVENT_INTERVAL) {
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
            send_hid_macro(HID_KEY_7);
            ESP_LOGI(TAG, "Volume UP - Level: %d%%", volume_level);
            last_event_time = current_time;
        }
    } 
    else if (event_type == 1) { // KNOB_RIGHT
        if (volume_level > 0) {
            volume_level -= VOLUME_STEP;
            if (volume_level < 0) {
                volume_level = 0;
            }
            send_hid_macro(HID_KEY_8);
            ESP_LOGI(TAG, "Volume DOWN - Level: %d%%", volume_level);
            last_event_time = current_time;
        }
    }
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

/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

// #include <stdlib.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "tinyusb.h"
// #include "class/hid/hid_device.h"
// #include "driver/gpio.h"

// #define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
// static const char *TAG = "example";

// /************* TinyUSB descriptors ****************/

// #define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

// /**
//  * @brief HID report descriptor
//  *
//  * In this example we implement Keyboard + Mouse HID device,
//  * so we must define both report descriptors
//  */
// const uint8_t hid_report_descriptor[] = {
//     TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD)),
//     TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE))
// };

// /**
//  * @brief String descriptor
//  */
// const char* hid_string_descriptor[5] = {
//     // array of pointer to string descriptors
//     (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
//     "TinyUSB",             // 1: Manufacturer
//     "TinyUSB Device",      // 2: Product
//     "123456",              // 3: Serials, should use chip ID
//     "Example HID interface",  // 4: HID
// };

// /**
//  * @brief Configuration descriptor
//  *
//  * This is a simple configuration descriptor that defines 1 configuration and 1 HID interface
//  */
// static const uint8_t hid_configuration_descriptor[] = {
//     // Configuration number, interface count, string index, total length, attribute, power in mA
//     TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

//     // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
//     TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
// };

// /********* TinyUSB HID callbacks ***************/

// // Invoked when received GET HID REPORT DESCRIPTOR request
// // Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
// uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
// {
//     // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
//     return hid_report_descriptor;
// }

// // Invoked when received GET_REPORT control request
// // Application must fill buffer report's content and return its length.
// // Return zero will cause the stack to STALL request
// uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
// {
//     (void) instance;
//     (void) report_id;
//     (void) report_type;
//     (void) buffer;
//     (void) reqlen;

//     return 0;
// }

// // Invoked when received SET_REPORT control request or
// // received data on OUT endpoint ( Report ID = 0, Type = 0 )
// void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
// {
// }

// /********* Application ***************/

// typedef enum {
//     MOUSE_DIR_RIGHT,
//     MOUSE_DIR_DOWN,
//     MOUSE_DIR_LEFT,
//     MOUSE_DIR_UP,
//     MOUSE_DIR_MAX,
// } mouse_dir_t;

// #define DISTANCE_MAX        125
// #define DELTA_SCALAR        5

// static void mouse_draw_square_next_delta(int8_t *delta_x_ret, int8_t *delta_y_ret)
// {
//     static mouse_dir_t cur_dir = MOUSE_DIR_RIGHT;
//     static uint32_t distance = 0;

//     // Calculate next delta
//     if (cur_dir == MOUSE_DIR_RIGHT) {
//         *delta_x_ret = DELTA_SCALAR;
//         *delta_y_ret = 0;
//     } else if (cur_dir == MOUSE_DIR_DOWN) {
//         *delta_x_ret = 0;
//         *delta_y_ret = DELTA_SCALAR;
//     } else if (cur_dir == MOUSE_DIR_LEFT) {
//         *delta_x_ret = -DELTA_SCALAR;
//         *delta_y_ret = 0;
//     } else if (cur_dir == MOUSE_DIR_UP) {
//         *delta_x_ret = 0;
//         *delta_y_ret = -DELTA_SCALAR;
//     }

//     // Update cumulative distance for current direction
//     distance += DELTA_SCALAR;
//     // Check if we need to change direction
//     if (distance >= DISTANCE_MAX) {
//         distance = 0;
//         cur_dir++;
//         if (cur_dir == MOUSE_DIR_MAX) {
//             cur_dir = 0;
//         }
//     }
// }

// static void app_send_hid_demo(void)
// {
//     // Get the knob direction (you'll need to implement this based on your hardware)
//     bool knob_turned_right = gpio_get_level(APP_BUTTON); // Replace this with your actual knob detection

//     // Prepare keyboard report
//     uint8_t keycode[6] = {0}; // Initialize all keys to 0
    
//     if (knob_turned_right) {
//         // Send Ctrl+Shift+7
//         keycode[0] = HID_KEY_7;
//         tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT, keycode);
//     } else {
//         // Send Ctrl+Shift+8
//         keycode[0] = HID_KEY_8;
//         tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT, keycode);
//     }
    
//     // Small delay to ensure the key press is registered
//     vTaskDelay(pdMS_TO_TICKS(50));
    
//     // Release all keys
//     tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);

//     // // Mouse output: Move mouse cursor in square trajectory
//     // ESP_LOGI(TAG, "Sending Mouse report");
//     // int8_t delta_x = 0;
//     // int8_t delta_y = 0;
//     // for (int i = 0; i < (DISTANCE_MAX / DELTA_SCALAR) * 4; i++) {
//     //     // Get the next x and y delta in the draw square pattern
//     //     mouse_draw_square_next_delta(&delta_x, &delta_y);
//     //     tud_hid_mouse_report(HID_ITF_PROTOCOL_MOUSE, 0x00, delta_x, delta_y, 0, 0);
//     //     vTaskDelay(pdMS_TO_TICKS(20));
//     // }
// }

// void app_features_init(void)
// {
//     // Initialize button that will trigger HID reports
//     const gpio_config_t boot_button_config = {
//         .pin_bit_mask = BIT64(APP_BUTTON),
//         .mode = GPIO_MODE_INPUT,
//         .intr_type = GPIO_INTR_DISABLE,
//         .pull_up_en = true,
//         .pull_down_en = false,
//     };
//     ESP_ERROR_CHECK(gpio_config(&boot_button_config));

//     ESP_LOGI(TAG, "USB initialization");
//     const tinyusb_config_t tusb_cfg = {
//         .device_descriptor = NULL,
//         .string_descriptor = hid_string_descriptor,
//         .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
//         .external_phy = false,
// #if (TUD_OPT_HIGH_SPEED)
//         .fs_configuration_descriptor = hid_configuration_descriptor,
//         .hs_configuration_descriptor = hid_configuration_descriptor,
//         .qualifier_descriptor = NULL,
// #else
//         .configuration_descriptor = hid_configuration_descriptor,
// #endif // TUD_OPT_HIGH_SPEED
//     };

//     ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
//     ESP_LOGI(TAG, "USB initialization DONE");

//     while (1) {
//         if (tud_mounted()) {
//             static bool last_button_state = false;
//             bool current_button_state = gpio_get_level(APP_BUTTON);
            
//             // Only send command when button state changes
//             if (current_button_state != last_button_state) {
//                 app_send_hid_demo();
//                 last_button_state = current_button_state;
//             }
//         }
//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }