#include "msg.h"
#include "poison_pill.h"


/* Creazione di un messaggio con un contenuto generico */
msg_t* msg_init(void* content){
    msg_t* msg = (msg_t*)malloc(sizeof(msg_t));
    msg->content=content;
    msg->msg_copy=msg_copy;
    msg->msg_init=msg_init;
    msg->msg_destroy=msg_destroy;
    return msg;
}

/* Distruzione di un messaggio */
void msg_destroy(msg_t* msg){
/* Se il contentuto del messaggio è allocato dinamicamente allora si dovrebbe aggiungere: */
    free(msg);
}

/*Copia di un messaggio*/
msg_t* msg_copy(msg_t* msg){
    return msg_init((void*)msg->content);
}
