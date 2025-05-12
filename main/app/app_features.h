/**
 * @file app_features.h
 * @brief Application features and UI interactions
 */

#pragma once

#include "esp_err.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the application features
 * 
 * @return ESP_OK if successful, error code otherwise
 */
esp_err_t app_features_init(void);

/**
 * @brief Register callbacks for UI elements
 * 
 * This should be called after the UI has been initialized
 * 
 * @return ESP_OK if successful, error code otherwise
 */
esp_err_t app_features_register_ui_callbacks(void);

/**
 * @brief Example timer button callback
 * 
 * @param e LVGL event
 */
void app_timer_button_callback(lv_event_t *e);

#ifdef __cplusplus
}
#endif 