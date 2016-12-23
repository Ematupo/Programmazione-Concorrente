#ifndef READER_H_
#define READER_H_

#include "reader_buffer.h"
#include "poison_pill.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


/* struttura dati reader */
typedef struct reader{
	char* name;
	int processing_time;
	reader_buffer_t* reader_buffer;
	int flag;
	pthread_t thread;
} reader_t;

/* inizializzazione e distruzione reader */
reader_t* reader_init(char name[], int processing_time, int size);
void reader_destroy(reader_t* reader);

/* funzione per il consumo dei messaggi da parte di un reader*/
void* reader_consume(reader_t* reader);
#endif