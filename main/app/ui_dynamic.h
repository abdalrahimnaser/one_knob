#ifndef UI_DYNAMIC_H
#define UI_DYNAMIC_H

#include "esp_err.h"

/**
 * @brief Initialize the usage display system
 * 
 * This function initializes the queue and task for handling CPU/GPU usage updates
 */
void init_usage_display(void);

/**
 * @brief Process incoming usage data from serial
 * 
 * @param input The input string containing CPU and GPU usage data
 *             Expected format: "CPU:XX GPU:YY"
 */
void process_usage_input(const char* input);

/**
 * @brief Initialize the clock functionality
 */
void init_clock(void);

/**
 * @brief Update the clock hands based on current time
 */
void update_clock_hands(void);

#endif /* UI_DYNAMIC_H */ 


