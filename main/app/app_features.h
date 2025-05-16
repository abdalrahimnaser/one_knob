/**
 * @file app_features.h
 * @brief Application features and UI interactions
 */

#pragma once

#include "esp_err.h"

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
 * @brief Process knob events for volume control
 * 
 * @param event Knob event data
 */
void app_process_knob_event(void *event);

#ifdef __cplusplus
}
#endif 