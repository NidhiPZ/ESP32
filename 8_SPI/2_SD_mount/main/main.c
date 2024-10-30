#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_log.h"

#define MOUNT_POINT "/sdcard"

// Pin assignments for SPI
#define PIN_NUM_MISO  19
#define PIN_NUM_MOSI  23
#define PIN_NUM_CLK   18
#define PIN_NUM_CS    5

static const char *TAG = "SD_CARD_CHECK";

void app_main(void) {
    esp_err_t ret;

    // Configuration for mounting the SD card
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,  // Do not format if mount fails
        .max_files = 5,                   // Max number of open files
        .allocation_unit_size = 16 * 1024 // Allocation unit size
    };
    
    sdmmc_card_t *card;

    ESP_LOGI(TAG, "Initializing SD card");

    // Set up SPI interface
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus.");
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;  // Chip select pin
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
    // Mount the SD card filesystem
    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount filesystem");
        return;
    }
    
    ESP_LOGI(TAG, "SD card mounted successfully");

    // Unmount the filesystem
    ESP_LOGI(TAG, "Unmounting filesystem");
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    ESP_LOGI(TAG, "SD card unmounted");

    // Deinitialize the SPI bus
    spi_bus_free(host.slot);
}
