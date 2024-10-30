#ifndef MX25R3235F
#define MX25R3235F

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

// Define result types
typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR
} flash_status_t;

// Define erase lengths (4KB, full chip)
typedef enum {
    QSPI_ERASE_LEN_4KB = 0,
    QSPI_ERASE_LEN_CHIP
} qspi_erase_len;

// Flash initialization and deinitialization
flash_status_t MX25R3235F_Flash_Init(void);
flash_status_t MX25R3235F_Flash_DeInit(void);

// Flash operations
flash_status_t MX25R3235F_Flash_Erase(uint32_t start_address, qspi_erase_len len);
flash_status_t MX25R3235F_Flash_Write(const void *p_tx_buffer, size_t tx_buffer_length, uint32_t dst_address);
//flash_status_t MX25R3235F_Flash_Read(void *p_rx_buffer, size_t rx_buffer_length, uint32_t src_address);
esp_err_t MX25R3235F_Flash_Read(uint32_t address, uint8_t *data, size_t length);
#endif // MX25R3235F
