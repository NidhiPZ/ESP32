#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
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
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_heap_caps.h" // Include for heap memory check
#include "time.h"
#include "sys/time.h"

#define SPP_SERVER_NAME "SPP_SERVER"
#define EXAMPLE_DEVICE_NAME "ESP32_BT"

#define ledR 14
#define ledG 12
#define ledB 2

const char *tag = "Bluetooth";

static const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;
static const esp_spp_sec_t sec_mask = ESP_SPP_SEC_AUTHENTICATE;
static const esp_spp_role_t role_slave = ESP_SPP_ROLE_SLAVE;

/**
 * @brief     This function sets the GPIO direction for the RGB LEDs and resets their initial states.
 * @param     None
 * @return    None
 */
static void init_led(void)
{
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG, GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB, GPIO_MODE_OUTPUT);

    ESP_LOGI(tag, "Init LED completed");
}

/**
 * @brief    Checks the current available heap memory
*/
static void check_heap_memory(void)
{
    size_t free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    ESP_LOGI(tag, "Free heap size: %d bytes", free_heap);
}

/**
 * @brief  Bluetooth SPP event callback.
 * This function handles various SPP events like initialization, opening, data reception, etc.
 * 
 * @param      event  The SPP event type.
 * @param      param  Pointer to the event parameters, which vary depending on the event.
 * 
 * @return          None
 */
static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event)
    {
    case ESP_SPP_INIT_EVT:
        ESP_LOGI(tag, "ESP_SPP_INIT_EVT");
        esp_spp_start_srv(sec_mask, role_slave, 0, SPP_SERVER_NAME);
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        ESP_LOGI(tag, "ESP_SPP_DISCOVERY_COMP_EVT");
        break;
    case ESP_SPP_OPEN_EVT:
        ESP_LOGI(tag, "ESP_SPP_OPEN_EVT");
        break;
    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(tag, "ESP_SPP_CLOSE_EVT");
        break;
    case ESP_SPP_START_EVT:
        ESP_LOGI(tag, "ESP_SPP_START_EVT");
        esp_bt_dev_set_device_name(EXAMPLE_DEVICE_NAME);
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        break;
    case ESP_SPP_DATA_IND_EVT:

        ESP_LOGI(tag, "ESP_SPP_DATA_IND_EVT len=%d handle=%lu", param->data_ind.len, param->data_ind.handle);
        esp_log_buffer_hex("", param->data_ind.data, param->data_ind.len);
        printf("Value received: ");
        for (size_t i = 0; i < param->data_ind.len; i++)
        {
            char value = param->data_ind.data[i];
            printf("%c", value);

            switch (value)
            {
            case 'R':
                gpio_set_level(ledR, 1);
                gpio_set_level(ledG, 0);
                gpio_set_level(ledB, 0);
                break;
            case 'G':
                gpio_set_level(ledR, 0);
                gpio_set_level(ledG, 1);
                gpio_set_level(ledB, 0);
                break;
            case 'B':
                gpio_set_level(ledR, 0);
                gpio_set_level(ledG, 0);
                gpio_set_level(ledB, 1);
                break;
            case 'O':
                gpio_set_level(ledR, 0);
                gpio_set_level(ledG, 0);
                gpio_set_level(ledB, 0);
                break;
            default:
                break;
            }
        }
        printf("\n");
        esp_spp_write(param->data_ind.handle, param->data_ind.len, param->data_ind.data);
        break;

    case ESP_SPP_CONG_EVT:
        ESP_LOGI(tag, "ESP_SPP_CONG_EVT");
        break;
    case ESP_SPP_WRITE_EVT:
        ESP_LOGI(tag, "ESP_SPP_WRITE_EVT");
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(tag, "ESP_SPP_SRV_OPEN_EVT");
        
        break;
    default:
        break;
    }
}

