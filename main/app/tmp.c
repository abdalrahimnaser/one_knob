// /*
//  * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
//  *
//  * SPDX-License-Identifier: Unlicense OR CC0-1.0
//  */

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
//  */
// const uint8_t hid_report_descriptor[] = {
//     TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD))
// };

// /**
//  * @brief String descriptor
//  */
// const char* hid_string_descriptor[5] = {
//     (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
//     "TinyUSB",             // 1: Manufacturer
//     "TinyUSB Device",      // 2: Product
//     "123456",              // 3: Serials, should use chip ID
//     "Example HID interface",  // 4: HID
// };

// /**
//  * @brief Configuration descriptor
//  */
// static const uint8_t hid_configuration_descriptor[] = {
//     TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
//     TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
// };

// /********* TinyUSB HID callbacks ***************/

// uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
// {
//     return hid_report_descriptor;
// }

// uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
// {
//     (void) instance;
//     (void) report_id;
//     (void) report_type;
//     (void) buffer;
//     (void) reqlen;
//     return 0;
// }

// void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
// {
// }

// static void app_send_hid_demo(void)
// {
//     // Prepare keyboard report
//     uint8_t keycode[6] = {0}; // Initialize all keys to 0
    
//     // Send HID_KEY_7
//     keycode[0] = HID_KEY_7;
//     tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, keycode);
    
//     // Small delay to ensure the key press is registered
//     vTaskDelay(pdMS_TO_TICKS(50));
    
//     // Release all keys
//     tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);
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