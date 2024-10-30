#include <stdio.h>
#include <string.h>
#include "mx25r3235f.h"
#include "esp_log.h"
#include "stdbool.h"

#define TAG "MX25R3235F_Test"
#define FLASH_START_ADDRESS 0x00000000

// Define a structure for the data you want to store
typedef struct {
    int id;
    char name[20];
    float value;
} my_data_t;

void app_main() {
    esp_err_t ret;

    // Initialize the flash
    ret = MX25R3235F_Flash_Init();
    if (ret != FLASH_OK) {
        ESP_LOGE(TAG, "Failed to initialize MX25R3235F flash chip");
        return;
    }

    // Erase a 4KB sector before writing
    ret = MX25R3235F_Flash_Erase(FLASH_START_ADDRESS, QSPI_ERASE_LEN_4KB);
    if (ret != FLASH_OK) {
        ESP_LOGE(TAG, "Failed to erase sector");
        MX25R3235F_Flash_DeInit();
        return;
    }
    ESP_LOGI(TAG, "Sector erased successfully");

    // Prepare test data to write
    my_data_t my_data;
    my_data.id = 1; // Example ID
    snprintf(my_data.name, sizeof(my_data.name), "Sensor1"); // Example name
    my_data.value = 23.5; // Example value

    // Write data to flash
    ret = MX25R3235F_Flash_Write((uint8_t*)&my_data, sizeof(my_data), FLASH_START_ADDRESS);
    if (ret != FLASH_OK) {
        ESP_LOGE(TAG, "Failed to write data to flash");
        MX25R3235F_Flash_DeInit();
        return;
    }
    ESP_LOGI(TAG, "Data written to flash successfully");
// Initialize a struct to read back data
my_data_t read_data = {0};  // Ensure struct is zeroed out for safety

// Perform read operation from flash
ret = MX25R3235F_Flash_Read((uint8_t*)&read_data, sizeof(read_data), FLASH_START_ADDRESS);
if (ret != FLASH_OK) {
    ESP_LOGE(TAG, "Failed to read data from flash");
    MX25R3235F_Flash_DeInit();
    return;
} else {
    ESP_LOGI(TAG, "Data read successfully. Field: %d", read_data);
}
    // Deinitialize the flash
    MX25R3235F_Flash_DeInit();
}
