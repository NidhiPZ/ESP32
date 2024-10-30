#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

static const char *TAG = "SPIFFS";

// Define macros for file paths
#define WRITE_FILE_PATH "/spiffs/first.txt"
#define APPEND_FILE_PATH "/spiffs/for.txt"
#define DELETE_FILE_PATH "/spiffs/first.txt"
#define RENAME_OLD_FILE_PATH "/spiffs/five.txt"
#define RENAME_NEW_FILE_PATH "/spiffs/for.txt"

// Function to check if a file exists using stat
bool file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0); // Returns 0 if the file exists
}

void app_main(void)
{
    // SPIFFS system initiation
    printf("\nSPIFFS system initiation:\n");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};
    esp_vfs_spiffs_register(&conf);
    size_t total = 0, used = 0;
    esp_spiffs_info(conf.partition_label, &total, &used);
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);

    // // Check if the file exists before writing
    // if (file_exists(WRITE_FILE_PATH)) {
    //     ESP_LOGI(TAG, "File '%s' already exists.", WRITE_FILE_PATH);
    // } else {
    //     ESP_LOGI(TAG, "File '%s' does not exist. Creating a new file.", WRITE_FILE_PATH);

    //     // Write to the file
    //     printf("\nWrite to the file:\n");
    //     FILE *f = fopen(WRITE_FILE_PATH, "w"); // Write mode
    //     if (f == NULL) {
    //         ESP_LOGE(TAG, "Failed to open file for writing");
    //     } else {
    //         fprintf(f, "helloooooo.\n");
    //         fclose(f);
    //         ESP_LOGI(TAG, "File written: '%s'", WRITE_FILE_PATH);
    //     }
    // }

    // Append to the file
    if (file_exists(APPEND_FILE_PATH)) 
    {
        ESP_LOGI(TAG, "File '%s' is exist.", APPEND_FILE_PATH);
        printf("\nAppend to the file:\n");
        ESP_LOGI(TAG, "Opening file for appending");
        FILE *f = fopen(APPEND_FILE_PATH, "a"); // Append mode
        if (f == NULL) {
            ESP_LOGE(TAG, "File doesn't exist.");
        } else {
            fprintf(f, "Appended text to the file.\n");
            fclose(f);
            ESP_LOGI(TAG, "Text appended to '%s'", APPEND_FILE_PATH);
        }
    } 
    else {
        ESP_LOGW(TAG, "File '%s' does not exist, creating a new file.", APPEND_FILE_PATH);
    }

    // // Open file for reading
    // ESP_LOGI(TAG, "Reading file '%s'", APPEND_FILE_PATH);
    // FILE *f = fopen(APPEND_FILE_PATH, "r");
    // if (f == NULL) {
    //     ESP_LOGE(TAG, "File does't exist.");
    // } else {
    //     char line[64];
    //     while (fgets(line, sizeof(line), f) != NULL) {
    //         ESP_LOGI(TAG, "Read from file: '%s'", line);
    //     }
    //     fclose(f);
    // }

    // // Check if the file to delete exists
    // printf("\nDelete the file:\n");
    // if (file_exists(DELETE_FILE_PATH)) {
    //     vTaskDelay(100);
    //     ESP_LOGI(TAG, "Deleting file '%s'", DELETE_FILE_PATH);
    //     if (unlink(DELETE_FILE_PATH) == 0) {
    //         ESP_LOGI(TAG, "File deleted");
    //     } else {
    //         ESP_LOGE(TAG, "Failed to delete the file");
    //     }
    // } else {
    //     ESP_LOGW(TAG, "File '%s' does not exist, cannot delete", DELETE_FILE_PATH);
    // }

    //  // Rename a file
    // printf("\nRenaming file:\n");
    // if (file_exists(RENAME_OLD_FILE_PATH)) {
    //     vTaskDelay(100);
    //     if (rename(RENAME_OLD_FILE_PATH, RENAME_NEW_FILE_PATH) == 0) {
    //         ESP_LOGI(TAG, "File renamed from '%s' to '%s'", RENAME_OLD_FILE_PATH, RENAME_NEW_FILE_PATH);
    //     } else {
    //         ESP_LOGE(TAG, "Failed to rename file '%s'", RENAME_OLD_FILE_PATH);
    //     }
      
    // } else {
       
    //       ESP_LOGW(TAG, "File '%s' does not exist, cannot rename", RENAME_OLD_FILE_PATH); 
    // }

    // List files in the directory
    DIR *d;
    struct dirent *dir;
    d = opendir("/spiffs");
    printf("\nList of files in the directory:\n");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            ESP_LOGI(TAG, "%s", dir->d_name);
        }
        closedir(d);
    }

    // Unmount partition and disable SPIFFS
    printf("\nSPIFFS system unmount:\n");
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}
