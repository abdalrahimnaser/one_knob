// #include <stdio.h>
// #include "esp_log.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "tinyusb.h"
// #include "tusb_hid.h"

// static const char *TAG = "usb_hid_example";

// // HID report descriptor using TinyUSB HID Composite Report Descriptor
// // Single Report (no ID) descriptor
// uint8_t const desc_hid_report[] = {
//     TUD_HID_REPORT_DESC_KEYBOARD()
// };

// void send_keyboard_combination(void) {
//     // The first byte is modifier keys (bit 0: LEFT CTRL, bit 1: LEFT SHIFT, etc)
//     // 0x03 represents LEFT CTRL (0x01) + LEFT SHIFT (0x02)
//     uint8_t keycode[6] = {0};
    
//     // Send key press
//     tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0x03, (uint8_t[]){HID_KEY_7, 0, 0, 0, 0, 0});
//     vTaskDelay(pdMS_TO_TICKS(50));
    
//     // Send key release
//     tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
// }

// void app_main(void) {
//     ESP_LOGI(TAG, "USB HID keyboard example");
    
//     // Initialize TinyUSB
//     tinyusb_config_t tusb_cfg = {
//         .descriptor = NULL,
//         .string_descriptor = NULL,
//         .external_phy = false
//     };
//     tinyusb_driver_install(&tusb_cfg);
    
//     // Initialize HID device
//     tud_hid_init();
    
//     ESP_LOGI(TAG, "USB HID keyboard initialized");
//     vTaskDelay(pdMS_TO_TICKS(5000));
    
//     while (1) {
//         ESP_LOGI(TAG, "Sending Ctrl+Shift+7...");
//         send_keyboard_combination();
//         ESP_LOGI(TAG, "Key combination sent!");
//         vTaskDelay(pdMS_TO_TICKS(5000));
//     }
// }
