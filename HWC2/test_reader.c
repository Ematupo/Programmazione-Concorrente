#include "reader.h"
#include "reader_list.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <pthread.h>

    pthread_t reader_thread;
    reader_t* reader;
    msg_t* msg1;
    msg_t* msg2;



/* Init della suite */
int init_suite_reader(void){
    reader = reader_init("Reader di test",1,5);  // reader con tempo di processamento di un secondo e grandezza 5
    reader_list_init();
    msg1=msg_init("1");
    msg2=msg_init("2");
    reader_buffer_insert(reader->reader_buffer, msg1);          // invio nel reader buffer 2 messaggi
    reader_buffer_insert(reader->reader_buffer, msg2);
    return 0;
}

/* Clean della suite */
int clean_suite_reader(void){       // distruggo le strutture dati
    reader_destroy(reader);
    reader_list_destroy();
    msg_destroy(msg1);
    msg_destroy(msg2);
    
    return 0;
}


/* Test consumazione di messaggi del reader */
void test_reader_consume (void) {
    pthread_create(&reader_thread,NULL, (void*)reader_consume,reader);     // creo il flusso reader che inizia a consumare i messaggi
    sleep(5);
    CU_ASSERT_EQUAL(reader->reader_buffer->buffer->k, 0);           // verifico che i messaggi inviati siano stati consumati
    reader_buffer_insert_poison_pill(reader->reader_buffer);    // invio il poison pill per farlo terminare terminare
    pthread_join(reader_thread,NULL);           // aspetto che termini

}


int main (){
    CU_pSuite pSuite = NULL;

    /* Inizializza il registro dei test */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Aggiungi la suite di test al registro */
    pSuite = CU_add_suite("Suite reader",init_suite_reader,clean_suite_reader);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Aggiungi i test alla suite */
    if ((NULL == CU_add_test(pSuite, "Test consumazione messaggi del reader \n", test_reader_consume))){
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