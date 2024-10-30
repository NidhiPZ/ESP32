// TCP Server with WiFi connection communication via Socket

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_check.h"

#include "esp_eth.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_event.h"
#include "esp_system.h"

#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"

#include "nvs_flash.h"
#include "ping/ping_sock.h"
#include "driver/gpio.h"

#define SSID "Conference"
#define PASS "DNKASDFGHJKL"

#define PORT 3333
static const char *TAG = "TCP_SOCKET";

static void tcp_server_task(void *pvParameters)
{
    char addr_str[128];      //To hold the string representation of the client IP address.
    char rx_buffer[128];     //To hold incoming data received from the TCP client.
    char string_data[128];   //To hold the response message that the server will send back.
    char data_to_send[128];  // To prepare the complete HTTP response string to be sent to the client.
    
    //hold the lengths of the data being sent and the response string length.
    int data_len, string_data_len;

    //variables set the TCP keep-alive options
    int keepAlive = 1;
    int keepIdle = 5;
    int keepInterval = 5;
    int keepCount = 3;

    //holds the destination address for the socket.
    struct sockaddr_storage dest_addr;

    int counter = 0;

    
    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);   //server will accept connections on any of its available network interfaces.
    dest_addr_ip4->sin_family = AF_INET;  //indicating that this socket will use IPv4
    dest_addr_ip4->sin_port = htons(PORT);  //converting it from host byte order to network byte 
    
    // Open socket
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0); // 0 for TCP Protocol
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ESP_LOGI(TAG, "Socket created");

    //Binds the socket (listen_sock) to the address specified in dest_addr
    bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    // Listen to the socket
    listen(listen_sock, 1);

    while (1)
    {
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);

        // Accept socket
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0)
        {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
        // Convert ip address to string
        if (source_addr.ss_family == PF_INET)
        {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }

        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);

        // Receive data from the clien
        int len =recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0)
        {
            ESP_LOGE(TAG, "recv failed: errno %d", errno);
        }
        else
        {
            // Null-terminate the received data
            rx_buffer[len] = 0;  
            // Print the received message to the terminal
            ESP_LOGI(TAG, "Received message: %s", rx_buffer);
        }

        strcpy(string_data,"Response from ESP32 Server via Socket connection");
        string_data_len = strlen(string_data);
        sprintf(data_to_send, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", string_data_len);
        strcat(data_to_send, string_data);
        data_len = strlen(data_to_send);

        // Send data via socket
        send(sock, data_to_send, data_len, 0);

        counter++;
        printf("send_reply function number %d was activated\n", counter);
        vTaskDelay(5000 / portTICK_PERIOD_MS);

        shutdown(sock, 0);
        ESP_LOGI(TAG, "Socket shutdown");
        close(sock);
    }
    close(listen_sock);
    vTaskDelete(NULL);
}

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi connecting WIFI_EVENT_STA_START ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi connected WIFI_EVENT_STA_CONNECTED ... \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection WIFI_EVENT_STA_DISCONNECTED ... \n");
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
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = 
    {
        .sta = 
        {
            .ssid = SSID,
            .password = PASS
        }
    };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_connect();
}

void app_main(void)
{
    wifi_connection();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void *)AF_INET, 5, NULL);
}