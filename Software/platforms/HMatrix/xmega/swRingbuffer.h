#ifndef _LALEMA_SW_RINGBUFFER_H_
#define _LALEMA_SW_RINGBUFFER_H_

#include <stdbool.h>
#include <stdint.h>

#define datatype_t uint8_t
#define datalen_t uint16_t

typedef struct sw_ringbuffer_t {
	datatype_t *buffer;
	datalen_t buffer_len;
	datalen_t read_pos;
	datalen_t write_pos;
} sw_ringbuffer_t;

typedef struct sw_ringbuffer_external_job_t {
	datatype_t *start;
	datalen_t len;
	bool more_data_available;
} sw_ringbuffer_external_job_t;

void sw_ringbuffer_init(sw_ringbuffer_t *sw_ringbuffer, datatype_t buffer[], datalen_t buffer_len);

datalen_t sw_ringbuffer_available(sw_ringbuffer_t *sw_ringbuffer);
datalen_t sw_ringbuffer_read(sw_ringbuffer_t *sw_ringbuffer, datatype_t *data, datalen_t maxlen);
sw_ringbuffer_external_job_t sw_ringbuffer_read_external(sw_ringbuffer_t *sw_ringbuffer, datalen_t maxlen);
void sw_ringbuffer_write(sw_ringbuffer_t *sw_ringbuffer, datatype_t *data, datalen_t len);
sw_ringbuffer_external_job_t sw_ringbuffer_write_external(sw_ringbuffer_t *sw_ringbuffer, datalen_t len);
void sw_ringbuffer_flush(sw_ringbuffer_t *sw_ringbuffer);

#endif


