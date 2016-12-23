#include "accepter.h"
#include "provider.h"
#include "reader.h"
#include "dispatcher.h"
#include "reader_list.h"

/* funzione di supporto per inviare richieste concorrenti all'accepter buffer */
void create_requests(pthread_t requester[], msg_t* messages[], int size){
	int i;
	for(i=0;i<size;i++){
		pthread_create(&requester[i],NULL,(void*)accepter_buffer_insert,messages[i]);
	}
	for(i=0; i<size;i++){
		pthread_join(requester[i],NULL);
	}
}

/* funzione di supporto che verifica che tutti i thread nella reader list sono terminati. NON è thread safe perchè altrimenti sarebbe impossibile per i flussi reader auto-rimuoversi dalla lista*/
void reader_list_join_threads(){
	iterator=iterator_init(reader_list);
	while (hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		if (reader!=NULL) pthread_join(reader->thread,NULL);
	}
	iterator_destroy(iterator);
}


int main(){	

	pthread_t provider;
	pthread_t accepter;
	pthread_t dispatcher;
	pthread_t requester[4];
	int size=4;

	printf("Inizializzazione delle strutture dati\n");

	msg_t* msg1= msg_init("1");
	msg_t* msg2= msg_init("2");
	msg_t* msg3= msg_init("3");
	msg_t* msg4= msg_init("4");
	args* param = malloc(sizeof(args));	  // args è una struttura di supporto per utilizzare array di messaggi nelle funzioni invocate dai thread concorrenti, è definita in msg.h
	param->messages=malloc(sizeof(msg_t*)*4);
	param->size =4;
	param->messages[0]= msg1;
	param->messages[1]= msg2;
	param->messages[2]= msg3;
	param->messages[3]= msg4;
	provider_buffer_init(5);
	accepter_buffer_init(5);
	reader_list_init();

	printf("Inizio esecuzione dell'accepter\n");
	pthread_create(&accepter,NULL,(void*)accepter_start,NULL);

	printf("Invio delle richieste\n");
	create_requests(requester,param->messages,size); // invio un array di messaggi di richieste all'accepter (il contenuto non è importante)
	
	printf("Inizio esecuzione del dispatcher\n");
	pthread_create(&dispatcher,NULL,(void*)dispatch_messages,NULL);

	printf("Invio dei messaggi da parte del provider\n");
	pthread_create(&provider, NULL,(void*)insert_messages_prov_buffer,param);		// creo il thread provider che invierà dei messaggi al provider buffer
	pthread_join(provider,NULL);
	accepter_buffer_insert_poison_pill();

	printf("Attesa della terminazione dei flussi creati \n");
	pthread_join(accepter,NULL);
	pthread_join(dispatcher,NULL);
	sleep(20);
	reader_list_join_threads();

	printf("Cancellazione delle strutture dati\n");

	free(param->messages);
	free(param);
	reader_list_destroy();
	provider_buffer_destroy();
	accepter_buffer_destroy();	

	printf("FINE \n");

	return 0;

}