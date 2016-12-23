#ifndef PROVIDER_BUFFER_H_
#define PROVIDER_BUFFER_H_

#include "buffer.h"
#include "poison_pill.h"

buffer_t* provider_buffer;

/*inizializzazione e distruzione del provider buffer */
void provider_buffer_init(int size);
void provider_buffer_destroy();

/*inserimento di un messaggio nel provider buffer */
void provider_buffer_insert(msg_t* message);

/*estrazione di un messaggio dal provider buffer */
msg_t* provider_buffer_read();

#endif