#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

// GPIO pin for onboard LED
#define LED_GPIO GPIO_NUM_2
// GPIO pin for the button (reset button)
#define BUTTON_GPIO GPIO_NUM_0

// Queue handle for GPIO events
static QueueHandle_t gpio_evt_queue = NULL;

// ISR handler for GPIO interrupt
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    // Send the GPIO number that triggered the interrupt to the queue
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

// Task to handle the GPIO interrupt event
static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    for(;;) {
        // Wait for a GPIO interrupt event
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) 
        {
            // Turn on the LED
            gpio_set_level(LED_GPIO, 1);
            printf("GPIO[%ld] intr, LED ON\n", io_num);

            // Wait for 5 seconds
            vTaskDelay(5000 / portTICK_PERIOD_MS);

            // Turn off the LED after the delay
            gpio_set_level(LED_GPIO, 0);
            printf("GPIO[%ld] LED OFF\n", io_num);
        }
    }
}

void app_main(void)
{
    // Initialize the GPIO for the LED
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);  // Start with LED off

    // Initialize the GPIO for the button
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    //gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY); // Use pull-up resistor
    gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_ANYEDGE); // Trigger on falling edge

    // Create a queue to handle GPIO events
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    
    // Start a task to handle the GPIO interrupt
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
    
    // Install the GPIO ISR handler
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL3);
    gpio_isr_handler_add(BUTTON_GPIO, gpio_isr_handler, (void*) BUTTON_GPIO);
    
    // Print a message indicating the setup is complete
    printf("GPIO interrupt example setup complete.\n");
}
