#ifndef READER_BUFFER_H_
#define READER_BUFFER_H_

#include "buffer.h"
#include "poison_pill.h"

/* struttura dati reader buffer */
typedef struct reader_buffer {
	buffer_t* buffer;
} reader_buffer_t;

/* inizializzazione e distruzione di un reader buffer */
reader_buffer_t* reader_buffer_init(int size);
void reader_buffer_destroy(reader_buffer_t* r_buffer);

/* inserimento non bloccante in un reader buffer */
msg_t* reader_buffer_insert(reader_buffer_t* r_buffer, msg_t* msg);

/* lettura bloccante in un reader buffer */
msg_t* reader_buffer_read(reader_buffer_t* r_buffer);

/* inserimento bloccante del poison pill in un reader buffer */
void reader_buffer_insert_poison_pill(reader_buffer_t* r_buffer);

#endif