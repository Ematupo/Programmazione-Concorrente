#include "dispatcher.h"

void* dispatch_messages(){
	while(1){
		msg_t* msg = provider_buffer_read();	// se la reader list è vuota il dispatcher si limita a consumare il messaggio senza inoltrarlo
		//if (msg==&POISON_PILL_MSG) 
		if (msg->content==0)	break;	// se il content è nullo vuol dire che il messaggio è il poison pill
		if (!reader_list_isEmpty()) {
			insert_message_broadcast(msg);		// se il messaggio non è nullo e non è il poison pill allora mando lo invio a tutti i reader
			sleep(1);
			remove_slow_readers();		// rimuove i reader troppo lenti
		}
	}
	// si esce dal ciclo quando il messaggio ricevuto è poison pill
	if (reader_list_isEmpty()){		// se la reader list è vuota non c'è alcun reader rimasto attivo quindi il dispatcher termina
		pthread_exit(NULL);
	}
	else insert_poison_pill_broadcast();	// se la reader list non è vuota invio a tutti i reader il poison pill
		pthread_exit(NULL);
}
 


/* invia il poison pill a tutti i reader nella reader list */
void insert_poison_pill_broadcast(){		
	pthread_mutex_lock(&(reader_list_access));
	iterator=iterator_init(reader_list);
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		reader_buffer_insert_poison_pill(reader->reader_buffer);
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_access));
}

/* invia un messaggio a tutti i reader nella reader list */
void insert_message_broadcast(msg_t* msg){
	pthread_mutex_lock(&(reader_list_access));
	iterator=iterator_init(reader_list);
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		msg_t* insert = reader_buffer_insert(reader->reader_buffer, msg);
		if (insert == BUFFER_ERROR){
			reader->flag=1;
		}
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_access));
}

/* rimuove i reader troppo lenti */
void remove_slow_readers(){
	pthread_mutex_lock(&(reader_list_access));
	iterator=iterator_init(reader_list);
	while (hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		if (reader->flag==1){
			reader_buffer_insert_poison_pill(reader->reader_buffer);
		}
	}
	pthread_mutex_unlock(&(reader_list_access));
}




