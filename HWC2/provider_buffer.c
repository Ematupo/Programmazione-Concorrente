#include "provider_buffer.h"

/* inizializzazione provider buffer */
void provider_buffer_init(int size){
	provider_buffer=buffer_init(size);
}

/* distruzione provider buffer */
void provider_buffer_destroy(){
	buffer_destroy(provider_buffer);
}

/* inserimento bloccante nel provider buffer */
void provider_buffer_insert(msg_t* msg){
	msg_t* msg_inserito= put_bloccante(provider_buffer,msg);
	free(msg_inserito);
}

/* lettura bloccante dal provider buffer */
msg_t* provider_buffer_read(){
	msg_t* message=get_bloccante(provider_buffer);
	return message;
}