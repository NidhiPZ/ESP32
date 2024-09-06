#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_2  // Onboard LED pin (GPIO2 for most ESP32 boards)

static const char *TAG = "uart_led_control";

static void uart_led_control_task(void *arg)
{
    // UART configuration
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, GPIO_NUM_1, GPIO_NUM_3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)); 

    // Install UART driver
    const int uart_buffer_size = (1024 * 2);
    uart_driver_install(UART_NUM_0, uart_buffer_size, uart_buffer_size, 0, NULL, 0);

    // Configure the onboard LED pin as output
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    uint8_t data[2];  // Buffer to store incoming UART data (1 byte + '\0')

    while (1)
    {
        // Read 1 byte from UART
        int len = uart_read_bytes(UART_NUM_0, data, 1, 10/portTICK_PERIOD_MS);

        if (len > 0)
        {
            data[len] = '\0';  // Null-terminate the received data
            ESP_LOGI(TAG, "Received: %s", data);

            // Check the received byte
            if (data[0] == '1')
            {
                // Turn on the LED
                gpio_set_level(LED_GPIO, 1);
                ESP_LOGI(TAG, "LED ON");
            }
            else if (data[0] == '0')
            {
                // Turn off the LED
                gpio_set_level(LED_GPIO, 0);
                ESP_LOGI(TAG, "LED OFF");
            }
            else
            {
               ESP_LOGI(TAG, "Invalid character");
            }
        }
    }
}

void app_main(void)
{
    // Create the UART task
    xTaskCreate(uart_led_control_task, "uart_led_control_task", 4096, NULL, 10, NULL);
}
