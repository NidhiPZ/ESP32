#include <string.h>
#include "mx25r3235f.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define TAG "MX25R3235F"

// Define the SPI bus and device
#define HSPI_HOST    SPI2_HOST
#define PIN_NUM_MISO 13
#define PIN_NUM_MOSI 11
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10

#define FLASH_MEMORY_SIZE (4 * 1024 * 1024) 
// SPI device handle
static spi_device_handle_t spi;

#define CMD_WRITE_ENABLE    0x06
#define CMD_PAGE_PROGRAM    0x02
#define CMD_READ_DATA       0x03
#define CMD_SECTOR_ERASE    0x20
#define CMD_CHIP_ERASE      0xC7

// Helper function to send commands
static esp_err_t spi_send_cmd(uint8_t cmd) {
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd,
    };
    return spi_device_transmit(spi, &t);
}

// Initialize flash (setup SPI)
flash_status_t MX25R3235F_Flash_Init(void) {
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .data2_io_num = GPIO_NUM_14,
        .data3_io_num = GPIO_NUM_9,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096
    };
    
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000,   // 10 MHz
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 7,
    };

    esp_err_t ret = spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) return FLASH_ERROR;

    ret = spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    if (ret != ESP_OK) return FLASH_ERROR;

    ESP_LOGI(TAG, "MX25R3235F initialized");
    return FLASH_OK;
}

// Deinitialize SPI
flash_status_t MX25R3235F_Flash_DeInit(void) {
    spi_bus_remove_device(spi);
    spi_bus_free(HSPI_HOST);
    ESP_LOGI(TAG, "MX25R3235F deinitialized");
    return FLASH_OK;
}

// Enable writing to flash
static esp_err_t write_enable() {
    return spi_send_cmd(CMD_WRITE_ENABLE);
}

// Erase function for sector and chip
flash_status_t MX25R3235F_Flash_Erase(uint32_t start_address, qspi_erase_len len) {
    esp_err_t ret = write_enable();
    if (ret != ESP_OK) return FLASH_ERROR;

    spi_transaction_t t = {
        .length = 32,
        .flags = SPI_TRANS_USE_TXDATA,
    };

    if (len == QSPI_ERASE_LEN_4KB) {
        t.tx_data[0] = CMD_SECTOR_ERASE;
        t.tx_data[1] = (start_address >> 16) & 0xFF;
        t.tx_data[2] = (start_address >> 8) & 0xFF;
        t.tx_data[3] = start_address & 0xFF;
        ret = spi_device_transmit(spi, &t);
    } else if (len == QSPI_ERASE_LEN_CHIP) {
        t.length = 8;
        t.tx_data[0] = CMD_CHIP_ERASE;
        ret = spi_device_transmit(spi, &t);
    }

    return (ret == ESP_OK) ? FLASH_OK : FLASH_ERROR;
}

// Write function
flash_status_t MX25R3235F_Flash_Write(const void *p_tx_buffer, size_t tx_buffer_length, uint32_t dst_address) {
    esp_err_t ret = write_enable();
    if (ret != ESP_OK) return FLASH_ERROR;

    // Ensure the length does not exceed the maximum SPI transfer size
    if (tx_buffer_length > 256) {  // Check if the length exceeds typical max page size
        return FLASH_ERROR;  // Handle error appropriately
    }

    // Prepare the transaction
    spi_transaction_t t = {
        .length = (8 * (tx_buffer_length + 4)),  // 4 bytes for command + address
        .tx_buffer = malloc((tx_buffer_length + 4)), // Allocate memory for tx_buffer
    };
    if (t.tx_buffer == NULL) {
        return FLASH_ERROR; // Handle allocation failure
    }

    // Prepare the command and address in the tx_buffer array
    ((uint8_t*)t.tx_buffer)[0] = CMD_PAGE_PROGRAM;  // Command for page program
    ((uint8_t*)t.tx_buffer)[1] = (dst_address >> 16) & 0xFF;  // Address (MSB)
    ((uint8_t*)t.tx_buffer)[2] = (dst_address >> 8) & 0xFF;   // Address (Middle byte)
    ((uint8_t*)t.tx_buffer)[3] = dst_address & 0xFF;          // Address (LSB)

    // Copy the data to tx_buffer after the command and address
    memcpy((uint8_t*)t.tx_buffer + 4, p_tx_buffer, tx_buffer_length);  // Start after command + address

    // Transmit the SPI transaction
    ret = spi_device_transmit(spi, &t);

    free(t.tx_buffer); // Free allocated memory
    return (ret == ESP_OK) ? FLASH_OK : FLASH_ERROR;
}

// Read function with improved boundary and argument validation
esp_err_t MX25R3235F_Flash_Read(uint32_t address, uint8_t *data, size_t length) {
    if (data == NULL || length == 0) {
        ESP_LOGE(TAG, "Invalid argument: data is NULL or length is 0");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Attempting to read. Address: %lu, Length: %u", address, length);

    // Check that the address and length are within bounds
    if (address >= FLASH_MEMORY_SIZE || (address + length) > FLASH_MEMORY_SIZE) {
        ESP_LOGE(TAG, "Read exceeds flash memory bounds. Address: %lu, Length: %u", address, length);
        return ESP_ERR_INVALID_ARG;
    }

    size_t bytes_read = 0;

    // Continue reading in chunks that respect the 4 MB boundary
    while (bytes_read < length) {
        size_t remaining_bytes = length - bytes_read;

        // Calculate the size without crossing a 4 MB boundary
        uint32_t boundary_limit = 0x400000 - ((address + bytes_read) & 0x3FFFFF);
        size_t chunk_size = remaining_bytes < boundary_limit ? remaining_bytes : boundary_limit;

        // Prepare the read command and address
        uint8_t read_cmd[4] = {
            CMD_READ_DATA,
            (uint8_t)((address + bytes_read) >> 16),
            (uint8_t)((address + bytes_read) >> 8),
            (uint8_t)(address + bytes_read)
        };

        spi_transaction_t trans = {
            .length = chunk_size * 8,
            .tx_buffer = read_cmd,
            .rx_buffer = data + bytes_read,
            .flags = SPI_TRANS_USE_TXDATA
        };

        // Transmit the read command and receive data for this chunk
        esp_err_t ret = spi_device_transmit(spi, &trans);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "SPI read failed at chunk starting at address %lu: %s", address + bytes_read, esp_err_to_name(ret));
            return ret;
        }

        // Update the bytes read so far
        bytes_read += chunk_size;
    }

    ESP_LOGI(TAG, "Read completed successfully. Total bytes read: %u", bytes_read);
    return ESP_OK;
}
