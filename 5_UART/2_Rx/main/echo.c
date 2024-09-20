#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "uart_test";

static void uart_task(void *arg)
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, GPIO_NUM_1, GPIO_NUM_3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);
    QueueHandle_t uart_queue;
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0));

    uint8_t data[128];

    while (1) 
    {
        // Read data from UART
        int len = uart_read_bytes(UART_NUM_0, data, sizeof(data) - 1, 10);
        // Echo the received data back
         uart_write_bytes(UART_NUM_0, (const char *)data, len);
        if (len)
        {
            // Null-terminate the received data
            data[len] = '\0';
            // Log the received string
            ESP_LOGI(TAG, " Recv str = %s", (char *)data);
        }
    }
}

void app_main(void)
{
    xTaskCreate(uart_task, "uart_task", 4096, NULL, 10, NULL);
}
