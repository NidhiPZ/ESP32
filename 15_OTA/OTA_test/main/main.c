#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_ota_ops.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"

#define WIFI_SSID "your_wifi_ssid"      // Replace with your Wi-Fi SSID
#define WIFI_PASS "your_wifi_password"   // Replace with your Wi-Fi Password
#define OTA_TAG "OTA"

static esp_err_t ota_update_handler(httpd_req_t *req) {
    esp_ota_handle_t ota_handle;
    const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
    esp_err_t err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE(OTA_TAG, "OTA Begin failed");
        return ESP_FAIL;
    }

    ESP_LOGI(OTA_TAG, "Starting OTA update...");
    char ota_buffer[1024];
    int received;
    int total_bytes = 0;
    int total_received = 0;
    bool header_checked = false;

    // Check content length
    char content_length_str[16];
    if (httpd_req_get_hdr_value_str(req, "Content-Length", content_length_str, sizeof(content_length_str)) > 0) {
        total_bytes = atoi(content_length_str);
        ESP_LOGI(OTA_TAG, "Content-Length: %d", total_bytes);
    }

    while ((received = httpd_req_recv(req, ota_buffer, sizeof(ota_buffer))) > 0) {
        if (!header_checked) {
            ESP_LOGI(OTA_TAG, "First bytes: %02X %02X %02X %02X", 
                     ota_buffer[0], ota_buffer[1], ota_buffer[2], ota_buffer[3]);
            if (ota_buffer[0] != 0xE9) {
                ESP_LOGE(OTA_TAG, "Invalid OTA file: Expected magic byte 0xE9 but got %02X", ota_buffer[0]);
                esp_ota_end(ota_handle);
                return ESP_FAIL;
            }
            header_checked = true;
        }

        if (esp_ota_write(ota_handle, (const void *)ota_buffer, received) != ESP_OK) {
            ESP_LOGE(OTA_TAG, "OTA Write failed");
            esp_ota_end(ota_handle);
            return ESP_FAIL;
        }

        // Update the total received bytes
        total_received += received;

        // Report progress
        if (total_bytes > 0) {
            int progress = (total_received * 100) / total_bytes;
            ESP_LOGI(OTA_TAG, "Progress: %d%%", progress);
        }
    }

    if (received < 0) {
        ESP_LOGE(OTA_TAG, "Error receiving OTA file");
        esp_ota_end(ota_handle);
        return ESP_FAIL;
    }

    if (esp_ota_end(ota_handle) != ESP_OK) {
        ESP_LOGE(OTA_TAG, "OTA End failed");
        return ESP_FAIL;
    }

    if (esp_ota_set_boot_partition(update_partition) != ESP_OK) {
        ESP_LOGE(OTA_TAG, "Set boot partition failed");
        return ESP_FAIL;
    }

    ESP_LOGI(OTA_TAG, "OTA update successful! Rebooting...");
    esp_restart();
    return ESP_OK;
}

static esp_err_t index_get_handler(httpd_req_t *req) {
    const char *resp = 
        "<html>"
        "<head><title>ESP32 OTA Update</title></head>"
        "<body>"
        "<h1>ESP32 OTA Update</h1>"
        "<form id='uploadForm' method='POST' enctype='multipart/form-data' action='/ota'>"
        "<input type='file' name='file' /><br/>"
        "<input type='submit' value='Upload' />"
        "</form>"
        "<div id='progress' style='margin-top:20px;'>"
        "<span id='prg'>Progress: 0%</span>"
        "</div>"
        "<script>"
        "document.getElementById('uploadForm').onsubmit = function(event) {"
        "    event.preventDefault();"
        "    var xhr = new XMLHttpRequest();"
        "    var formData = new FormData(this);"
        "    xhr.open('POST', this.action, true);"
        "    xhr.upload.addEventListener('progress', function(evt) {"
        "        if (evt.lengthComputable) {"
        "            var per = evt.loaded / evt.total;"
        "            document.getElementById('prg').innerHTML = 'Progress: ' + Math.round(per * 100) + '%';"
        "        }"
        "    });"
        "    xhr.onload = function() {"
        "        if (xhr.status === 200) {"
        "            alert('Update successful! Rebooting...');"
        "        } else {"
        "            alert('Update failed!');"
        "        }"
        "    };"
        "    xhr.send(formData);"
        "};"
        "</script>"
        "</body>"
        "</html>";
    
    httpd_resp_send(req, resp, strlen(resp));
    return ESP_OK;
}

httpd_uri_t uri_index = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_get_handler,
    .user_ctx  = NULL
};

httpd_uri_t uri_ota = {
    .uri       = "/ota",
    .method    = HTTP_POST,
    .handler   = ota_update_handler,
    .user_ctx  = NULL
};

void start_webserver(void) {
    httpd_handle_t server = NULL;

    httpd_config_t config = {
        .ctrl_port = 80,
        .stack_size = 8192,
        .ctrl_port = 32768,
        .task_priority = 5,
        .task_caps = 0,
                                                
    };

    // Start the httpd server
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_index);
        httpd_register_uri_handler(server, &uri_ota);
    }
}

void wifi_init(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi
    esp_netif_init();
    esp_event_loop_create_default();

    esp_netif_create_default_wifi_sta();
    
    wifi_config_t wifi_config = {};
    strcpy((char *)wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char *)wifi_config.sta.password, WIFI_PASS);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(OTA_TAG, "Connecting to Wi-Fi...");
}

void app_main() {
    // Initialize Wi-Fi
    wifi_init();

    // Start the web server
    start_webserver();
}
