#include "reader_buffer.h"

reader_buffer_t* reader_buffer_init(int size){
	reader_buffer_t* new_r_buffer=malloc(sizeof(reader_buffer_t));
	new_r_buffer->buffer=buffer_init(size);		
	return new_r_buffer;
}


void reader_buffer_destroy(reader_buffer_t* r_buffer){
	buffer_destroy(r_buffer->buffer);
	free(r_buffer);

}

msg_t* reader_buffer_insert(reader_buffer_t* r_buffer, msg_t* msg){
	return put_non_bloccante(r_buffer->buffer, msg);
}

void reader_buffer_insert_poison_pill(reader_buffer_t* r_buffer){
	msg_t* temp = put_bloccante(r_buffer->buffer, POISON_PILL);
	msg_destroy(temp);
}

msg_t* reader_buffer_read(reader_buffer_t* r_buffer){
	return get_bloccante(r_buffer->buffer);
}

