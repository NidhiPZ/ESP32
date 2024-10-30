#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "spp_task.h"

#define SPP_TAG "SPP_INITIATOR_DEMO"
#define EXAMPLE_DEVICE_NAME "ESP_SPP_INITIATOR"

// Change this to the MAC address of your SPP server device
static const char *server_mac_address = "b4:31:61:32:d3:7a"; // Replace with your SPP server's MAC address
static esp_bd_addr_t peer_bd_addr;

static esp_spp_sec_t sec_mask = ESP_SPP_SEC_AUTHENTICATE;
static esp_spp_role_t role_master = ESP_SPP_ROLE_MASTER;

#define SPP_DATA_LEN 20

// Function to convert MAC address string to esp_bd_addr_t
static void str2bda(const char *str, esp_bd_addr_t bda) {
    sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &bda[0], &bda[1], &bda[2], &bda[3], &bda[4], &bda[5]);
}

// Function to convert esp_bd_addr_t to a string (MAC address format)
static const char *bda2str(const esp_bd_addr_t bda, char *str, size_t size) {
    snprintf(str, size, "%02x:%02x:%02x:%02x:%02x:%02x", 
             bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
    return str;
}

static void spp_write_handle(void *param) {
    int fd = (int)param;
    char data[SPP_DATA_LEN];

    while (1) {
        // Prepare data to send
        snprintf(data, sizeof(data), "Hello from ESP32");
        esp_err_t err = esp_spp_write(fd, sizeof(data), (uint8_t *)data);
        if (err != ESP_OK) {
            ESP_LOGE(SPP_TAG, "Error sending data: %s", esp_err_to_name(err));
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Send data every 2 seconds
    }
}

static void esp_spp_cb(uint16_t e, void *p) {
    esp_spp_cb_event_t event = e;
    esp_spp_cb_param_t *param = p;
    char bda_str[18] = {0};

    switch (event) {
    case ESP_SPP_INIT_EVT:
        if (param->init.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
            esp_spp_vfs_register();
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_INIT_EVT status:%d", param->init.status);
        }
        break;

    case ESP_SPP_OPEN_EVT:
        if (param->open.status == ESP_SPP_SUCCESS) {
            ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT handle:%" PRIu32 " fd:%d rem_bda:[%s]",
                     param->open.handle, param->open.fd, bda2str(param->open.rem_bda, bda_str, sizeof(bda_str)));
            spp_write_handle((void *)param->open.fd);
        } else {
            ESP_LOGE(SPP_TAG, "ESP_SPP_OPEN_EVT status:%d", param->open.status);
        }
        break;

    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT status:%d handle:%" PRIu32 " close_by_remote:%d",
                 param->close.status, param->close.handle, param->close.async);
        break;

    case ESP_SPP_WRITE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_WRITE_EVT handle:%" PRIu32 " status:%d",
                 param->write.handle, param->write.status);
        break;

    case ESP_SPP_DATA_IND_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DATA_IND_EVT len:%d handle:%" PRIu32,
                 param->data_ind.len, param->data_ind.handle);
        // Handle incoming data here if needed
        break;

    default:
        break;
    }
}

// GAP callback function
static void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    switch (event) {
        case ESP_BT_GAP_AUTH_CMPL_EVT:
            ESP_LOGI(SPP_TAG, "Authentication completed");
            break;
        // Handle other GAP events as necessary
        default:
            break;
    }
}

void app_main(void) {
    esp_err_t ret = ESP_OK;

    // Initialize NVS
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize and enable the Bluetooth controller
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT));

    // Initialize and enable the Bluedroid stack
    esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bluedroid_init_with_cfg(&bluedroid_cfg));
    ESP_ERROR_CHECK(esp_bluedroid_enable());

    // Register GAP and SPP callbacks
    ESP_ERROR_CHECK(esp_bt_gap_register_callback(esp_bt_gap_cb));
    ESP_ERROR_CHECK(esp_spp_register_callback(esp_spp_cb));

    // Initialize SPP
    esp_spp_cfg_t bt_spp_cfg = BT_SPP_DEFAULT_CONFIG();
    ESP_ERROR_CHECK(esp_spp_enhanced_init(&bt_spp_cfg));

    // Convert MAC address string to esp_bd_addr_t
    str2bda(server_mac_address, peer_bd_addr);
    // Directly connect to the server
    esp_spp_connect(sec_mask, role_master, 0, peer_bd_addr); // ESP_SPP_CHANNEL should be replaced with the correct channel

    ESP_LOGI(SPP_TAG, "Initiating connection to server at [%s]", server_mac_address);
}
