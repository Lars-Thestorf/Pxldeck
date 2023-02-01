#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lwip/api.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "driver/ledc.h"

#include "string.h"
#include "esp_http_server.h"
#include "server.h"

#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN

static const char *TAG = "Server AP";
httpd_handle_t server_handle = NULL;
int ws_client_sockfds[EXAMPLE_MAX_STA_CONN];

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

void wifi_deinit_softap(){
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
    ESP_ERROR_CHECK(esp_netif_deinit());
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());
}

esp_err_t page_handler(httpd_req_t *req)
{
	char html_start[] = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n\
	<html>\n\
	<head>\n\
		<title>ESP Weblog</title>\n\
			<style>\n\
				body {\n\
					background: #101010\n\
				}\n\
				pre {\n\
					margin: 0\n\
				}\n\
			</style>\n\
		<script>\n\
			var ws;\n\
			function onBodyLoad() {\n\
				ws = new WebSocket(\"ws://\" + window.location.hostname + \"" CONFIG_HTV_WEBLOG_WS_URL "\");\n\
				ws.onmessage = (x) => {\n\
					var div = document.createElement(\"span\");\n\
					div.innerHTML = '<pre>' + x.data + '</pre>';\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;30m\",'</pre><pre style=\"color: black\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;31m\",'</pre><pre style=\"color: red\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;32m\",'</pre><pre style=\"color: green\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;33m\",'</pre><pre style=\"color: brown\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;34m\",'</pre><pre style=\"color: blue\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;35m\",'</pre><pre style=\"color: purple\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;36m\",'</pre><pre style=\"color: cyan\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0;37m\",'</pre><pre style=\"color: white\">');\n\
					div.innerHTML = div.innerHTML.replace(\"\033[0m\",'</pre><pre>');\n\
					document.body.appendChild(div);\n\
					if (document.body.children.length > 1000) {\n\
						document.body.firstChild.remove();\n\
					}\n\
				}\n\
			}\n\
		</script>\n\
	</head>\n\
	<body onload=\"onBodyLoad()\">\n\
	</body>\n";
	httpd_resp_send_chunk(req, html_start, sizeof(html_start) - 1);
	return httpd_resp_send_chunk(req, NULL, 0);
}
void ws_close_fkt(void *ctx) {
	int sockfd = (int)ctx;
	for (uint8_t i = 0; i < EXAMPLE_MAX_STA_CONN; i++)
	{
		if (ws_client_sockfds[i] == sockfd)
		{
			ws_client_sockfds[i] = -1;
		}
	}
	ESP_LOGI(TAG, "connection was closed %d", sockfd);
}

static esp_err_t ws_handler(httpd_req_t *req)
{
	int sockfd = httpd_req_to_sockfd(req);
	if (req->method == HTTP_GET)
	{
		ESP_LOGI(TAG, "Handshake done, the new connection was opened %d", (int)sockfd);
		httpd_sess_set_ctx(server_handle, sockfd, (void*)sockfd, ws_close_fkt);
		for (uint8_t i = 0; i < EXAMPLE_MAX_STA_CONN; i++)
		{
			if (ws_client_sockfds[i] == -1)
			{
				ws_client_sockfds[i] = sockfd;
				break;
			}
		}
		return ESP_OK;
	}

	httpd_ws_frame_t ws_pkt;
	memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
	ws_pkt.type = HTTPD_WS_TYPE_TEXT;
	/* Set max_len = 0 to get the frame len */
	esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
	if (ret != ESP_OK)
	{
		ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
		return ret;
	}
	ESP_LOGI(TAG, "frame len is %d conn: %d", ws_pkt.len, (int)sockfd);
	if (ws_pkt.len)
	{
		/* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
		uint8_t buf[ws_pkt.len + 1];
		memset(buf, 0, sizeof(buf));
		ws_pkt.payload = buf;
		/* Set max_len = ws_pkt.len to get the frame payload */
		ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
		if (ret != ESP_OK)
		{
			ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
			return ret;
		}
		ESP_LOGI(TAG, "Got packet with message: %s", ws_pkt.payload);
	}
	ESP_LOGI(TAG, "Packet type: %d", ws_pkt.type);
	ret = httpd_ws_send_frame(req, &ws_pkt);
	if (ret != ESP_OK)
	{
		ESP_LOGE(TAG, "httpd_ws_send_frame failed with %d", ret);
	}
	return ret;
}

httpd_uri_t page =
{
	.uri       = CONFIG_HTV_WEBLOG_URL,
	.method    = HTTP_GET,
	.handler   = page_handler,
	.user_ctx  = NULL
};
httpd_uri_t websocket = {
	.uri       = CONFIG_HTV_WEBLOG_WS_URL,
	.method    = HTTP_GET,
	.handler   = ws_handler,
	.user_ctx  = NULL,
	.is_websocket = true,
	.handle_ws_control_frames = false,
};

void broadcast_message(void* arg)
{ // arg = buffer_idx
httpd_ws_frame_t ws_pkt;
	memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
	ws_pkt.payload = (uint8_t*)arg;
	ws_pkt.len = strlen(arg);
	ws_pkt.type = HTTPD_WS_TYPE_TEXT;
	for (uint8_t i = 0; i < EXAMPLE_MAX_STA_CONN; i++)
	{
		if (ws_client_sockfds[i] != -1)
		{
			httpd_ws_send_frame_async(server_handle, ws_client_sockfds[i], &ws_pkt);
		}
	}
}


void server_uninit()
{
    wifi_deinit_softap();
    if (server_handle) {
        httpd_stop(server_handle);
    }
}


void server_init()
{
    wifi_init_softap();   
	for(uint8_t i = 0; i < EXAMPLE_MAX_STA_CONN; i++)
	{
		ws_client_sockfds[i] = -1;
	}

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (httpd_start(&server_handle, &config) == ESP_OK) {
	    httpd_register_uri_handler(server_handle, &page);
	    httpd_register_uri_handler(server_handle, &websocket);
    }

	esp_register_shutdown_handler(server_uninit);
}