#include "dispatcher.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <pthread.h>

pthread_t dispatcher, provider, reader_lento_thread, reader_veloce_thread;
reader_t* reader_lento;
reader_t* reader_veloce;
msg_t* msg1; 
msg_t* msg2;
int check;
int stop;

  


/* Init della suite */
int init_suite_dispatcher(void){
	provider_buffer_init(10);
	reader_list_init();
    check=0;
	reader_lento = reader_init("lento", 10, 2);
	reader_veloce = reader_init("veloce",0,2);
	addReader(reader_lento);
	addReader(reader_veloce);
    msg1 = msg_init("1");

    return 0;
}

/* Clean della suite */
int clean_suite_dispatcher(void){
	provider_buffer_destroy();
	reader_list_destroy();
 
    return 0;
}


/* funzione di supporto, invia messaggi nel provider buffer finchè check=0 */
void* send_messages() {
    while(check==0) {
        provider_buffer_insert(msg_init("1"));
    }

    pthread_exit(NULL);
}




/* Test inoltro messaggi (dispatcher) */
void test_dispatch_messages (void) {
	pthread_create(&dispatcher, NULL, (void*)dispatch_messages, NULL);    // creo il flusso dispatcher
	provider_buffer_insert(msg1);  // inserisco nel provider buffer un messaggio e il poison pill
	provider_buffer_insert(POISON_PILL);
	pthread_join(dispatcher,NULL); // aspetto che il dispatcher termini
	CU_ASSERT_EQUAL(reader_lento->reader_buffer->buffer->queue[0].content, msg1->content); // verifico l'avvenuta consegna dei messaggi nei reader buffer
	CU_ASSERT_EQUAL(reader_lento->reader_buffer->buffer->queue[1].content, NULL);
	CU_ASSERT_EQUAL(reader_veloce->reader_buffer->buffer->queue[0].content, msg1->content);
	CU_ASSERT_EQUAL(reader_veloce->reader_buffer->buffer->queue[1].content, NULL);
}


/* Test rimozione reader lenti */
void test_remove_slow_readers (void) {
    clean_suite_dispatcher();
    init_suite_dispatcher();
    pthread_create(&reader_veloce_thread,NULL, (void*)reader_consume,reader_veloce);   // creo due flussi reader, uno con tempo di processamento basso ed uno alto
    pthread_create(&reader_lento_thread, NULL, (void*)reader_consume, reader_lento);
    pthread_create(&provider, NULL, (void*)send_messages, NULL);   // creo il flusso provider che invia messaggi al provider buffer
    pthread_create(&dispatcher, NULL, (void*)dispatch_messages, NULL); // creo il flusso dispatcher
    sleep(30);
    CU_ASSERT_EQUAL(reader_lento->flag,1);          // dopo 30 secondi mi aspetto che il flag di reader lento sia portato a 1 (il reader è da rimuovere dalla reader list perchè troppo lento)
    sleep(20);
    CU_ASSERT_EQUAL(reader_list_size(),1);          // dopo altri 20 secondi mi aspetto che la rimozione sia avvenuta
    check=1;                                        // imposto il check a 1 facendo terminare l'invio di messaggi da parte del provider
    pthread_join(provider,NULL);
    provider_buffer_insert(POISON_PILL);            // invio il poison pill e aspetto la fine dei flussi rimanenti
    pthread_join(dispatcher,NULL);
    pthread_join(reader_veloce_thread,NULL);
    pthread_join(reader_lento_thread,NULL);
}




int main (){
    CU_pSuite pSuite = NULL;

    /* Inizializza il registro dei test */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Aggiungi la suite di test al registro */
    pSuite = CU_add_suite("Suite dispatcher",init_suite_dispatcher,clean_suite_dispatcher);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Aggiungi i test alla suite */
    if ((NULL == CU_add_test(pSuite, "Test inoltro messaggi (dispatcher) \n", test_dispatch_messages))||
        (NULL == CU_add_test(pSuite, "Test rimozione reader lenti (dispatcher) \n", test_remove_slow_readers))){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Esegui tutti i test con l'iterfaccia di base */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    /* Pulisci il registro dei test e esci */
    CU_cleanup_registry();
    return CU_get_error();
}