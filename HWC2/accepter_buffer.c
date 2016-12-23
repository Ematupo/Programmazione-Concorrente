#include "accepter_buffer.h"

/* inizializzazione accepter buffer */
void accepter_buffer_init(int size){
	accepter_buffer=buffer_init(size);
}

/* distruzione accepter buffer */
void accepter_buffer_destroy(){
	buffer_destroy(accepter_buffer);
}

/*inserimento bloccante nell'accepter buffer */
void* accepter_buffer_insert(void* message){
	msg_t* msg_inserito=put_bloccante(accepter_buffer,msg_copy((msg_t*)message));
	free(msg_inserito);
	pthread_exit(NULL);
}

/* inserimento del poison pill nell'accepter buffer */
void accepter_buffer_insert_poison_pill(){
	put_bloccante(accepter_buffer,POISON_PILL);
}

/* lettura bloccante dall'accepter buffer */
msg_t* accepter_buffer_read(){
	msg_t* msg_estratto = get_bloccante(accepter_buffer);
	return msg_estratto;
}