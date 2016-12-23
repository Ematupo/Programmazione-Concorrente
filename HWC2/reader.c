#include "reader.h"
#include "reader_list.h"
#include <string.h>

reader_t* reader_init(char name[], int processing_time, int size){
		reader_t* reader=(reader_t*)malloc(sizeof(reader_t));
    	reader->name = strdup(name);
		reader->reader_buffer=reader_buffer_init(size);
		reader->processing_time= processing_time;
		reader->flag=0;
		return reader;
}


void reader_destroy(reader_t* reader){
	if(removeReader(reader)){
		reader_buffer_destroy(reader->reader_buffer);
		free(reader);
	}
}

void* reader_consume(reader_t* reader){
	msg_t* msg= reader_buffer_read(reader->reader_buffer);
	while (msg->content!=NULL){		// finchè il messaggio è diverso da POISON PILL
		printf("%s ha ricevuto il messaggio: %s\n", reader->name, (char*)msg->content);	// stampo
		sleep(reader->processing_time);	// mi fermo per un tempo pari al processing time
		msg_destroy(msg);	// distruggo il messaggio ricevuto 
		msg = reader_buffer_read(reader->reader_buffer);	// leggo il prossimo messaggio
	}
	reader_destroy(reader);	// invoco reader destroy (che rimuove il reader dalla reader list e distrugge il reader)
	pthread_exit(NULL);
}
