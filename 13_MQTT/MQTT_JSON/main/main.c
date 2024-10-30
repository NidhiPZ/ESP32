#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "driver/gpio.h"
#include "cJSON.h" // Include cJSON library

#define SSID "Conference"
#define PASS "DNKASDFGHJKL"
#define LED_PIN GPIO_NUM_2 // Define your LED pin here

static const char *TAG = "MQTT";

// Function to create JSON object for publishing
void publish_json_message(esp_mqtt_client_handle_t client)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "device", "ESP32");
    cJSON_AddStringToObject(json, "status", "active");

    // Convert JSON to string
    char *json_str = cJSON_PrintUnformatted(json);

    // Publish the JSON string
    esp_mqtt_client_publish(client, "my_topic", json_str, 0, 1, 0);

    // Free the JSON object and string
    cJSON_Delete(json);
    free(json_str);
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi connecting ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi connected ... \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection ... \n");
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi got IP ... \n\n");
        break;
    default:
        break;
    }
}

void wifi_connection()
{
    // 1 - Wi-Fi/LwIP Init Phase
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);

    // 2 - Wi-Fi Configuration Phase
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);

    // 3 - Wi-Fi Start Phase
    esp_wifi_start();

    // 4- Wi-Fi Connect Phase
    esp_wifi_connect();
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        esp_mqtt_client_subscribe(client, "my_topic", 0);

        // Publish JSON message upon connection
        publish_json_message(client);
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);

        // Parse JSON data received
    cJSON *json = cJSON_Parse(event->data);
    if (json != NULL)
    {
        cJSON *command = cJSON_GetObjectItem(json, "command");
        if (command != NULL && cJSON_IsString(command))
        {
            if (strcmp(command->valuestring, "on") == 0)
            {
                gpio_set_level(LED_PIN, 1); // Turn on LED
                ESP_LOGI(TAG, "LED turned ON");
                // Send confirmation message only for "on"
                esp_mqtt_client_publish(client, "my_topic", "LED ON SUCCESSFULLY", 0, 1, 0);
            }
            else if (strcmp(command->valuestring, "off") == 0)
            {
                gpio_set_level(LED_PIN, 0); // Turn off LED
                 ESP_LOGI(TAG, "LED turned OFF");
                esp_mqtt_client_publish(client, "my_topic", "LED OFF SUCCESSFULLY", 0, 1, 0);
            }
        }
        cJSON_Delete(json); // Free the JSON object
    }
    

        break;

    // Handle other MQTT events...
    default:
        break;
    }
    return ESP_OK;
}
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    mqtt_event_handler_cb(event_data);
}

static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://mqtt.eclipseprojects.io",
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void app_main(void)
{
   esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Configure LED GPIO
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Connect to WiFi
    wifi_connection();

    // Delay to allow WiFi to connect
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    printf("WIFI was initiated ...........\n");

    // Start MQTT client
    mqtt_app_start();
}
