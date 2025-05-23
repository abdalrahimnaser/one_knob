#include "user_config.h"
#include "esp_partition.h"
#include "esp_log.h"


static const char *TAG = "user_config";

// Define the variables with default values
uint8_t code_left = DEFAULT_LEFT_KEY;
uint8_t code_right = DEFAULT_RIGHT_KEY;
uint8_t modifier_left = DEFAULT_LEFT_MODIFIER;
uint8_t modifier_right = DEFAULT_RIGHT_MODIFIER;

void load_user_config(void) {
    // Find the user_config partition
    const esp_partition_t *config_partition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, 0x99, "user_config");
    
    if (config_partition) {
        // Configuration structure
        struct {
            uint8_t left_key;
            uint8_t left_mod;
            uint8_t right_key;
            uint8_t right_mod;
            uint32_t magic; // Magic number to verify valid config
        } config;
        
        esp_err_t err = esp_partition_read(config_partition, 0, &config, sizeof(config));
        
        // Check if read was successful and magic number matches
        if (err == ESP_OK && config.magic == 0xABCD1234) {
            code_left = config.left_key;
            modifier_left = config.left_mod;
            code_right = config.right_key;
            modifier_right = config.right_mod;
            ESP_LOGI(TAG, "User configuration loaded successfully");
        } else {
            ESP_LOGW(TAG, "Invalid configuration or read error, using defaults");
        }
    } else {
        ESP_LOGW(TAG, "Configuration partition not found, using defaults");
    }
}
