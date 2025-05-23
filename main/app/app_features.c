#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "class/cdc/cdc_device.h"
#include "driver/gpio.h"
#include "ui/ui.h"
#include "ui/ui_helpers.h"
#include "lvgl.h"
#include "user_config.h"
#include "tusb_cdc_acm.h"
#include "nvs_flash.h"
#include "ui_dynamic.h"
////////////////////
static const char *TAG = "app_features";
///////////////////
/// helper print function
void cdc_print(const char* str) {
    tud_cdc_write(str, strlen(str));
    tud_cdc_write_flush();
}

////////////////////////////

#include "esp_partition.h"
#include "esp_flash.h"

#define IMAGE_FLASH_ADDRESS 0x311000
#define MAX_IMAGE_SIZE (466*466*2 + 8)  // Image size plus header

// Static buffer for image data
static uint8_t *image_data = NULL;
static lv_img_dsc_t custom_image = {
    .header.always_zero = 0,
    .header.w = 0,
    .header.h = 0,
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .data_size = 0,
    .data = NULL
};

bool load_custom_image_from_flash(void) {
    // Find the custom_images partition
    const esp_partition_t *image_partition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, "custom_images");
    
    if (!image_partition) {
        ESP_LOGE(TAG, "Failed to find custom_images partition");
        cdc_print("Failed to find custom_images partition");
        return false;
    }

    // Allocate memory for image if not already done
    if (image_data == NULL) {
        image_data = heap_caps_malloc(MAX_IMAGE_SIZE, MALLOC_CAP_8BIT);
        if (image_data == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for image");
            cdc_print("Failed to allocate memory for image");
            return false;
        }
    }
    
    // Read image header first (8 bytes)
    esp_err_t err = esp_partition_read(image_partition, 0, image_data, 8);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read image header from flash: %d", err);
        cdc_print("Failed to read image header from flash");
        return false;
    }
    
    // Parse header
    uint16_t width = *(uint16_t*)image_data;
    uint16_t height = *(uint16_t*)(image_data + 2);
    uint8_t format = *(uint8_t*)(image_data + 4);
    
    // Validate header
    if (width == 0 || width > 466 || height == 0 || height > 466 || format != LV_IMG_CF_TRUE_COLOR) {
        ESP_LOGE(TAG, "Invalid image header: %dx%d, format %d", width, height, format);
        cdc_print("Invalid image header");
        return false;
    }
    
    // Calculate data size
    size_t data_size = width * height * 2;  // 16-bit color
    
    // Read full image
    err = esp_partition_read(image_partition, 8, image_data + 8, data_size);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read image data from flash: %d", err);
        cdc_print("Failed to read image data from flash");
        return false;
    }
    
    // Update image descriptor
    custom_image.header.w = width;
    custom_image.header.h = height;
    custom_image.header.cf = format;
    custom_image.data_size = data_size;
    custom_image.data = image_data + 8;  // Skip header
    
    ESP_LOGI(TAG, "Custom image loaded from flash: %dx%d", width, height);
    cdc_print("Custom image loaded from flash");
    return true;
}

