#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TIMER_GROUP TIMER_GROUP_0
#define TIMER_INDEX TIMER_0
#define GPIO_CAPTURE_PIN GPIO_NUM_4  // Pin to capture the signal
#define TIMER_DIVIDER 40            // Divide 80 MHz clock to 1 MHz (1us resolution)
#define TIMER_SCALE (80000000 / TIMER_DIVIDER)  // 2 MHz timer frequency

static const char* TAG = "FrequencyCapture";

// Variables to store timer values for consecutive edges
volatile uint64_t first_edge_time = 0;
volatile uint64_t second_edge_time = 0;
volatile bool first_edge_captured = false;
volatile bool data_ready = false;
volatile bool log_first_edge = false;
volatile bool log_second_edge = false;

void IRAM_ATTR capture_isr(void* arg) {
    // Capture the timer value when the rising edge is detected
    uint64_t timer_value;
    timer_get_counter_value(TIMER_GROUP, TIMER_INDEX, &timer_value);
    
    if (!first_edge_captured) {
        first_edge_time = timer_value;
        first_edge_captured = true;
        log_first_edge = true;  // Flag to log first edge outside the ISR
    } else {
        second_edge_time = timer_value;
        first_edge_captured = false;
        log_second_edge = true;  // Flag to log second edge outside the ISR
        data_ready = true;  // Signal to main task that data is ready
    }

    // Clear the interrupt status
    timer_group_clr_intr_status_in_isr(TIMER_GROUP, TIMER_INDEX);
}

void configure_input_capture() {
    // Timer configuration (count up, no auto-reload)
    timer_config_t timer_config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_DIS,
        .auto_reload = false,
    };
    timer_init(TIMER_GROUP, TIMER_INDEX, &timer_config);
    timer_start(TIMER_GROUP, TIMER_INDEX);

    // Configure GPIO for input capture
    gpio_set_direction(GPIO_CAPTURE_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_CAPTURE_PIN, GPIO_PULLUP_ONLY);
    
    // Configure GPIO to generate interrupts on the rising edge
    gpio_set_intr_type(GPIO_CAPTURE_PIN, GPIO_INTR_POSEDGE);

    // Register the ISR for input capture
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_CAPTURE_PIN, capture_isr, NULL);
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting Frequency Capture Example");

    // Initialize the input capture timer and GPIO
    configure_input_capture();
    
    // Main loop
    while (1) {
        // Log first and second edge outside of ISR
        if (log_first_edge) {
            log_first_edge = false;  // Reset flag
            ESP_LOGI(TAG, "First edge captured at: %llu us", first_edge_time);
        }
        
        if (log_second_edge) {
            log_second_edge = false;  // Reset flag
            ESP_LOGI(TAG, "Second edge captured at: %llu us", second_edge_time);
        }

        // Calculate and log frequency if data is ready
        if (data_ready) {
            data_ready = false;  // Reset flag

            uint64_t period_us = second_edge_time - first_edge_time;  // Period in microseconds
            double period_sec = period_us / 1000000.0;   // into second
            double frequency_hz = 2000000.0 / period_us;  // Frequency in Hz

            ESP_LOGI(TAG, "Period: %.2f sec, Frequency: %.2f Hz", period_sec, frequency_hz);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);  // Short delay to avoid hogging the CPU
    }
}
