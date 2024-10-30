#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/timer.h"
#include "esp_log.h"
#include "esp_types.h"
#include "soc/timer_group_struct.h"

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5

static const char *TAG = "SPI_Interrupt_ISR";
spi_device_handle_t spi; // SPI device handle
volatile bool isr_flag = false; // Flag to indicate ISR occurred

// ISR handler
void IRAM_ATTR timer_isr(void *arg) {
    isr_flag = true; // Set flag to notify task that interrupt occurred
   TIMERG0.int_clr_timers.t0_int_clr = 1; // Clear the interrupt
}

// Function to initialize a hardware timer and register an ISR
void init_timer_isr() {
    timer_config_t config = {
        .divider = 80, // Timer frequency divider (1us per tick)
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true,
    };

    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 4000000); // 1 second alarm
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
    timer_start(TIMER_GROUP_0, TIMER_0);
}

// Function to initialize SPI
void init_spi() {
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000, // 1 MHz
        .mode = 0,                 // SPI mode 0
        .spics_io_num = PIN_NUM_CS, // CS pin
        .queue_size = 1,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi));

    ESP_LOGI(TAG, "SPI initialized");
}

// Function to perform SPI transaction
void perform_spi_transaction() {
    uint8_t data_to_send = 0xAA; // Example data

    spi_transaction_t trans = {
        .flags = 0,
        .length = 8 * sizeof(uint8_t), // Number of bits in a transaction
        .tx_buffer = &data_to_send,    // Data to send
        .rx_buffer = NULL,             // No receive data
    };

    ESP_ERROR_CHECK(spi_device_transmit(spi, &trans));
    ESP_LOGI(TAG, "Sent SPI data: 0x%X", data_to_send);
}

// FreeRTOS task to perform SPI transactions and handle interrupt notification
void spi_task(void *arg) {
    while (1) {
        perform_spi_transaction(); // Perform SPI transaction

        if (isr_flag) {
            // Handle the interrupt outside the ISR
            ESP_LOGI(TAG, "Interrupt triggered outside ISR!");

            // Clear the flag
            isr_flag = false;
        }

        // Wait for a second before performing the next transaction
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main() {
    // Initialize SPI
    init_spi();

    // Initialize and start the timer with ISR
    init_timer_isr();

    // Create a FreeRTOS task to handle SPI transactions
    xTaskCreate(spi_task, "spi_task", 2048, NULL, 10, NULL);
}
