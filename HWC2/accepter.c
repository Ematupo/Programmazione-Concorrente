#include "accepter.h"

/* legge la richiesta dall'accepter_buffer, crea il nuovo reader lo aggiunge alla lista dei reader e lo lancia */ 
void submitRequest(char name[]){
	msg_t* request= accepter_buffer_read();
	if(request->content!=0){			// se il messaggio letto non è poison pill
		int processing_time= 1+ (rand() % 5);		// ad ogni reader è assegnato un tempo di processamento tra 1 e 5 secondi
		reader_t* reader =reader_init(name, processing_time, 5);
		addReader(reader);	// aggiungo il reader alla reader list
		pthread_create(&reader->thread, NULL,(void*)reader_consume, reader);
	}
	else(pthread_exit(NULL));			// se il messaggio letto è poison pill il thread dell'accepter termina
}


/* funzione di supporto, crea ed assegna i nomi ai reader e invoca la funzione submitRequest */
void* accepter_start(){			
	char nome[20];
	int i=0;
	for(i=0; ;i++){
		sprintf(nome, "Reader %d", i);
		submitRequest(nome);
	}
}
