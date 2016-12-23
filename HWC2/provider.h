#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <pthread.h>
#include <unistd.h>
#include "poison_pill.h"
#include "msg.h"
#include "provider_buffer.h"

/* inserisce un array di messaggi nel provider buffer */
void* insert_messages_prov_buffer(void* arguments);

#endif
