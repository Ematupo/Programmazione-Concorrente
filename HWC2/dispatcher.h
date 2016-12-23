#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "provider_buffer.h"
#include "reader_list.h"
#include <unistd.h>

/* metodo per gestire l'inoltro dei messaggi del provider ai reader da parte del dispatcher */
void* dispatch_messages();

/* metodi di supporto */
void insert_poison_pill_broadcast();
void insert_message_broadcast(msg_t* msg);
void remove_slow_readers();


#endif