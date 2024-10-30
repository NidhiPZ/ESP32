#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"

static const char *TAG = "PCNT_EXAMPLE";
#define PCNT_HIGH_LIMIT 100
#define PCNT_LOW_LIMIT  -100
#define GPIO_A 2
#define GPIO_B 5

// Callback function when a watch point is reached
static bool on_pcnt_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx) {
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    return (high_task_wakeup == pdTRUE);
}

void app_main(void) {
    // Install and configure PCNT unit
    pcnt_unit_config_t unit_config = {
        .high_limit = PCNT_HIGH_LIMIT,
        .low_limit = PCNT_LOW_LIMIT,
    };
    pcnt_unit_handle_t pcnt_unit = NULL;
    pcnt_new_unit(&unit_config, &pcnt_unit);

    // Set glitch filter
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config);

    // Install PCNT channels for GPIO A and GPIO B
    pcnt_chan_config_t chan_a_config = {.edge_gpio_num = GPIO_A, .level_gpio_num = GPIO_B};
    pcnt_chan_config_t chan_b_config = {.edge_gpio_num = GPIO_B, .level_gpio_num = GPIO_A};
    pcnt_channel_handle_t chan_a, chan_b;
    pcnt_new_channel(pcnt_unit, &chan_a_config, &chan_a);
    pcnt_new_channel(pcnt_unit, &chan_b_config, &chan_b);

    // Set edge and level actions for both channels
    pcnt_channel_set_edge_action(chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    pcnt_channel_set_level_action(chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
    pcnt_channel_set_edge_action(chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE);
    pcnt_channel_set_level_action(chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE);

    // Add watch points
    int watch_points[] = {PCNT_LOW_LIMIT, -50, 0, 50, PCNT_HIGH_LIMIT};
    for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
        pcnt_unit_add_watch_point(pcnt_unit, watch_points[i]);
    }

    // Create a queue for handling watch point events
    QueueHandle_t queue = xQueueCreate(10, sizeof(int));
    pcnt_event_callbacks_t callbacks = {.on_reach = on_pcnt_reach};
    pcnt_unit_register_event_callbacks(pcnt_unit, &callbacks, queue);

    // Enable and start the PCNT unit
    pcnt_unit_enable(pcnt_unit);
    pcnt_unit_clear_count(pcnt_unit);
    pcnt_unit_start(pcnt_unit);

    // Main loop to report pulse count and handle watch point events
    int pulse_count = 0;
    int event_count = 0;
    while (1) {
        if (xQueueReceive(queue, &event_count, pdMS_TO_TICKS(1000))) {
            ESP_LOGI(TAG, "Watch point event, count: %d", event_count);
        } else {
            pcnt_unit_get_count(pcnt_unit, &pulse_count);
            ESP_LOGI(TAG, "Pulse count: %d", pulse_count);
        }
    }
}
