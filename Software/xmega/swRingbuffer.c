#include "swRingbuffer.h"

//#include "malloc.h"
#include "string.h"



void sw_ringbuffer_init(sw_ringbuffer_t *sw_ringbuffer, uint8_t buffer[], uint16_t buffer_len) {
	sw_ringbuffer->buffer = buffer;
	sw_ringbuffer->buffer_len = buffer_len;
	sw_ringbuffer->read_pos = 0;
	sw_ringbuffer->write_pos = 0;
}

void sw_ringbuffer_write(sw_ringbuffer_t *sw_ringbuffer, uint8_t *data, uint16_t len){
	size_t copylen = len;
	if (sw_ringbuffer->write_pos > sw_ringbuffer->buffer_len - len)
		copylen = sw_ringbuffer->buffer_len - sw_ringbuffer->write_pos;
	memcpy(&sw_ringbuffer->buffer[sw_ringbuffer->write_pos], data, copylen);
	if (copylen < len)
		memcpy(&sw_ringbuffer->buffer[0], data + copylen, len - copylen);
	sw_ringbuffer->write_pos += len;
	if (sw_ringbuffer->write_pos >= sw_ringbuffer->buffer_len)
		sw_ringbuffer->write_pos -= sw_ringbuffer->buffer_len;
}

uint16_t sw_ringbuffer_availabe(sw_ringbuffer_t *sw_ringbuffer) {
	if (sw_ringbuffer->write_pos >= sw_ringbuffer->read_pos)
		return sw_ringbuffer->write_pos - sw_ringbuffer->read_pos;
	else
		return sw_ringbuffer->buffer_len - sw_ringbuffer->read_pos + sw_ringbuffer->write_pos;
}

uint16_t sw_ringbuffer_read(sw_ringbuffer_t *sw_ringbuffer, uint8_t *data, uint16_t maxlen){
	uint16_t total_len;
	if (sw_ringbuffer->write_pos >= sw_ringbuffer->read_pos){
		total_len = sw_ringbuffer->write_pos - sw_ringbuffer->read_pos;
		if (total_len > maxlen)
			total_len = maxlen;
		memcpy(data, &sw_ringbuffer->buffer[sw_ringbuffer->read_pos], total_len);
	} else {
		uint16_t first_len = sw_ringbuffer->buffer_len - sw_ringbuffer->read_pos;
		if (first_len > maxlen)
			first_len = maxlen;
		memcpy(data, &sw_ringbuffer->buffer[sw_ringbuffer->read_pos], first_len);
		uint16_t second_len = sw_ringbuffer->write_pos;
		if (second_len > maxlen - first_len)
			second_len = maxlen - first_len;
		memcpy(data + first_len, &sw_ringbuffer->buffer[0], sw_ringbuffer->write_pos);
		total_len = first_len + second_len;
	}
	sw_ringbuffer->read_pos += total_len;
	if (sw_ringbuffer->read_pos >= sw_ringbuffer->buffer_len)
		sw_ringbuffer->read_pos -= sw_ringbuffer->buffer_len;
	return total_len;
}

sw_ringbuffer_external_job_t sw_ringbuffer_read_external(sw_ringbuffer_t* sw_ringbuffer, uint16_t maxlen)
{
	sw_ringbuffer_external_job_t job;
	job.start = &sw_ringbuffer->buffer[sw_ringbuffer->read_pos];
	job.more_data_available = false;
	if (sw_ringbuffer->write_pos >= sw_ringbuffer->read_pos) {
		job.len = sw_ringbuffer->write_pos - sw_ringbuffer->read_pos;
	} else {
		job.len = sw_ringbuffer->buffer_len - sw_ringbuffer->read_pos;
		job.more_data_available = true;
	}
	if (job.len > maxlen){
		job.len = maxlen;
		job.more_data_available = true;
	}
	sw_ringbuffer->read_pos += job.len;
	if (sw_ringbuffer->read_pos >= sw_ringbuffer->buffer_len)
		sw_ringbuffer->read_pos -= sw_ringbuffer->buffer_len;
	return job;
}

sw_ringbuffer_external_job_t sw_ringbuffer_write_external(sw_ringbuffer_t* sw_ringbuffer, uint16_t len)
{
	sw_ringbuffer_external_job_t job;
	job.start = &sw_ringbuffer->buffer[sw_ringbuffer->write_pos];
	if (sw_ringbuffer->write_pos > sw_ringbuffer->buffer_len - len) {
		job.len = sw_ringbuffer->write_pos - sw_ringbuffer->buffer_len;
		sw_ringbuffer->write_pos = 0;
		job.more_data_available = true;
	} else {
		job.len = len;
		sw_ringbuffer->write_pos += len;
		job.more_data_available = false;
	}
	return job;
}
