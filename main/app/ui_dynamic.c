#include <time.h>
#include "esp_system.h"
#include "esp_log.h"
#include "ui/ui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <string.h>
#include "vars.h"

static const char *TAG = "UI_DYNAMIC";

// Queue for usage updates
static QueueHandle_t usage_queue = NULL;

// Structure to hold usage data - reduced size for efficiency
typedef struct {
    char cpu_usage[16];  // Reduced from 32 to 16 bytes
    char gpu_usage[16];  // Reduced from 32 to 16 bytes
} usage_data_t;

// Function to update clock hands based on current time
void update_clock_hands(void) {
    time_t now;
    struct tm timeinfo;
    
    // Get current time
    time(&now);
    localtime_r(&now, &timeinfo);
    
    // Calculate angles for each hand
    // Seconds: 360 degrees / 60 seconds = 6 degrees per second
    int16_t sec_angle = (timeinfo.tm_sec * 6) * 10; // Multiply by 10 for LVGL's angle format
    
    // Minutes: 360 degrees / 60 minutes = 6 degrees per minute
    // Add seconds influence: 6 degrees / 60 seconds = 0.1 degrees per second
    int16_t min_angle = ((timeinfo.tm_min * 6) + (timeinfo.tm_sec * 0.1)) * 10;
    
    // Hours: 360 degrees / 12 hours = 30 degrees per hour
    // Add minutes influence: 30 degrees / 60 minutes = 0.5 degrees per minute
    int16_t hour_angle = ((timeinfo.tm_hour % 12) * 30 + (timeinfo.tm_min * 0.5)) * 10;
    
    // Update the clock hands
    lv_img_set_angle(ui_sec, sec_angle);
    lv_img_set_angle(ui_min, min_angle);
    lv_img_set_angle(ui_hour, hour_angle);
}

// Timer callback function to update clock
static void clock_timer_cb(lv_timer_t *timer) {
    update_clock_hands();
}

// Initialize clock functionality
void init_clock(void) {
    // Set timezone if needed (example for UTC)
    setenv("TZ", "UTC", 1);
    tzset();
    
    // Create a timer to update clock every second
    lv_timer_create(clock_timer_cb, 1000, NULL);  // 1000ms = 1 second
    
    // Initial update
    update_clock_hands();
    
    ESP_LOGI(TAG, "Clock initialized");
}

// Function to update usage labels - optimized to reduce UI updates
void update_usage_labels(const char* cpu_text, const char* gpu_text) {
    // Cache previous values to avoid unnecessary UI updates
    static char last_cpu[16] = "";
    static char last_gpu[16] = "";
    
    // Only update CPU if text has changed
    if (strcmp(last_cpu, cpu_text) != 0) {
        if (ui_Label1 != NULL) {
            lv_label_set_text(ui_Label1, cpu_text);
            strcpy(last_cpu, cpu_text);
            ESP_LOGD(TAG, "CPU Usage label updated to: %s", cpu_text); // Changed to debug level
        } else {
            ESP_LOGE(TAG, "CPU Usage label not found!");
        }
    }

    // Only update GPU if text has changed
    if (strcmp(last_gpu, gpu_text) != 0) {
        if (ui_Label2 != NULL) {
            lv_label_set_text(ui_Label2, gpu_text);
            strcpy(last_gpu, gpu_text);
            ESP_LOGD(TAG, "GPU Usage label updated to: %s", gpu_text); // Changed to debug level
        } else {
            ESP_LOGE(TAG, "GPU Usage label not found!");
        }
    }
}

// Task to handle usage updates from queue - optimized with lower stack size
static void usage_task(void *pvParameters) {
    usage_data_t usage_data;
    while(1) {
        if(xQueueReceive(usage_queue, &usage_data, portMAX_DELAY)) {
            update_usage_labels(usage_data.cpu_usage, usage_data.gpu_usage);
            // Add small delay to prevent overwhelming the UI
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

// Function to initialize usage display
void init_usage_display(void) {
    // Create queue for usage updates
    usage_queue = xQueueCreate(3, sizeof(usage_data_t));  // Reduced from 5 to 3
    if (usage_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create usage queue");
        return;
    }

    // Create task for handling usage updates with reduced stack size
    BaseType_t ret = xTaskCreate(
        usage_task,       // Task function
        "usage_task",     // Task name
        1536,            // Stack size reduced from 2048 to 1536
        NULL,            // Task parameters
        5,               // Task priority
        NULL             // Task handle
    );
    
    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create usage task");
        return;
    }

    ESP_LOGI(TAG, "Usage display initialized");
}

// Function to be called from CDC callback when new data is received - optimized for efficiency
void process_usage_input(const char* input) {
    // Static counter to throttle updates (process only every 2nd message)
    static uint8_t update_counter = 0;
    
    // Skip some updates to reduce USB traffic and UI load
    if (++update_counter < 2) {
        return;
    }
    update_counter = 0;
    
    if (usage_queue == NULL || input == NULL) {
        return;
    }
    
    // Check if queue is full - don't process if it is
    if (uxQueueSpacesAvailable(usage_queue) == 0) {
        ESP_LOGW(TAG, "Usage queue full, skipping update");
        return;
    }
    
    // More efficient parsing with a single sscanf call
    int cpu_val = 0, gpu_val = 0;
    if (sscanf(input, "CPU:%d GPU:%d", &cpu_val, &gpu_val) == 2) {
        usage_data_t usage_data;
        snprintf(usage_data.cpu_usage, sizeof(usage_data.cpu_usage), "CPU: %d%%", cpu_val);
        snprintf(usage_data.gpu_usage, sizeof(usage_data.gpu_usage), "GPU: %d%%", gpu_val);
        
        // Use xQueueSendToBack with timeout to prevent blocking
        if (xQueueSendToBack(usage_queue, &usage_data, 0) != pdPASS) {
            ESP_LOGW(TAG, "Failed to send to usage queue");
        }
    }
}
