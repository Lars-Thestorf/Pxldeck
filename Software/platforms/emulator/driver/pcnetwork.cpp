#include <HLM_network.h>

#include "pcnetwork.h"

#include <SDL2/SDL_net.h>

#define UDP_PORT 4685
//#define UDP_SEND_PORT 41288
#define UDP_SEND_PORT 0x4D12
//#define UDP_SEND_PORT 7485

typedef struct remote_device_t {
    uint32_t ip;
    uint16_t port;
} remote_device_t;

typedef struct network_mem_t {
    bool inited;
    UDPsocket socket;
    listen_callback_t *listen_callback;
    UDPpacket *packet;
    remote_device_t devices[256];
    uint8_t device_count;
} network_mem_t;

network_mem_t mem = {
    .inited = false,
    .device_count = 0
};

void network_init(const listen_callback_t *listen_callback) {
    printf("net init: %d\n", SDLNet_Init());
    mem.socket = SDLNet_UDP_Open(UDP_PORT);
    mem.listen_callback = listen_callback;
    mem.packet = SDLNet_AllocPacket(2048);
    mem.inited = true;
}

void network_broadcast(uint8_t *msg, size_t len) {
    mem.packet->address.host = 0xFFFFFFFF;
    mem.packet->address.port = UDP_SEND_PORT;
    memcpy(mem.packet->data, msg, len);
    mem.packet->len = len;
    printf("return %d\n", SDLNet_UDP_Send(mem.socket, -1, mem.packet));
}

void network_unicast(device_id_t device, uint8_t *msg, size_t len) {
    mem.packet->address.host = mem.devices[device].ip;
    mem.packet->address.port = mem.devices[device].port;
    memcpy(mem.packet->data, msg, len);
    mem.packet->len = len;
    SDLNet_UDP_Send(mem.socket, -1, mem.packet);
}

void network_uninit() {
    mem.inited = false;
    SDLNet_UDP_Close(mem.socket);
    SDLNet_FreePacket(mem.packet);
    SDLNet_Quit();
}

void network_worker() {
    if (mem.inited) {
        while (SDLNet_UDP_Recv(mem.socket, mem.packet)) {
            printf("Paket mit %d bytes von %d:%d\n", mem.packet->len, mem.packet->address.host, mem.packet->address.port);
            bool found = false;
            uint8_t device = -1;
            for (uint8_t i = 0; i < mem.device_count && !found; i++) {
                device = i;
                if (mem.devices[i].ip == mem.packet->address.host && mem.devices[i].port == mem.packet->address.port) {
                    found = true;
                }
            }
            if (!found) {
                device++;
                mem.devices[device].ip = mem.packet->address.host;
                mem.devices[device].port = mem.packet->address.port;
                mem.device_count++;
            }
            mem.listen_callback(device, mem.packet->data, mem.packet->len);
        }
    }
}