#ifndef MSG_H_INCLUDED
#define MSG_H_INCLUDED

#include <stdlib.h>
#include <unistd.h>




/** Messaggio **/
typedef struct msg {
    void* content; // generico contenuto del messaggio
    struct msg * (*msg_init)(void*); // creazione msg
    void (*msg_destroy)(struct msg *); // deallocazione msg
    struct msg * (*msg_copy)(struct msg *); // creazione/copia msg
} msg_t;


/* Struttura di supporto per utilizzare array di messaggi nelle funzioni invocate dai thread concorrenti */
typedef struct {
	msg_t** messages;
	int size;
} args;

/** funzioni Messaggio **/
msg_t* msg_init(void* content);
void msg_destroy(msg_t* msg);
msg_t* msg_copy(msg_t* msg);






#endif // MSG_H_INCLUDED
