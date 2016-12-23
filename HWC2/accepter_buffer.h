#ifndef ACCEPTER_BUFFER_H_
#define ACCEPTER_BUFFER_H_

#include "buffer.h"
#include "poison_pill.h"
#include <stdlib.h>

buffer_t* accepter_buffer;

/* inizializzazione e distruzione di un accepter buffer */
void accepter_buffer_init(int size);
void accepter_buffer_destroy();

/* inserimento di un messaggio nell'accepter buffer */ 
void* accepter_buffer_insert(void* message);
void accepter_buffer_insert_poison_pill();
msg_t* accepter_buffer_read();

#endif