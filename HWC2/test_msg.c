#include "msg.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <pthread.h>
#include <string.h>

msg_t* msg = NULL;
char content[] = "test";

/* Init della suite */
int init_suite_message(void){
    return 0;
}

/* Clean della suite */
int clean_suite_message(void){
    return 0;
}

/* Test creazione messaggio */
void test_msg_init (void) { 
    msg=msg_init((void*)content);
    CU_ASSERT_PTR_NOT_NULL(msg);
    CU_ASSERT_PTR_EQUAL(msg->content, content);
}

/* Test copia messaggio */
void test_msg_copy(void) {
    msg_t* copia = msg_copy(msg);
    CU_ASSERT_PTR_NOT_EQUAL(copia,msg);
    CU_ASSERT_PTR_EQUAL(copia->content, content);
    msg_destroy(msg);
}


int main (){
    CU_pSuite pSuite = NULL;

    /* Inizializza il registro dei test */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Aggiungi la suite di test al registro */
    pSuite = CU_add_suite("Suite messaggio",init_suite_message,clean_suite_message);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Aggiungi i test alla suite */
    if ((NULL == CU_add_test(pSuite, "Test init ", test_msg_init))||
        (NULL == CU_add_test(pSuite, "Test copy ", test_msg_copy)))    {
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