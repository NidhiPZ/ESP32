#include <stdio.h>
#include <stdlib.h> // for random value generation
#include <string.h> // for strlen and snprintf
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"

char *TAG = "BLE-Server";
uint8_t ble_addr_type;
uint16_t connection_handle = 0; // Store the connection handle

// Placeholder for sensor characteristic handle
uint16_t sensor_char_val_handle = 0;

// Function prototypes
void ble_app_advertise(void);
static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg);
void notify_sensor_value();
void notify_task(void* param);

// Simulated sensor value generation
int read_sensor_value() {
    return rand() % 100; // Generate a random value between 0 and 99
}

// // Write data to ESP32 defined as server
// static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
// {
//     char data[ctxt->om->om_len + 1];
//     memcpy(data, ctxt->om->om_data, ctxt->om->om_len);
//     data[ctxt->om->om_len] = '\0'; // Null-terminate the string

//     if (strncmp(data, "LED ON", ctxt->om->om_len) == 0)
//     {
//         gpio_set_level(GPIO_NUM_2, 1);
//         ESP_LOGI(TAG, "LED is ON");
//     }
//     else if (strncmp(data, "LED OFF", ctxt->om->om_len) == 0)
//     {
//         gpio_set_level(GPIO_NUM_2, 0);
//         ESP_LOGI(TAG, "LED is OFF");
//     }
//     else
//     {
//         ESP_LOGI(TAG, "Received unknown command: %s", data);
//     }
//     return 0;
// }

// Read data from ESP32 defined as server (simulated sensor data)
static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    int sensor_value = read_sensor_value(); // Simulated sensor reading
    char sensor_str[16];
    snprintf(sensor_str, sizeof(sensor_str), "Sensor: %d", sensor_value);
    os_mbuf_append(ctxt->om, sensor_str, strlen(sensor_str)); // Send sensor value to client
    return 0;
}

// Array of pointers to other service definitions
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(0x1801), // GATT Service
        .characteristics = (struct ble_gatt_chr_def[]){
            {
                .uuid = BLE_UUID16_DECLARE(0xFEF4), // Custom UUID for Sensor Data
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                .access_cb = device_read,
                .val_handle = &sensor_char_val_handle // Store the value handle
            },
            // {
            //     .uuid = BLE_UUID16_DECLARE(0xDEAD), // Custom UUID for LED Control
            //     .flags = BLE_GATT_CHR_F_WRITE,
            //     .access_cb = device_write
            // },
            { 0 } // Terminate the array
        }
    },
    { 0 } // Terminate the array
};

// BLE event handling
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI(TAG, "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        if (event->connect.status == 0)
        {
            connection_handle = event->connect.conn_handle; // Store connection handle
             ESP_LOGI("GAP", "Connected with handle: %d", connection_handle);
        }
        else
        {
            ble_app_advertise(); // Retry advertising if connection failed
        }
        break;

    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI(TAG, "BLE GAP EVENT DISCONNECTED");
        connection_handle = 0; // Clear connection handle
        ble_app_advertise(); // Restart advertising
        break;
        
    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI(TAG, "BLE GAP EVENT ADV COMPLETE");
        ble_app_advertise(); // Restart advertising after advertising completes
        break;
    default:
        break;
    }
    return 0;
}

// Define the BLE connection
void ble_app_advertise(void)
{
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields));
    device_name = ble_svc_gap_device_name(); // Retrieve device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&fields); // Set advertising fields

    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // Undirected connectable
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // General discoverable
    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL); // Start advertising
}

void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Automatically determine address type
    ble_app_advertise(); // Start advertising
}

void host_task(void *param)
{
    nimble_port_run(); // Runs the NimBLE host until stopped
}

void notify_sensor_value()
{
    // if (connection_handle != 0 ) // Ensure there's an active connection and valid handle
    // {
        int sensor_value = read_sensor_value(); // Simulate sensor reading
        char sensor_str[16];
        snprintf(sensor_str, sizeof(sensor_str), "Sensor: %d", sensor_value);
        struct os_mbuf *om = ble_hs_mbuf_from_flat(sensor_str, strlen(sensor_str)); // Create a notification buffer

        // Send notification to the sensor characteristic
        int rc = ble_gatts_notify(connection_handle, sensor_char_val_handle );
          if (connection_handle == 0) { // Ensure you have a valid connection handle
        ESP_LOGI("BLE", "Sending notification: %d", sensor_value);
          int rc = ble_gatts_notify(connection_handle, sensor_char_val_handle );
    } else {
        ESP_LOGE("BLE", "No active connection to send notification");
    }
}


void notify_task(void* param)
{
    while (1)
    {
        notify_sensor_value(); // Send sensor data notification
        vTaskDelay(pdMS_TO_TICKS(5000)); // Wait for 5 seconds
    }
}

void app_main()
{
    // Initialize GPIO
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    // Initialize NVS
    esp_err_t ret = nvs_flash_erase();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to erase NVS partition: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "NVS partition erased successfully");
    }

    ret = nvs_flash_init();  // Initialize NVS flash
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize NVS: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "NVS initialized successfully");
    }
    
    // Initialize NimBLE stack
    nimble_port_init();
    ble_svc_gap_device_name_set("BLE-Server"); // Set the BLE server name
    ble_svc_gap_init();                        // Initialize the GAP service
    ble_svc_gatt_init();                       // Initialize the GATT service
    ble_gatts_count_cfg(gatt_svcs);            // Configure GATT services
    ble_gatts_add_svcs(gatt_svcs);             // Add GATT services
    ble_hs_cfg.sync_cb = ble_app_on_sync;      // Set sync callback
    nimble_port_freertos_init(host_task);      // Start the NimBLE task

    // Create a FreeRTOS task to periodically notify sensor values
    xTaskCreate(notify_task, "notify_task", 4096, NULL, 5, NULL);
}
