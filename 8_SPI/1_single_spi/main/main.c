#include <stdio.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SCLK_PIN 18
#define SPI_CS_PIN   5

static const char *TAG = "SPI_DMA";

// SPI device handle
spi_device_handle_t spi;

// Function to initialize SPI with DMA
void spi_master_init(void) {
    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MOSI_PIN,
        .miso_io_num = SPI_MISO_PIN,
        .sclk_io_num = SPI_SCLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,  // Max transfer size (set for DMA)
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,  // 1 MHz
        .mode = 0,                  // SPI mode 0 (CPOL=0,CPAH=0)
        .spics_io_num = SPI_CS_PIN,  // Chip select pin
        .queue_size = 1,
    };

    // Initialize the SPI bus with DMA on channel 1
    esp_err_t ret = spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);  // DMA channel 1
    ESP_ERROR_CHECK(ret);

    // Attach the device to the SPI bus
    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
}

void spi_send_receive(uint8_t* send_data, uint8_t* recv_data, size_t length) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = length * 8;   // Data length in bits
    t.tx_buffer = send_data;
    t.rx_buffer = recv_data;

    // Transmit and receive data using DMA
    esp_err_t ret = spi_device_transmit(spi, &t);
    ESP_ERROR_CHECK(ret);
}

void app_main(void) 
{
    // Initialize SPI with DMA
    spi_master_init();

    // Data to be sent
    uint8_t send_data[] = {0xAA, 0xBB, 0xCC};
    uint8_t recv_data[sizeof(send_data)] = {0};

    while (1) 
    {
        ESP_LOGI(TAG, "Sending data over SPI...");
        spi_send_receive(send_data, recv_data, sizeof(send_data));

        ESP_LOGI(TAG, "Received data:");
        for (int i = 0; i < sizeof(recv_data); i++) {
            ESP_LOGI(TAG, "0x%02X", recv_data[i]);
        }

        vTaskDelay((2000)/portTICK_PERIOD_MS);  // Delay for 2 second
    }
}
