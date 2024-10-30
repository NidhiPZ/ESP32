#include <string.h>
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "ff.h"  // FATFS header for getting file system stats

static const char *TAG = "main";

#define MOUNT_POINT "/sdcard"
#define FILE_PATH  MOUNT_POINT "/example.txt"
#define RENAME_FILE_PATH  MOUNT_POINT "/file.txt"
#define DELETE_FILE_PATH  MOUNT_POINT "/Abc.txt"


// Check if a file exists on the SD card
static bool file_exists(const char *path) {
    struct stat st;
    ESP_LOGI(TAG, "Checking if file exists: %s", path);
    
    // Check file existence
    if (stat(path, &st) == 0) {
        ESP_LOGW(TAG, "File exists: %s", path);
        return true;
    } else {
        ESP_LOGW(TAG, "File does not exist: %s", path);
        return false;
    }
}

// Function to create and write data to a file
static esp_err_t create_and_write_file(const char *path, const char *data) {
    ESP_LOGI(TAG, "Creating and writing to file: %s", path);
    
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing: %s", path);
        return ESP_FAIL;
    }
    
    fprintf(f, "%s", data);
    fclose(f);
    
    ESP_LOGI(TAG, "File successfully created and written: %s", path);
    return ESP_OK;
}

// Function to append data to a file
static esp_err_t append_to_file(const char *path, const char *data) {
    ESP_LOGI(TAG, "Appending to file: %s", path);
    
    FILE *f = fopen(path, "a");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for appending: %s", path);
        return ESP_FAIL;
    }
    
    fprintf(f, "%s", data);
    fclose(f);
    
    ESP_LOGI(TAG, "Data successfully appended to file: %s", path);
    return ESP_OK;
}

// Function to read data from a file
static esp_err_t read_file(const char *path) {
    ESP_LOGI(TAG, "Reading file: %s", path);
    
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading: %s", path);
        return ESP_FAIL;
    }

    char buffer[128]; // Buffer to hold the read data
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        ESP_LOGI(TAG, "Read from file: %s", buffer);
    }

    fclose(f);
    return ESP_OK;
}

// Function to rename a file
static esp_err_t rename_file(const char *old_path, const char *new_path) {
    ESP_LOGI(TAG, "Renaming file from %s to %s", old_path, new_path);
    
    if (rename(old_path, new_path) != 0) {
        ESP_LOGE(TAG, "Failed to rename file from %s to %s", old_path, new_path);
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "File renamed successfully from %s to %s", old_path, new_path);
    return ESP_OK;
}

// Function to delete a file
static esp_err_t delete_file(const char *path) {
    ESP_LOGI(TAG, "Deleting file: %s", path);
    
    if (remove(path) != 0) {
        ESP_LOGE(TAG, "Failed to delete file: %s", path);
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "File deleted successfully: %s", path);
    return ESP_OK;
}

// Function to display available and total space on the SD card
static void display_available_space(const char *mount_point) {
    FATFS *fs;
    DWORD free_clusters, free_sectors, total_sectors;

    // Get the free clusters in the FAT file system
    if (f_getfree(&mount_point[1], &free_clusters, &fs) == FR_OK) {
        total_sectors = (fs->n_fatent - 2) * fs->csize;
        free_sectors = free_clusters * fs->csize;

        // Calculate total space and free space (in KB)
        uint64_t total_size = total_sectors / 2;  // Convert sectors to KB
        uint64_t free_size = free_sectors / 2;    // Convert sectors to KB

        ESP_LOGI(TAG, "Total space: %llu KB", total_size);
        ESP_LOGI(TAG, "Free space: %llu KB", free_size);
    } else {
        ESP_LOGE(TAG, "Failed to get free space information");
    }
}

void app_main(void) {
    esp_err_t ret;

    // Mount the filesystem
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    ESP_LOGI(TAG, "Initializing SD card");

    // SPI interface configuration
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = 23,
        .miso_io_num = 19,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus.");
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = 5;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount filesystem");
        return;
    }

    ESP_LOGI(TAG, "Filesystem mounted successfully.");

    // Print storage information
   display_available_space(MOUNT_POINT);

    // Check if the file exists and according to create
    if (!file_exists(FILE_PATH)) {
        vTaskDelay((2000)/portTICK_PERIOD_MS);
        // If the file does not exist, create it and write some data
        const char *data = "Hello, how are you!\n";
        ret = create_and_write_file(FILE_PATH, data);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error writing to file: %s", FILE_PATH);
            return;
        }
        ret = read_file(FILE_PATH);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error reading from file: %s", FILE_PATH);
            return;
        }
    } else {
        // If the file exists, read the data
        ret = read_file(FILE_PATH);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error reading from file: %s", FILE_PATH);
            return;
        }
    }

    // // Example: Append data to the file
    // if (file_exists(FILE_PATH)) {
    //     const char *append_data = "hello.\n";
    //     ret = append_to_file(FILE_PATH, append_data);
    //     if (ret != ESP_OK) {
    //         ESP_LOGE(TAG, "Error appending to file: %s", FILE_PATH);
    //         return;
    //     }
        
    // }

    // // Example: Rename the file
    // if (file_exists(FILE_PATH)) {
    //     ret = rename_file(FILE_PATH, RENAME_FILE_PATH);
    //     if (ret != ESP_OK) {
    //         ESP_LOGE(TAG, "Error renaming file: %s", FILE_PATH);
    //         return;
    //     }
    // }

    // // Example: Delete the renamed file
    // if (file_exists(RENAME_FILE_PATH)) {
    //     vTaskDelay((1000)/portTICK_PERIOD_MS);
    //     ret = delete_file(RENAME_FILE_PATH);
    //     if (ret != ESP_OK) {
    //         ESP_LOGE(TAG, "Error deleting file: %s", RENAME_FILE_PATH);
    //         return;
    //     }
    // }

    // Unmount filesystem and cleanup
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    ESP_LOGI(TAG, "Card unmounted.");

    // Deinitialize the SPI bus
    spi_bus_free(host.slot);
}
