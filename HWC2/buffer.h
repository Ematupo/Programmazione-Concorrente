#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#define BUFFER_ERROR (msg_t*) NULL
#include <pthread.h>
#include "msg.h"

/**Buffer **/
typedef struct buffer {
        int T;
        int D;
        int size;
        int k;
        msg_t* queue;
        pthread_cond_t non_vuote;
        pthread_cond_t non_piene;
        pthread_mutex_t access;
        pthread_mutex_t uso_D;
        pthread_mutex_t uso_T;
} buffer_t;

/* allocazione / deallocazione buffer */
// creazione di un buffer vuoto di dim. max nota
buffer_t* buffer_init(unsigned int maxsize);
// deallocazione di un buffer
void buffer_destroy(buffer_t* buffer);
/* operazioni sul buffer */
// inserimento bloccante: sospende se pieno, quindi
// effettua l’inserimento non appena si libera dello spazio
// restituisce il messaggio inserito; N.B.: msg!=null
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);
// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg);
// estrazione bloccante: sospende se vuoto, quindi
// restituisce il valore estratto non appena disponibile
msg_t* get_bloccante(buffer_t* buffer);
// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* buffer);

#endif // BUFFER_H_INCLUDED
