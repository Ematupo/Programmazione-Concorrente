#include "accepter.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <pthread.h>

    pthread_t accepter, requester1, requester2;
    msg_t* msg1;
    msg_t* msg2;

  


/* Init della suite */
int init_suite_accepter(void){
    accepter_buffer_init(2);
    reader_list_init();
    msg1 = msg_init("1");
    msg2 = msg_init("2");


    return 0;
}

/* Clean della suite */
int clean_suite_accepter(void){
    iterator=iterator_init(reader_list);
    while (hasNext(iterator)){
        reader_t* reader=(reader_t*)next(iterator);
        pthread_cancel(reader->thread);                 // uccido i flussi reader creati dall'accepter
        pthread_join(reader->thread,NULL);
    }
    iterator_destroy(iterator);
    reader_list_destroy();
    accepter_buffer_destroy();
    msg_destroy(msg1);
    msg_destroy(msg2);
 
    return 0;
}


/* Test accettazione richieste e creazione reader (accepter) */
void test_submitRequest (void) {
    pthread_create(&accepter,NULL, accepter_start, NULL);       // invoco i thread dell'accepter e dei requester
    pthread_create(&requester1,NULL, accepter_buffer_insert, msg1);
    pthread_join(requester1,NULL);
    pthread_create(&requester2,NULL, accepter_buffer_insert, msg2);
    pthread_join(requester2,NULL);
    sleep(2);
    CU_ASSERT_EQUAL(reader_list_size(), 2);         // verifico che l'accepter abbia creato i reader e li abbia inseriti nella reader list
    accepter_buffer_insert_poison_pill();           // invio il poison pill all'accepter
    pthread_join(accepter,NULL);                    // aspetto che termini l'accepter
}



int main (){
    CU_pSuite pSuite = NULL;

    /* Inizializza il registro dei test */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Aggiungi la suite di test al registro */
    pSuite = CU_add_suite("Suite accepter",init_suite_accepter,clean_suite_accepter);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Aggiungi i test alla suite */
    if ((NULL == CU_add_test(pSuite, "Test accettazione richieste e creazione reader (accepter) \n", test_submitRequest))){
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