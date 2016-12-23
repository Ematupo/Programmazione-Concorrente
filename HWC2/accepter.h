#ifndef ACCEPTER_H_
#define ACCEPTER_H_

#include "accepter_buffer.h"
#include "reader_list.h"
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

void submitRequest(char name[]);
void* accepter_start();

#endif

