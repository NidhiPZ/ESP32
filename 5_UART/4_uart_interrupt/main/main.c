#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "soc/uart_struct.h"
#include "esp_intr_alloc.h"
#include "soc/uart_reg.h"

#define UART_NUM UART_NUM_0
#define BUF_SIZE (1024)
#define TXD_PIN (1)
#define RXD_PIN (3)

static const char *TAG = "UART_INTERRUPT";

// Queue to hold UART events
static QueueHandle_t uart_queue;

// UART interrupt handler
static void IRAM_ATTR uart_intr_handle(void *arg) {
    uint16_t rx_fifo_len;
    uint8_t data[BUF_SIZE];

    // Check the UART interrupt status for UART0
    uint32_t status = UART0.int_st.val;

    // Handle the RX FIFO full interrupt
    if (status & UART_RXFIFO_FULL_INT_ST_M) {
        // Get the length of data in RX FIFO
        rx_fifo_len = UART0.status.rxfifo_cnt;
        
        // Read directly from UART FIFO
        for (int i = 0; i < rx_fifo_len; i++) {
            data[i] = UART0.fifo.rw_byte;  // Directly access UART0 FIFO
        }
        
        data[rx_fifo_len] = '\0';  // Null-terminate the received data

        ESP_LOGI(TAG, "Received: %s", data);

        // Clear the RX FIFO full interrupt status for UART0
        uart_clear_intr_status(UART_NUM, UART_RXFIFO_FULL_INT_CLR_M);
    }
}

// UART initialization function
void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Configure UART parameters
    uart_param_config(UART_NUM, &uart_config);

    // Install UART driver using a buffer for RX
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 10, &uart_queue, 0);

    // Set UART pins
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Enable UART RX interrupt
    uart_enable_rx_intr(UART_NUM);

    // Allocate UART interrupt and set the ISR handler
    //esp_intr_alloc(ETS_UART0_INTR_SOURCE, 0, uart_intr_handle, NULL, NULL);
    esp_intr_alloc(ETS_UART0_INTR_SOURCE, ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM, uart_intr_handle, NULL, NULL);

}

void app_main(void) {
    uart_init();

    ESP_LOGI(TAG, "UART RX Interrupt system initialized. Waiting for data...");

    while (1) {
        // Main loop can do other work, data is handled in the interrupt handler
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
