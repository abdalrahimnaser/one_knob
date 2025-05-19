#include <time.h>
#include "esp_system.h"
#include "esp_log.h"
#include "ui/ui.h"

static const char *TAG = "UI_DYNAMIC";

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
