#ifndef _LALEMA_SERVER_H_
#define _LALEMA_SERVER_H_

void wifi_setup();
void server_task(void* pvParameters);
void server_handle_task(void* pvParameters);

#endif
