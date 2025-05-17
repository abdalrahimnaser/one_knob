#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

#define REPORT_ID_CONSUMER_CONTROL 3 // got it from usb_descriptors.h (it's 3 since first is 1, then each is +1 till you get to 3)

#define APP_BUTTON (GPIO_NUM_0) // Use BOOT signal by default
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
static const int VOLUME_STEP = 5;      // Change per knob tick
static bool volume_debounce = false;   // Simple debounce flag



/// NOTE:
/// FOR LATER, IF YOU WANNA ENHANCE DEBOUNCE, TRY TO REPLICATE (OR FIND IF ALREADY THERE)
/// PRESS() FUNCTION, IT LOOKS FOR AN EMPTY SLOT THE KEYBOARD TASK QUEUE,
/// AND THEN ADDS THE KEYCODE TO THE QUEUE. (I THINK)
/// READ CONSUMER::PRESS() IN CONSUMERAPI.HPP (HID-PROJECT.H)
/// 



static void send_hid_macro(uint8_t code)
{
    // Prepare keyboard report
    uint8_t keycode[6] = {0}; // Initialize all keys to 0
    
    // Send media control key
    keycode[0] = code;
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, keycode);
    
    // Small delay to ensure the key press is registered
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // Release all keys
    tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL);
}

static void send_consumer_control(uint16_t usage_code){

    // Send the consumer control code
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &usage_code, 2);
    
    // Small delay to ensure the command is registered
    vTaskDelay(pdMS_TO_TICKS(50));
    

    // Release the control
    uint16_t empty_key = 0;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);

}

void app_process_knob_event(void *event)
{
    // Only process volume on main screen (screen_id 1)
    extern uint8_t HF_ui_screen_id;
    if (HF_ui_screen_id != 1) {
        return;
    }
    
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
            send_consumer_control(HID_USAGE_CONSUMER_VOLUME_INCREMENT);
        }
    } 
    else if (event_type == 1) { // KNOB_RIGHT
        if (volume_level > 0) {
            volume_level -= VOLUME_STEP;
            if (volume_level < 0) {
                volume_level = 0;
            }
            ESP_LOGI(TAG, "Volume DOWN - Level: %d%%", volume_level);
            send_consumer_control(HID_USAGE_CONSUMER_VOLUME_DECREMENT);

        }
    }
    else if (event_type == 4) { // KNOB_ZERO
        send_consumer_control(HID_USAGE_CONSUMER_MUTE);
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

