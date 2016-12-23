#include "reader_list.h"

/* funzioni per la gestione delle reader list che rendono thread safe quelle già presenti in list.c */

void reader_list_init(){
	reader_list=list_init();
	pthread_mutex_init(&(reader_list_access),NULL);	
}


void reader_list_destroy(){
	pthread_mutex_destroy(&(reader_list_access));
	list_destroy(reader_list);
}


int reader_list_isEmpty(){
	pthread_mutex_lock(&(reader_list_access));
	int empty=isEmpty(reader_list);
	pthread_mutex_unlock(&(reader_list_access));
	return empty;
}

void addReader(reader_t* reader){
	pthread_mutex_lock(&(reader_list_access));
	addElement(reader_list, reader);
	pthread_mutex_unlock(&(reader_list_access));
}

int removeReader(reader_t* reader){			
	pthread_mutex_lock(&(reader_list_access));
	int deleted= removeElement(reader_list, reader);
	pthread_mutex_unlock(&(reader_list_access));
	return deleted;
}


/* ************FUNZIONE NON THREAD SAFE! ***************** */
/* Questa funzione di supporto deve essere utilizzata solo all'interno di altre funzioni thread safe (dopo il lock e prima dell'unlock del mutex) */
int reader_list_size(){
	int dimension= size(reader_list);	
	return dimension;
}

