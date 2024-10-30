#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "driver/gpio.h"

#define SSID "Nid"
#define PASS "11111111"

#define LED_PIN GPIO_NUM_2  // You can change this to the pin your LED is connected to

// Function to handle requests for turning the LED on
static esp_err_t led_on_handler(httpd_req_t *req)
{
    gpio_set_level(LED_PIN, 1);  // Turn LED on
    const char* response = "LED is ON";
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Function to handle requests for turning the LED off
static esp_err_t led_off_handler(httpd_req_t *req)
{
    gpio_set_level(LED_PIN, 0);  // Turn LED off
    const char* response = "LED is OFF";
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Function to initialize the HTTP server
void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    // Start the HTTP server
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Define the URI handlers for turning the LED on and off
        httpd_uri_t led_on_uri = {
            .uri = "/led/on",  // Endpoint for turning the LED on
            .method = HTTP_GET,
            .handler = led_on_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &led_on_uri);

        httpd_uri_t led_off_uri = {
            .uri = "/led/off",  // Endpoint for turning the LED off
            .method = HTTP_GET,
            .handler = led_off_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &led_off_uri);
    }
}

// Function to initialize Wi-Fi
void wifi_connection(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_cfg);

    wifi_config_t wifi_cfg = {
        .sta = {
            .ssid = SSID,  //  Wi-Fi SSID
            .password = PASS  //  Wi-Fi password
        }
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg);
    esp_wifi_start();
    esp_wifi_connect();
}

// Main function
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Configure the GPIO for the LED
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Initialize Wi-Fi connection
    wifi_connection();

    // Start the web server
    start_webserver();
}
