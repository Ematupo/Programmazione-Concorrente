#ifndef BUFFER_C_INCLUDED
#define BUFFER_C_INCLUDED
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>

/* Creazione del buffer */
buffer_t* buffer_init(unsigned int maxsize){
    buffer_t* buffer = (buffer_t*)malloc(sizeof(buffer_t)); 
    buffer->queue=(msg_t*)malloc(maxsize*sizeof(msg_t)); // coda del buffer (implmentata come un array circolare FIFO)
    buffer->T=0;                                      // indice di lettura dei messaggi nel buffer
    buffer->D=0;                                      // indice di scrittura dei messaggi nel buffer
    buffer->size=maxsize;                             // dimensione del buffer
    buffer->k=0;                                      // quantità di messaggi inseriti nel buffer
	pthread_mutex_init(&(buffer->access), NULL);      // mutex che protegge l'accesso al buffer
	pthread_mutex_init(&(buffer->uso_D), NULL);       // mutex che protegge l'accesso all'indice di scrittura del buffer
	pthread_mutex_init(&(buffer->uso_T), NULL);       // mutex che protegge l'accesso all'indice di lettura del buffer
	pthread_cond_init(&(buffer->non_piene),NULL);     // condizione: buffer non pieno 
 	pthread_cond_init(&(buffer->non_vuote),NULL);     // condizione: buffer non vuoto
	return buffer;
}




/* Distruzione del buffer */
void buffer_destroy(buffer_t* buffer){
    pthread_cond_destroy(&(buffer->non_piene));
	pthread_cond_destroy(&(buffer->non_vuote));
	pthread_mutex_destroy(&(buffer->access));
	pthread_mutex_destroy(&(buffer->uso_D));
	pthread_mutex_destroy(&(buffer->uso_T));
    free(buffer->queue);
    free(buffer);
}


/* Estrazione bloccante di un messaggio dal buffer */
msg_t* get_bloccante(buffer_t* buffer){
	msg_t* msg=NULL;
	pthread_mutex_lock(&(buffer->access));         // lock del mutex che protegge l'accesso al buffer
    while(buffer->k==0){
        pthread_cond_wait(&(buffer->non_vuote),&(buffer->access));  // se il buffer è vuoto si attende che un altro thread modifichi la condizione "non_vuote"
    }
    pthread_mutex_lock(&(buffer->uso_T));           // una volta accertato che il buffer non è vuoto si fa la lock del mutex che protegge l'indice di lettura T e si effettua l'estrazione del messaggio
    msg=msg_copy(&(buffer->queue[buffer->T]));      
    buffer->T= (buffer->T+1)%(buffer->size);        
    buffer->k--;
    pthread_mutex_unlock(&(buffer->uso_T));
    pthread_cond_signal(&(buffer->non_piene));      // segnalo agli altri thread che il buffer non è pieno (si modifica la condizione "non_piene")
    pthread_mutex_unlock(&(buffer->access));
    return msg;
}

/* Estrazione non bloccante di un messaggio dal buffer */
msg_t* get_non_bloccante(buffer_t* buffer){
    msg_t* msg=NULL;
    pthread_mutex_lock(&(buffer->access));
	if(buffer->k==0){
		pthread_mutex_unlock(&(buffer->access));          // come la precedente ma in questo caso se il buffer è vuoto ritorno la costante BUFFER_ERROR
		return BUFFER_ERROR;
	}
	pthread_mutex_lock(&(buffer->uso_T));
    msg=msg_copy(&(buffer->queue[buffer->T]));
    buffer->T= (buffer->T+1)%(buffer->size);
    buffer->k--;
    pthread_mutex_unlock(&(buffer->uso_T));
    pthread_cond_signal(&(buffer->non_piene));
    pthread_mutex_unlock(&(buffer->access));
    return msg;
}

/* Inserimento bloccante di un messaggio nel buffer */
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){
    msg_t* temp= msg_copy(msg);
    pthread_mutex_lock(&(buffer->access));          // lock del mutex che protegge l'accesso al buffer
    while(buffer->k==buffer->size){
        pthread_cond_wait(&(buffer->non_piene),&(buffer->access));      // se il buffer è pieno si attende che un altro thread modifichi la condizione "non_piene"
    }
    pthread_mutex_lock(&(buffer->uso_D)); // una volta accertato che il buffer non è pieno si fa il lock del mutex che protegge l'indice di lettura T e si effettua l'inserimento del messaggio
    buffer->queue[buffer->D]=*temp;
    buffer->k++;
    buffer->D=(buffer->D+1)%(buffer->size);
    pthread_mutex_unlock((&(buffer->uso_D)));
    pthread_cond_signal(&(buffer->non_vuote));  // segnalo agli altri thread che il buffer non è vuoto (si modifica la condizione "non_vuote")
    pthread_mutex_unlock(&(buffer->access));
    return temp;
}



/* Inserimento non bloccante di un messaggio nel buffer */
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg){
    msg_t* temp= msg_copy(msg);
    pthread_mutex_lock(&(buffer->access));
    while(buffer->k==buffer->size){
        pthread_mutex_unlock(&(buffer->access));        // come la precedente ma in questo caso se il buffer è pieno ritorno la costante BUFFER_ERROR
        return BUFFER_ERROR;
    }
    pthread_mutex_lock(&(buffer->uso_D));       
    buffer->queue[buffer->D]=*temp;
    buffer->k++;
    buffer->D=(buffer->D+1)%(buffer->size);
    pthread_mutex_unlock((&(buffer->uso_D)));
    pthread_cond_signal(&(buffer->non_vuote));
    pthread_mutex_unlock(&(buffer->access));

    return temp;


}






#endif
