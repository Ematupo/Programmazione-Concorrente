#include "provider.h"

void* insert_messages_prov_buffer(void* arguments){
	args *parameters = (args*)arguments;		// args è una struttura di supporto per gestire array di messaggi (definita in msg.h)
	int i=0;
	int size= parameters->size;
	while(i<size){
		provider_buffer_insert(parameters->messages[i]);	// inserisce uno alla volta i messaggi nel provider_buffer  
		i++;
	}
	provider_buffer_insert(POISON_PILL);	// alla fine inserisce il poison pill e termina
	pthread_exit(NULL);
}