#ifndef HLM_NETWORK_H_
#define HLM_NETWORK_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef uint8_t device_id_t;
typedef void listen_callback_t(device_id_t device, const uint8_t *msg, size_t len);

void network_init(listen_callback_t listen_callback);
void network_broadcast(uint8_t *msg, size_t len);
void network_unicast(device_id_t device, uint8_t *msg, size_t len);
void network_uninit();

#endif
