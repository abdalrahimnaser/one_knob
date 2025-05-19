/**
 * @file app_features.h
 * @brief Application features and UI interactions
 */

#ifndef APP_FEATURES_H
#define APP_FEATURES_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize all app features
 * 
 * This function initializes:
 * - USB HID and CDC interfaces
 * - Button configuration
 * - Usage display system
 * - Volume control system
 * 
 * @return esp_err_t ESP_OK on success, otherwise an error code
 */
esp_err_t app_features_init(void);

/**
 * @brief Process knob events for profile 1
 * 
 * @param event The event data
 */
void profile_1_process_knob_event(void *event);

/**
 * @brief Process knob events for volume control
 * 
 * @param event The event data
 */
void volume_control_process_knob_event(void *event);

#ifdef __cplusplus
}
#endif

#endif /* APP_FEATURES_H */ 