/**
 * @brief    Bluetooth GAP event callback.
 * This function handles GAP events like authentication completion, PIN request, and others.
 * 
 * @param      event  The GAP event type.
 * @param      param  Pointer to the event parameters, which vary depending on the event.
 * 
 * @return   None
 */
void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event)
    {
    case ESP_BT_GAP_AUTH_CMPL_EVT:
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGI(tag, "Authentication success: %s", param->auth_cmpl.device_name);
            esp_log_buffer_hex(tag, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
        }
        else
        {
            ESP_LOGE(tag, "Authentication failed, status: %d", param->auth_cmpl.stat);
        }
        break;
    case ESP_BT_GAP_PIN_REQ_EVT:
        ESP_LOGI(tag, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit: %d", param->pin_req.min_16_digit);
        if (param->pin_req.min_16_digit)
        {
            ESP_LOGI(tag, "Input pin code: 0000 0000 0000 0000");
            esp_bt_pin_code_t pin_code = {0};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
        }
        else
        {
            ESP_LOGI(tag, "Input pin code: 1234");
            esp_bt_pin_code_t pin_code = {'1', '2', '3', '4'};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
        }
        break;
    default:
        ESP_LOGI(tag, "event: %d", event);
        break;
    }
}


/**
 * @brief    Main application entry point.
 */

void app_main(void)
{
    // Initialize the LEDs for the RGB lights (Red, Green, Blue)
    init_led();
    
    // Check and log the available heap memory before initializing Bluetooth
    check_heap_memory();

    // Erase the NVS partition to ensure a clean start
    esp_err_t ret = nvs_flash_erase();
    if (ret != ESP_OK) {
        ESP_LOGE("NVS", "Failed to erase NVS partition: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI("NVS", "NVS partition erased successfully");
    }

    // Initialize the NVS (Non-Volatile Storage) after erasing the partition
    ret = nvs_flash_init();
    if (ret != ESP_OK) {
        ESP_LOGE("NVS", "Failed to initialize NVS: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI("NVS", "NVS initialized successfully");
    }

    // Ensure that the NVS initialization was successful
    ESP_ERROR_CHECK(ret);

    // Release memory allocated for BLE (Bluetooth Low Energy) since we only need Classic Bluetooth
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    // Initialize the Bluetooth controller with default configurations
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK)
    {
        ESP_LOGE(tag, "Initialize controller failed: %s", esp_err_to_name(ret));
        return; 
    }

    // Enable the Bluetooth controller in Classic Bluetooth mode
    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)
    {
        ESP_LOGE(tag, "Enable controller failed: %s", esp_err_to_name(ret));
        return; 
    }

    // Initialize Bluedroid, which is the Bluetooth protocol stack
    if ((ret = esp_bluedroid_init()) != ESP_OK)
    {
        ESP_LOGE(tag, "Initialize bluedroid failed: %s", esp_err_to_name(ret));
        return; 
    }

    // Enable Bluedroid for handling Bluetooth communication
    if ((ret = esp_bluedroid_enable()) != ESP_OK)
    {
        ESP_LOGE(tag, "Enable bluedroid failed: %s", esp_err_to_name(ret));
        return; 
    }

    // Register GAP callback to handle Bluetooth pairing and security events
    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK)
    {
        ESP_LOGE(tag, "GAP register failed: %s", esp_err_to_name(ret));
        return; 
    }

    // Register SPP callback to handle Bluetooth data transmission and events
    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK)
    {
        ESP_LOGE(tag, "SPP register failed: %s", esp_err_to_name(ret));
        return;
    }

    // Initialize SPP in callback mode (ESP_SPP_MODE_CB), which triggers the SPP event handler
    if ((ret = esp_spp_init(esp_spp_mode)) != ESP_OK)
    {
        ESP_LOGE(tag, "SPP init failed: %s", esp_err_to_name(ret));
        return; 
    }

    // Check and log the available heap memory after initializing 
    check_heap_memory();
}
