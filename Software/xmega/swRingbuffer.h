#ifndef _LALEMA_SW_RINGBUFFER_H_
#define _LALEMA_SW_RINGBUFFER_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct sw_ringbuffer_t {
	uint8_t *buffer;
	uint16_t buffer_len;
	uint16_t read_pos;
	uint16_t write_pos;
} sw_ringbuffer_t;

typedef struct sw_ringbuffer_external_job_t {
	uint8_t *start;
	uint16_t len;
	bool more_data_available;
} sw_ringbuffer_external_job_t;

void sw_ringbuffer_init(sw_ringbuffer_t *sw_ringbuffer, uint8_t buffer[], uint16_t buffer_len);

uint16_t sw_ringbuffer_availabe(sw_ringbuffer_t *sw_ringbuffer);
uint16_t sw_ringbuffer_read(sw_ringbuffer_t *sw_ringbuffer, uint8_t *data, uint16_t maxlen);
sw_ringbuffer_external_job_t sw_ringbuffer_read_external(sw_ringbuffer_t *sw_ringbuffer, uint16_t maxlen);
void sw_ringbuffer_write(sw_ringbuffer_t *sw_ringbuffer, uint8_t *data, uint16_t len);
sw_ringbuffer_external_job_t sw_ringbuffer_write_external(sw_ringbuffer_t *sw_ringbuffer, uint16_t len);

#endif


