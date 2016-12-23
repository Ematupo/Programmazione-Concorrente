#include "provider.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <pthread.h>

    pthread_t provider;
    args* arguments=NULL;


/* Init della suite */
int init_suite_provider(void){
    provider_buffer_init(3);
    arguments=malloc(sizeof(args));
    arguments->messages = malloc(sizeof(msg_t*)*2);
    arguments->size=2;
    msg_t* msg1= msg_init("1");
    msg_t* msg2= msg_init("2");
    arguments->messages[0]=msg1;
    arguments->messages[1]=msg2;
    return 0;
}

/* Clean della suite */
int clean_suite_provider(void){
    msg_destroy(arguments->messages[0]);
    msg_destroy(arguments->messages[1]);
    free(arguments->messages);
    free(arguments);
    provider_buffer_destroy();
    return 0;
}


/* Test inserimento messaggi nel provider buffer */
void test_insert_messages_prov_buffer (void) {

    pthread_create(&provider,NULL,(void*)insert_messages_prov_buffer,arguments);   // creo il thread provider che inserisce due messaggi nel provider buffer
    pthread_join(provider,NULL);
    CU_ASSERT_EQUAL(arguments->messages[0]->content, provider_buffer->queue[0].content);    // verifico l'avvenuto inserimento
    CU_ASSERT_EQUAL(arguments->messages[1]->content, provider_buffer->queue[1].content);
}


int main (){
    CU_pSuite pSuite = NULL;

    /* Inizializza il registro dei test */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Aggiungi la suite di test al registro */
    pSuite = CU_add_suite("Suite provider",init_suite_provider,clean_suite_provider);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Aggiungi i test alla suite */
    if ((NULL == CU_add_test(pSuite, "Test inserimento messaggi nel provider buffer ", test_insert_messages_prov_buffer))){
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