void apply_custom_image(void) {
    ESP_LOGI(TAG, "Attempting to apply custom image to UI");
    
    // Check if we have valid image data
    if (custom_image.data == NULL || custom_image.data_size == 0) {
        ESP_LOGW(TAG, "No valid image data available to apply");
        return;
    }
    
    // Log image details
    ESP_LOGI(TAG, "Image details: %dx%d pixels, format %d, %d bytes", 
             custom_image.header.w, custom_image.header.h, 
             custom_image.header.cf, custom_image.data_size);
    
    // Check if UI element exists
    if (ui_Image2 == NULL) {
        ESP_LOGE(TAG, "UI Image element (ui_Image2) not found!");
        return;
    }
    
    // Store the previous source to check if update is needed
    const void *prev_src = lv_img_get_src(ui_Image2);
    if (prev_src == &custom_image) {
        ESP_LOGI(TAG, "Image already set to custom image, no update needed");
        return;
    }
    
    // Apply the image to the UI element
    lv_img_set_src(ui_Image2, &custom_image);
    
    // Force a redraw of the image object
    lv_obj_invalidate(ui_Image2);
    
    // Make sure the parent screen is visible
    lv_obj_t *parent_screen = lv_obj_get_parent(ui_Image2);
    if (parent_screen != NULL && parent_screen != lv_scr_act()) {
        ESP_LOGI(TAG, "Switching to screen containing the image");
        lv_scr_load(parent_screen);
    }
    
    // Force a full screen update
    lv_refr_now(NULL);
    
    // Verify the image was applied correctly
    const void *current_src = lv_img_get_src(ui_Image2);
    if (current_src == &custom_image) {
        ESP_LOGI(TAG, "Custom image successfully applied to UI");
    } else {
        ESP_LOGW(TAG, "Failed to apply custom image - source mismatch");
    }
    
    // If using a display driver with a flush callback, you might need to call it manually
    // display_flush_pending();
}




////////////////////////////




#define REPORT_ID_CONSUMER_CONTROL 3 



enum
{
  ITF_NUM_HID,
  ITF_NUM_CDC,
};

#define ITF_NUM_TOTAL 3
#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN + TUD_CDC_DESC_LEN)
#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT 0x02
#define EPNUM_CDC_IN 0x82
#define EPNUM_HID 0x83




#define APP_BUTTON (GPIO_NUM_0)

/************* TinyUSB descriptors ****************/

#define TUSB_DESC_TOTAL_LEN      (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN + CFG_TUD_CDC * TUD_CDC_DESC_LEN)

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
const char* hid_string_descriptor[6] = {
    (char[]){0x09, 0x04},  // 0: is supported language is English (0x0409)
    "TinyUSB",             // 1: Manufacturer
    "TinyUSB Device",      // 2: Product
    "123456",              // 3: Serials, should use chip ID
    "CDC Interface",       // 4: CDC
    "Example HID interface",  // 5: HID

};

/**
 * @brief Configuration descriptor
 */
static const uint8_t hid_configuration_descriptor[] = {
    // Configuration number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 200),

    // Interface number, string index, boot protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 5, HID_ITF_PROTOCOL_NONE, sizeof(hid_report_descriptor), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 10),

    // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
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

/********* TinyUSB CDC callbacks ***************/

static uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];


void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    size_t rx_size = 0;
    esp_err_t ret = tinyusb_cdcacm_read(itf, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);
    if (ret == ESP_OK)
    {
        ESP_LOGD(TAG, "Data from channel %d:", itf);
        ESP_LOG_BUFFER_HEXDUMP(TAG, buf, rx_size, ESP_LOG_DEBUG);
        
        // Null terminate the received data
        buf[rx_size] = '\0';
        
        // Process the input as usage data
        process_usage_input((char*)buf);
    }
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
    _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_NONE, 500, 0, &ui_Screen1_screen_init);
}

static void arc_display_task(void *pvParameters) {
    int volume_level;
    while(1) {
        if(xQueueReceive(volume_queue, &volume_level, portMAX_DELAY)) {
            //show screen4
            _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_NONE, 500, 0, &ui_Screen4_screen_init);

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
            cdc_print("Volume DOWN");
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
    // Load user configuration first
    load_user_config();

    // Initialize button that will trigger HID reports
    const gpio_config_t boot_button_config = {
        .pin_bit_mask = BIT64(APP_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_up_en = true,
        .pull_down_en = false,
    };
    ESP_ERROR_CHECK(gpio_config(&boot_button_config));

    // Initialize CPU usage display
    init_usage_display();
    
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

    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &tinyusb_cdc_rx_callback,
        .callback_rx_wanted_char = NULL,
    };

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));


        // Load custom image from flash
    if (load_custom_image_from_flash()) {
        apply_custom_image();
    }
    


    return ESP_OK;
}


