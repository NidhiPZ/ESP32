#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "sdkconfig.h"

static const char *TAG = "example";
static uint8_t buf[CONFIG_TINYUSB_CDC_RX_BUFSIZE + 1];

// Function to generate random sensor data
void generate_sensor_data(char *buffer, size_t size) {
    // Generate a random value (for example, between 0 and 100)
    int random_value = rand() % 101; // Random value between 0 and 100
    snprintf(buffer, size, "Sensor Value: %d\n", random_value);
}

// Callback function for received data
void tinyusb_cdc_rx_callback(int itf, cdcacm_event_t *event) {
    size_t rx_size = 0;

    // Read data from the USB CDC
    esp_err_t ret = tinyusb_cdcacm_read(itf, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Data from channel %d:", itf);
        ESP_LOG_BUFFER_HEXDUMP(TAG, buf, rx_size, ESP_LOG_INFO);
        
        // Echo the received data back
        tinyusb_cdcacm_write_queue(itf, buf, rx_size);
        tinyusb_cdcacm_write_flush(itf, 0);
    } else {
        ESP_LOGE(TAG, "Read error");
    }
}

// Callback function for line state changes (DTR, RTS)
void tinyusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event) {
    int dtr = event->line_state_changed_data.dtr;
    int rts = event->line_state_changed_data.rts;
    ESP_LOGI(TAG, "Line state changed on channel %d: DTR:%d, RTS:%d", itf, dtr, rts);
}

void app_main(void) {
    ESP_LOGI(TAG, "USB initialization");

    // Initialize random seed
    srand(time(NULL));

    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = NULL,
        .external_phy = false,
#if (TUD_OPT_HIGH_SPEED)
        .fs_configuration_descriptor = NULL,
        .hs_configuration_descriptor = NULL,
        .qualifier_descriptor = NULL,
#else
        .configuration_descriptor = NULL,
#endif
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &tinyusb_cdc_rx_callback,
        .callback_line_state_changed = &tinyusb_cdc_line_state_changed_callback, // Register the callback
        .callback_line_coding_changed = NULL
    };

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));

    ESP_LOGI(TAG, "USB initialized. Sending sensor data...");

    char sensor_data[50];
    const int sensor_update_interval = 1000; // Interval in milliseconds

    while (1) {
        generate_sensor_data(sensor_data, sizeof(sensor_data)); // Generate random sensor data
        tinyusb_cdcacm_write_queue(TINYUSB_CDC_ACM_0, (uint8_t *)sensor_data, strlen(sensor_data));
        tinyusb_cdcacm_write_flush(TINYUSB_CDC_ACM_0, 0);
        vTaskDelay(pdMS_TO_TICKS(sensor_update_interval)); // Delay based on the interval
    }
}
