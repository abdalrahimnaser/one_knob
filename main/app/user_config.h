#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include <stdint.h>

// Default key mappings
#define DEFAULT_LEFT_KEY 0x52
#define DEFAULT_LEFT_MODIFIER 0
#define DEFAULT_RIGHT_KEY 0x51
#define DEFAULT_RIGHT_MODIFIER 0

// External variables to be used in app_features.c
extern uint8_t code_left;
extern uint8_t code_right;
extern uint8_t modifier_left;
extern uint8_t modifier_right;
extern uint8_t code_left2;  // Profile 2
extern uint8_t code_right2;  // Profile 2
extern uint8_t modifier_left2;  // Profile 2
extern uint8_t modifier_right2;  // Profile 2

// Function to load user configuration from partition
void load_user_config(void);

#endif // USER_CONFIG_H
