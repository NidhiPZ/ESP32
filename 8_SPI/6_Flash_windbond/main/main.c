#include <stdio.h>
#include <stdint.h>

// Define flash operation status
#define FLASH_OK 0
#define FLASH_ERROR -1

// Define the qspi_erase_len enumeration
typedef enum {
    QSPI_ERASE_LEN_4KB = 1,
    QSPI_ERASE_LEN_64KB,
    QSPI_ERASE_LEN_ALL
} qspi_erase_len;

// Define a structure to store data
typedef struct {
    uint8_t ID;
    char Name[20];
    float Value;
} DataStruct;

// Function prototypes
uint32_t MX25R3235F_Flash_Init(void);
uint32_t MX25R3235F_Flash_DeInit(void);
uint32_t MX25R3235F_Flash_Write(void *p_tx_buffer, size_t tx_buffer_length, uint32_t dst_address);
uint32_t MX25R3235F_Flash_Read(void *p_rx_buffer, size_t rx_buffer_length, uint32_t src_address);
uint32_t MX25R3235F_Flash_Erase(uint32_t start_address, qspi_erase_len len);

// Function to initialize External Flash Memory (MX25R3235F)
uint32_t MX25R3235F_Flash_Init(void) {
    // Here you would initialize the QSPI interface
    printf("Flash Initialized\n");
    return FLASH_OK;  // Return success for now
}

// Function to Deinitialize External Flash Memory (MX25R3235F)
uint32_t MX25R3235F_Flash_DeInit(void) {
    // Here you would deinitialize the QSPI interface
    printf("Flash Deinitialized\n");
    return FLASH_OK;  // Return success for now
}

// Function to erase blocks of External Flash Memory
uint32_t MX25R3235F_Flash_Erase(uint32_t start_address, qspi_erase_len len) {
    // Perform erase operation based on the specified length
    switch (len) {
        case QSPI_ERASE_LEN_4KB:
            printf("Erasing 4KB sector at address: %lX\n", start_address);
            break;
        case QSPI_ERASE_LEN_64KB:
            printf("Erasing 64KB block at address: %lX\n", start_address);
            break;
        case QSPI_ERASE_LEN_ALL:
            printf("Erasing entire chip\n");
            break;
        default:
            printf("Invalid erase length\n");
            return FLASH_ERROR;
    }
    return FLASH_OK;  // Return success for now
}

// Function to write data to External Flash Memory
uint32_t MX25R3235F_Flash_Write(void *p_tx_buffer, size_t tx_buffer_length, uint32_t dst_address) {
    // Simulate writing data to flash
    printf("Writing %zu bytes to address: %lX\n", tx_buffer_length, dst_address);
    return FLASH_OK;  // Return success for now
}

// Function to read data from External Flash Memory
uint32_t MX25R3235F_Flash_Read(void *p_rx_buffer, size_t rx_buffer_length, uint32_t src_address) {
    // Simulate reading data from flash
    printf("Reading %zu bytes from address: %lX\n", rx_buffer_length, src_address);

    // Simulate populating the read buffer with example data
    DataStruct *data = (DataStruct *)p_rx_buffer;
    data->ID = 1;
    snprintf(data->Name, sizeof(data->Name), "Sensor1");
    data->Value = 1.57;

    return FLASH_OK;  // Return success for now
}

// Main function
void app_main(void) {
    uint32_t address = 0x00000000;
    DataStruct data;  // Data buffer to hold the structure

    // Initialize Flash
    if (MX25R3235F_Flash_Init() == FLASH_OK) {
        // Erase Flash
        MX25R3235F_Flash_Erase(address, QSPI_ERASE_LEN_4KB);

        // Write data to Flash (for example, using the same structure)
        MX25R3235F_Flash_Write(&data, sizeof(data), address);

        // Read data from Flash
        if (MX25R3235F_Flash_Read(&data, sizeof(data), address) == FLASH_OK) {
            // Print the data read from flash
            printf("Loaded Data: ID=%d, Name=%s, Value=%.2f\n", data.ID, data.Name, data.Value);
        } else {
            printf("Error reading data\n");
        }
    }

    // Deinitialize Flash
    MX25R3235F_Flash_DeInit();
}
