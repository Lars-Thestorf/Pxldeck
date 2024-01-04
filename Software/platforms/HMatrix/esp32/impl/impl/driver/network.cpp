#include "HLM_network.h"

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include <string.h>

listen_callback_t* cb;


esp_now_peer_info_t peerInfo;

static uint8_t s_example_broadcast_mac[ESP_NOW_ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static void example_espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    cb(1, data, len);
}

void network_init(listen_callback_t listen_callback) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfghallo = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfghallo) );
    
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    
    ESP_ERROR_CHECK( esp_wifi_start());
    ESP_ERROR_CHECK( esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));
    
    ESP_ERROR_CHECK( esp_now_init() );
    //ESP_ERROR_CHECK( esp_now_register_send_cb(example_espnow_send_cb) );
    ESP_ERROR_CHECK( esp_now_register_recv_cb(example_espnow_recv_cb) );
    cb = listen_callback;

    memcpy(peerInfo.peer_addr, s_example_broadcast_mac, 6);
    peerInfo.channel = 1;  
    peerInfo.encrypt = false;
  
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        printf("Failed to add peer\n");
    }
}
void network_broadcast(uint8_t *msg, size_t len) {
    esp_now_send(s_example_broadcast_mac, msg, len);
    cb(0, msg, len);
}
void network_unicast(device_id_t device, uint8_t *msg, size_t len) {
    esp_now_send(s_example_broadcast_mac, msg, len);
}
void network_uninit() {
    esp_now_deinit();
    esp_wifi_stop();
    esp_wifi_deinit();
}