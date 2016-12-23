#ifndef READER_LIST_H_
#define READER_LIST_H_

#include "list.h"
#include "poison_pill.h"
#include "reader.h"
#include <pthread.h>


list_t* reader_list;
iterator_t* iterator;
pthread_mutex_t reader_list_access;	// rispetto a ciò che è presente in list.c si aggiunge un mutex per rendere la reader list thread safe


void reader_list_init();
void reader_list_destroy();
int reader_list_size();
int reader_list_isEmpty();
void addReader(reader_t* reader);
int removeReader(reader_t* reader);

#endif
