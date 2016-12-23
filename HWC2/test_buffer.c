    #include "buffer.h"
    #include <CUnit/Basic.h>
    #include <CUnit/CUnit.h>
    #include <pthread.h>
    #include <string.h>

    /* Variabili utili per i test */

    // buffer
    buffer_t* b_vuoto_unitario=NULL;
    buffer_t* b_pieno_unitario=NULL;
    buffer_t* b_vuoto=NULL;
    buffer_t* b_pieno=NULL;

    // messaggi
    char prova[] ="Prova";
    char prova2[]="Prova2";
    char errore[]="Errore";
    char esempi[5][3]={"es0","es1","es2","es3","es4"};

    // thread concorrenti
    pthread_t thread_prod;  // un thread produttore
    pthread_t thread_cons;  // un thread consumatore

    pthread_t threads_p[5]; // array di 5 thread produttori
    pthread_t threads_c[5]; // array di 5 thread consumatori 


    /* init e clean della suite buffer vuoto unitario */
    int init_suite_buffer_vuoto_unitario (void){
        b_vuoto_unitario=buffer_init(1);
        if (b_vuoto_unitario !=NULL)
            return 0;
        else
            return 1;
    }

    int clean_suite_buffer_vuoto_unitario(void){
        buffer_destroy(b_vuoto_unitario);
        return 0;
    }



    /* init e clean della suite buffer pieno unitario */
    int init_suite_buffer_pieno_unitario (void){
        b_pieno_unitario=buffer_init(1);                    // creo un buffer unitario e inserisco un messaggio all'interno
        if (b_pieno_unitario !=NULL){
            msg_t* msg = msg_init(prova);
            b_pieno_unitario->queue[b_pieno_unitario->D] = *msg;
            b_pieno_unitario->D = (b_pieno_unitario->D+1)%(b_pieno_unitario->size);
            b_pieno_unitario->k=1;
            return 0;
        }
        else return 1;
    }

    int clean_suite_buffer_pieno_unitario(void){
        buffer_destroy(b_pieno_unitario);
        return 0;
    }


    /* init e clean della suite buffer vuoto di dimensione 5 */
    int init_suite_buffer_vuoto(void){
        b_vuoto=buffer_init(5);
        if (b_vuoto !=NULL)
            return 0;
        else
            return 1;
    }

    int clean_suite_buffer_vuoto(void){
        buffer_destroy(b_vuoto);
        return 0;
    }


    /* init e clean della suite buffer pieno di dimensione 5 */
    int init_suite_buffer_pieno (void){
        int i=0;
        b_pieno=buffer_init(5);                 // creo un buffer di dimensione 5 e ci inserisco 5 messaggi all'interno
        if (b_pieno !=NULL){
            for (i=0; i<5; i++){
            msg_t* msg = msg_init(esempi[i]);
            b_pieno->queue[b_pieno->D] = *msg;
            b_pieno->D = (b_pieno->D+1)%(b_pieno->size);
            b_pieno->k++;
            }
            return 0;
        }
        else return 1;
    }

    int clean_suite_buffer_pieno(void){
        buffer_destroy(b_pieno);
        return 0;
    }


/* Funzioni di supporto per i thread concorrenti*/



// Buffer unitario vuoto


    void* prod_bloccante_buffer_unitario_vuoto(msg_t* msg_aspettato) {
        msg_t* msg_inserito = put_bloccante(b_vuoto_unitario,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);
    }

    void* prod_non_bloccante_buffer_unitario_vuoto(msg_t* msg_aspettato) {
        msg_t* msg_inserito = put_non_bloccante(b_vuoto_unitario,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);


    }

    void* cons_non_bloccante_buffer_unitario_vuoto() {
        msg_t* msg_ritirato = get_non_bloccante(b_vuoto_unitario);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);

    }

    void* cons_bloccante_buffer_unitario_vuoto() {
        msg_t* msg_ritirato = get_bloccante(b_vuoto_unitario);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);


    }

    // Buffer unitario pieno

    void* prod_bloccante_buffer_unitario_pieno(msg_t* msg_aspettato){
        msg_t* msg_inserito=put_bloccante(b_pieno_unitario,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);

    }

    void* prod_non_bloccante_buffer_unitario_pieno(msg_t* msg_aspettato){
        msg_t* msg_inserito=put_non_bloccante(b_pieno_unitario,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);

    }

    void* cons_bloccante_buffer_unitario_pieno(){
        msg_t* msg_ritirato = get_bloccante(b_pieno_unitario);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);

    }

    void* cons_non_bloccante_buffer_unitario_pieno(){
        msg_t* msg_ritirato = get_non_bloccante(b_pieno_unitario);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);

    }


// Buffer vuoto

    void* prod_non_bloccante_buffer_vuoto(msg_t* msg_aspettato){
        msg_t* msg_inserito=put_non_bloccante(b_vuoto,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);

    }

    void* prod_bloccante_buffer_vuoto(msg_t* msg_aspettato){
        msg_t* msg_inserito=put_bloccante(b_vuoto,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);

    }

    void* cons_bloccante_buffer_vuoto(){
        msg_t* msg_ritirato = get_bloccante(b_vuoto);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);
    }

   void* cons_non_bloccante_buffer_vuoto() {
        msg_t* msg_ritirato = get_non_bloccante(b_vuoto);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);
    }

// Buffer pieno

    void* prod_bloccante_buffer_pieno(msg_t* msg_aspettato) {
        msg_t* msg_inserito=put_bloccante(b_pieno,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);

    }

    void* cons_bloccante_buffer_pieno(){
        msg_t* msg_ritirato = get_bloccante(b_pieno);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);

    }

    void* cons_non_bloccante_buffer_pieno(){
        msg_t* msg_ritirato = get_non_bloccante(b_pieno);
        msg_destroy(msg_ritirato);
        pthread_exit(NULL);


    }

    void* prod_non_bloccante_buffer_pieno(msg_t* msg_aspettato){
        msg_t* msg_inserito=put_non_bloccante(b_pieno,msg_aspettato);
        msg_destroy(msg_inserito);
        pthread_exit(NULL);

    }

    /* TEST */

    /*Test della suite buffer unitario vuoto */

    /* Produzione bloccante in un buffer unitario vuoto */
    void test_produttore_bloccante_buffer_unitario_vuoto (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        msg_t* msg_aspettato = msg_init((void*)prova);
        msg_t* msg_inserito= put_bloccante(b_vuoto_unitario, msg_aspettato);
        CU_ASSERT_PTR_NOT_EQUAL(msg_inserito, msg_aspettato);
        CU_ASSERT_PTR_EQUAL(msg_inserito->content, msg_aspettato->content);
        CU_ASSERT_EQUAL(b_vuoto_unitario->k,1);
        msg_destroy(msg_aspettato);
        msg_destroy(msg_inserito);

    }


    /* Produzione non bloccante in un buffer unitario vuoto */
    void test_produttore_non_bloccante_buffer_unitario_vuoto (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        msg_t* msg_aspettato = msg_init((void*)prova);
        msg_t* msg_inserito= put_non_bloccante(b_vuoto_unitario, msg_aspettato);
        CU_ASSERT_PTR_NOT_EQUAL(msg_inserito, msg_aspettato);
        CU_ASSERT_PTR_EQUAL(msg_inserito->content, msg_aspettato->content);
        CU_ASSERT_EQUAL(b_vuoto_unitario->k,1);
        msg_destroy(msg_inserito);
        msg_destroy(msg_aspettato);

    }

    /* Consumazione bloccante in un buffer unitario vuoto - il thread consumatore resta infinitamente in attesa*/
    void test_consumatore_bloccante_buffer_unitario_vuoto (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        pthread_create(&thread_cons,NULL, (void*)&cons_bloccante_buffer_unitario_vuoto,NULL);
        sleep(3);                                       // aspetto 3 secondi 
        pthread_cancel(thread_cons);                    // uccido il thread rimasto in attesa
        pthread_join(thread_cons,NULL);                 // aspetto che il thread sia effettivamente terminato
        CU_ASSERT_EQUAL(b_vuoto_unitario->k, 0);        // verifico che il buffer sia rimasto con 0 messaggi
    }


    /* Consumazione non bloccante in un buffer unitario vuoto */
    void test_consumatore_non_bloccante_buffer_unitario_vuoto (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        msg_t* msg_ritirato = get_non_bloccante(b_vuoto_unitario);
        CU_ASSERT_PTR_EQUAL(msg_ritirato, BUFFER_ERROR);            // verifico che la get non sia andata a buon fine e abbia restituito BUFFER_ERROR
        CU_ASSERT_EQUAL(b_vuoto_unitario->k,0);
        msg_destroy(msg_ritirato);

    }

    /* Produzione e consumazione concorrente bloccante in un buffer unitario vuoto - prima il consumatore */
    void test_prod_cons_bloccante_buffer_unitario_vuoto_prima_cons (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        msg_t* msg_aspettato = msg_init((void*)prova);
        pthread_create(&thread_cons,NULL, (void*)&cons_bloccante_buffer_unitario_vuoto,NULL);      // creo un thread consumatore che rimane in attesa che arrivi un messaggio nel buffer
        pthread_create(&thread_prod,NULL,(void*)&prod_bloccante_buffer_unitario_vuoto,msg_aspettato);   // creo un thread produttore che produce un messaggio nel buffer
        pthread_join(thread_cons,NULL);     // aspetto che entrambi i thread terminino
        pthread_join(thread_prod,NULL);
        CU_ASSERT_EQUAL(b_vuoto_unitario->k,0);     // verifico che dopo una produzione ed una consumazione il buffer sia rimasto vuoto
        msg_destroy(msg_aspettato);

    }


    /* Produzione e consumazione concorrente non bloccante in un buffer unitario vuoto - prima il consumatore */
    void test_prod_cons_non_bloccante_buffer_unitario_vuoto_prima_cons (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        msg_t* msg_aspettato = msg_init((void*)prova);
        pthread_create(&thread_cons,NULL, (void*)&cons_non_bloccante_buffer_unitario_vuoto,NULL);    // creo un thread consumatore non bloccante che non potrà ritirare niente dal buffer e aspetto che termini
        pthread_join(thread_cons,NULL);
        pthread_create(&thread_prod,NULL,(void*)&prod_non_bloccante_buffer_unitario_vuoto,msg_aspettato);       // creo un thread produttore che produce un messaggio nel buffer e aspetto che termini
        pthread_join(thread_prod,NULL);
        CU_ASSERT_EQUAL(b_vuoto_unitario->k,1);     // mi aspetto che nel buffer ci sia il messaggio del produttore dato che non è stato consumato
        msg_destroy(msg_aspettato);

    }

    /* Produzione e consumazione concorrente bloccante di più thread in un buffer unitario vuoto */
    void test_produttori_consumatori_concorrenti_bloccanti_buffer_unitario_vuoto (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        int i=0;
        for (i=0;i<5;i++){                                                                      // creo 5 thread concorrenti produttori e 5 consumatori
            pthread_create(&threads_p[i],NULL,(void*)prod_bloccante_buffer_unitario_vuoto,esempi[i]);
            pthread_create(&threads_c[i],NULL,(void*)cons_bloccante_buffer_unitario_vuoto,NULL);
        }
        for (i=0;i<5;i++){                                                                      // aspetto che terminino
            pthread_join(threads_p[i],NULL);
            pthread_join(threads_c[i],NULL);
        }
        CU_ASSERT_EQUAL(b_vuoto_unitario->k,0);                                                 // verifico che dopo 5 produzioni e 5 consumazioni il buffer sia vuoto
    }


    /* Produzione e consumazione concorrente non bloccante di più thread in un buffer unitario vuoto */
    void test_produttori_consumatori_concorrenti_non_bloccanti_buffer_unitario_vuoto (void){
        clean_suite_buffer_vuoto_unitario();
        init_suite_buffer_vuoto_unitario();
        int i=0;
        for (i=0;i<5;i++){
            pthread_create(&threads_p[i],NULL,(void*)prod_non_bloccante_buffer_unitario_vuoto,esempi[i]);          // creo  5 produttori non bloccanti e poi 5 consumatori non bloccanti con pause di 1 secondo
            sleep(1);
            pthread_create(&threads_c[i],NULL,(void*)cons_non_bloccante_buffer_unitario_vuoto,NULL);
            sleep(1);
        }
        for (i=0;i<5;i++){                                                                                  // aspetto che terminino
            pthread_join(threads_p[i],NULL);
            pthread_join(threads_c[i],NULL);
        }
        CU_ASSERT_EQUAL(b_vuoto_unitario->k,0);                                                 // verifico che dopo 5 produzioni e 5 consumazioni il buffer sia vuoto
    }



    /*Test della suite buffer unitario pieno*/

    /* Consumazione bloccante in un buffer unitario pieno */
    void test_consumatore_bloccante_buffer_unitario_pieno (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        msg_t* msg_ritirato = get_bloccante(b_pieno_unitario);
        CU_ASSERT_PTR_NOT_NULL(msg_ritirato);
        CU_ASSERT_PTR_EQUAL(msg_ritirato->content, prova);
        CU_ASSERT_EQUAL(b_pieno_unitario->k,0);
        msg_destroy(msg_ritirato);
    }

    /* Consumazione non bloccante in un buffer unitario pieno */
    void test_consumatore_non_bloccante_buffer_unitario_pieno (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        msg_t* msg_ritirato = get_non_bloccante(b_pieno_unitario);
        CU_ASSERT_PTR_NOT_NULL(msg_ritirato);
        CU_ASSERT_PTR_EQUAL(msg_ritirato->content, prova);
        CU_ASSERT_EQUAL(b_pieno_unitario->k,0);
        msg_destroy(msg_ritirato);
    }

    /* Produzione bloccante in un buffer unitario pieno - il thread produttore rimane infinitamente in attesa*/
    void test_produttore_bloccante_buffer_unitario_pieno (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        msg_t* msg_inserito = msg_init((void*)prova2);
        pthread_create(&thread_prod,NULL, (void*)&prod_bloccante_buffer_unitario_pieno,msg_inserito); // creo un thread che effettua una put bloccante
        sleep(3);                                                                                                     // aspetto 3 secondi
        pthread_cancel(thread_prod);                                                                                  //uccido il thread
        pthread_join(thread_prod,NULL);                                                                               // aspetto che il thread sia effettivamente terminato                                                                                  
        CU_ASSERT_EQUAL(b_pieno_unitario->k, b_pieno_unitario->size);                                                 // verifico che nel buffer non sia cambiato nulla
        CU_ASSERT_PTR_NOT_EQUAL(msg_inserito->content, b_pieno_unitario->queue[0].content);
        msg_destroy(msg_inserito);
    }


    /* Produzione non bloccante in un buffer unitario pieno */
    void test_produttore_non_bloccante_buffer_unitario_pieno (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        msg_t* msg_aspettato = msg_init((void*)prova2);
        msg_t* msg_inserito= put_non_bloccante(b_pieno_unitario, msg_aspettato);
        CU_ASSERT_PTR_NOT_EQUAL(msg_inserito, msg_aspettato);
        CU_ASSERT_PTR_EQUAL(msg_inserito, BUFFER_ERROR);                // verifico che la put non sia andata a buon fine e abbia restituito BUFFER_ERROR
        CU_ASSERT_EQUAL(b_pieno_unitario->k,1);
        msg_destroy(msg_inserito);
        msg_destroy(msg_aspettato);

    }


    /* Produzione e consumazione concorrente non bloccante in un buffer unitario pieno - prima il produttore */
    void test_prod_cons_bloccante_buffer_unitario_pieno_prima_prod (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        msg_t* msg_aspettato = msg_init((void*)prova);
        pthread_create(&thread_prod,NULL, (void*)&prod_bloccante_buffer_unitario_pieno,msg_aspettato); // creo un thread produttore bloccante
        pthread_create(&thread_cons,NULL, (void*)&cons_bloccante_buffer_unitario_pieno,NULL);          // creo un thread consumatore bloccante
        pthread_join(thread_cons,NULL);             // aspetto che terminino
        pthread_join(thread_prod,NULL);
        CU_ASSERT_EQUAL(b_pieno_unitario->k,1);     // dopo la loro terminazione verifico che il buffer unitario sia pieno
        msg_destroy(msg_aspettato);
    }


    /* Produzione e consumazione concorrente non bloccante in un buffer unitario pieno - prima il produttore */
    void test_prod_cons_non_bloccante_buffer_unitario_pieno_prima_prod (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        msg_t* msg_aspettato = msg_init((void*)prova);
        pthread_create(&thread_prod,NULL, (void*)&prod_non_bloccante_buffer_unitario_pieno,msg_aspettato);  // creo un thread produttore non bloccante che non potrà inserire nulla nel buffer unitario pieno e restituirà BUFFER_ERROR
        pthread_join(thread_prod,NULL);     // aspetto che termini
        pthread_create(&thread_cons,NULL,(void*)&cons_non_bloccante_buffer_unitario_pieno,NULL);        // creo un thread consumatore non bloccante
        pthread_join(thread_cons,NULL);     // aspetto che termini
        CU_ASSERT_EQUAL(b_pieno_unitario->k,0);     // dato che solo la consumazione non bloccante è andata a buon fine, il buffer deve essere vuoto
        msg_destroy(msg_aspettato);
    }

 /* Produzione e consumazione concorrente bloccante di più thread in un buffer unitario pieno */
    void test_produttori_consumatori_concorrenti_bloccanti_buffer_unitario_pieno (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        int i=0;
        for (i=0;i<5;i++){                                                                       // creo 5 thread concorrenti produttori e 5 consumatori bloccanti
            pthread_create(&threads_c[i],NULL,(void*)cons_bloccante_buffer_unitario_pieno,NULL);
            pthread_create(&threads_p[i],NULL,(void*)prod_bloccante_buffer_unitario_pieno,esempi[i]);

        }
        for (i=0;i<5;i++){                  // aspetto che terminino
            pthread_join(threads_c[i],NULL);
            pthread_join(threads_p[i],NULL);
        }
        CU_ASSERT_EQUAL(b_pieno_unitario->k,1);     // dopo le 5 produzioni e le 5 consumazioni mi aspetto che nel buffer sia rimasto un messaggio
    }

    /* Produzione e consumazione concorrente non bloccante di più thread in un buffer unitario vuoto */
    void test_produttori_consumatori_concorrenti_non_bloccanti_buffer_unitario_pieno (void){
        clean_suite_buffer_pieno_unitario();
        init_suite_buffer_pieno_unitario();
        int i=0;
        for (i=0;i<5;i++){                                      // creo 5 thread concorrenti produttori e 5 consumatori non bloccanti con un intervallo di 1 secondo tra le creazioni
            pthread_create(&threads_c[i],NULL,(void*)cons_non_bloccante_buffer_unitario_pieno,NULL);
            sleep(1);
            pthread_create(&threads_p[i],NULL,(void*)prod_non_bloccante_buffer_unitario_pieno,esempi[i]);
            sleep(1);

        }
        for (i=0;i<5;i++){              // aspetto che terminino
            pthread_join(threads_c[i],NULL);
            pthread_join(threads_p[i],NULL);
        }
        CU_ASSERT_EQUAL(b_pieno_unitario->k,1);    // dopo le 5 produzioni e le 5 consumazioni mi aspetto che nel buffer sia rimasto un messaggio
    }



    /*Test della suite buffer vuoto (di dimensione 5) */

    /* Produzione concorrente bloccante, il buffer si satura in corso*/
    void test_produttori_concorrenti_bloccanti_buffer_vuoto(void){
        clean_suite_buffer_vuoto();
        init_suite_buffer_vuoto();
        int i=0;
        for (i=0;i<5;i++){                              // creo 5 thread concorrenti bloccanti che inseriranno un messaggio nel buffer
            pthread_create(&threads_p[i],NULL,(void*)prod_bloccante_buffer_vuoto,esempi[i]);
        }
        pthread_join(threads_p[0],NULL);                // aspetto che terminino
        pthread_join(threads_p[1],NULL);
        pthread_join(threads_p[2],NULL);
        pthread_join(threads_p[3],NULL);
        pthread_join(threads_p[4],NULL);
        CU_ASSERT_EQUAL(b_vuoto->k,5);      // dopo 5 produzioni il buffer deve essere saturo
    }


    /* Produzione concorrente non bloccante, il buffer si satura in corso*/
    void test_produttori_concorrenti_non_bloccanti_buffer_vuoto(void){
        clean_suite_buffer_vuoto();
        init_suite_buffer_vuoto();
        int i=0;
        for (i=0;i<5;i++){                              // creo 5 thread concorrenti non bloccanti che inseriranno un messaggio nel buffer 
            pthread_create(&threads_p[i],NULL,(void*)prod_non_bloccante_buffer_vuoto,esempi[i]);
        }
        pthread_join(threads_p[0],NULL);                // aspetto che terminino
        pthread_join(threads_p[1],NULL);
        pthread_join(threads_p[2],NULL);
        pthread_join(threads_p[3],NULL);
        pthread_join(threads_p[4],NULL);
        CU_ASSERT_EQUAL(b_vuoto->k,5);  // il buffer si è saturato, se provo ad aggiungere un altro messaggio ritorna BUFFER_ERROR
    }

    /* Produzione e consumazione concorrente bloccante*/
    void test_produttori_consumatori_concorrenti_bloccanti_buffer_vuoto(void){
        clean_suite_buffer_vuoto();
        init_suite_buffer_vuoto();
        int i=0;
        CU_ASSERT_EQUAL(b_vuoto->k,0)
        for (i=0;i<5;i++){                  // creo 5 thread concorrenti produttori e 5 consumatori bloccanti
            pthread_create(&threads_p[i],NULL,(void*)prod_bloccante_buffer_vuoto,esempi[i]); 
            pthread_create(&threads_c[i],NULL,(void*)cons_bloccante_buffer_vuoto,NULL);

        }
        pthread_join(threads_p[0],NULL);          // aspetto che terminino
        pthread_join(threads_c[0],NULL);
        pthread_join(threads_p[1],NULL);
        pthread_join(threads_c[1],NULL);
        pthread_join(threads_p[2],NULL);
        pthread_join(threads_c[2],NULL);
        pthread_join(threads_p[3],NULL);
        pthread_join(threads_c[3],NULL);
        pthread_join(threads_p[4],NULL);
        pthread_join(threads_c[4],NULL);
        CU_ASSERT_EQUAL(b_vuoto->k,0);  // dopo 5 consumazioni e 5 produzioni il buffer deve tornare vuoto
    }


    /* Produzione e consumazione concorrente non bloccante*/
    void test_produttori_consumatori_concorrenti_non_bloccanti_buffer_vuoto(void){
        clean_suite_buffer_vuoto();
        init_suite_buffer_vuoto();
        int i=0;
        for (i=0;i<5;i++){                  // creo 5 thread concorrenti produttori e 5 consumatori non bloccanti con un intervallo di 1 secondo tra le loro creazioni
            pthread_create(&threads_p[i],NULL,(void*)prod_non_bloccante_buffer_vuoto,esempi[i]);
            sleep(1);
            pthread_create(&threads_c[i],NULL,(void*)cons_non_bloccante_buffer_vuoto,NULL);
            sleep(1);
        }
        pthread_join(threads_p[0],NULL);    // aspetto che terminino
        pthread_join(threads_c[0],NULL);
        pthread_join(threads_p[1],NULL);
        pthread_join(threads_c[1],NULL);
        pthread_join(threads_p[2],NULL);
        pthread_join(threads_c[2],NULL);
        pthread_join(threads_p[3],NULL);
        pthread_join(threads_c[3],NULL);
        pthread_join(threads_p[4],NULL);
        pthread_join(threads_c[4],NULL);
        CU_ASSERT_EQUAL(b_vuoto->k,0);  // dopo 5 consumazioni e 5 produzioni il buffer deve tornare vuoto
    }


    /*Test della suite buffer pieno (di dimensione 5) */

    /* Consumazione concorrente bloccante, il buffer si svuota*/
    void test_consumatori_concorrenti_bloccanti_buffer_pieno(void){
        clean_suite_buffer_pieno();
        init_suite_buffer_pieno();
        int i=0;
        CU_ASSERT_EQUAL(b_pieno->k,5);
        for (i=0;i<5;i++){
            pthread_create(&threads_c[i],NULL,(void*)cons_bloccante_buffer_pieno,esempi[i]);       // creo 5 thread concorrenti consumatori bloccanti
        }
        pthread_join(threads_c[0],NULL);    // aspetto che terminino
        pthread_join(threads_c[1],NULL);
        pthread_join(threads_c[2],NULL);
        pthread_join(threads_c[3],NULL);
        pthread_join(threads_c[4],NULL);
        CU_ASSERT_EQUAL(b_pieno->k,0);  // verifico che il buffer si sia svuotato
    }


    /* Consumazione concorrente non bloccante, il buffer si svuota*/
    void test_consumatori_concorrenti_non_bloccanti_buffer_pieno(void){
        clean_suite_buffer_pieno();
        init_suite_buffer_pieno();
        int i=0;
        CU_ASSERT_EQUAL(b_pieno->k,5);
        for (i=0;i<5;i++){
            pthread_create(&threads_c[i],NULL,(void*)cons_non_bloccante_buffer_pieno,NULL);        // creo 5 thread concorrenti consumatori non bloccanti
        }
        pthread_join(threads_c[0],NULL);  // aspetto che terminino
        pthread_join(threads_c[1],NULL);
        pthread_join(threads_c[2],NULL);
        pthread_join(threads_c[3],NULL);
        pthread_join(threads_c[4],NULL);
        CU_ASSERT_EQUAL(b_pieno->k,0);  // verifico che il buffer si sia svuotato 
    }


    /* Produzione e consumazione concorrente bloccante*/
    void test_produttori_consumatori_concorrenti_bloccanti_buffer_pieno(void){
        clean_suite_buffer_pieno();
        init_suite_buffer_pieno();
        int i=0;
        for (i=0;i<5;i++){
            pthread_create(&threads_c[i],NULL,(void*)cons_bloccante_buffer_pieno,NULL);        // creo 5 thread concorrenti produttori e 5 consumatori bloccanti
            pthread_create(&threads_p[i],NULL,(void*)prod_bloccante_buffer_pieno,esempi[i]);

        }
        pthread_join(threads_c[0],NULL); // aspetto che terminino
        pthread_join(threads_p[0],NULL);
        pthread_join(threads_c[1],NULL);
        pthread_join(threads_p[1],NULL);
        pthread_join(threads_c[2],NULL);
        pthread_join(threads_p[2],NULL);
        pthread_join(threads_c[3],NULL);
        pthread_join(threads_p[3],NULL);
        pthread_join(threads_c[4],NULL);
        pthread_join(threads_p[4],NULL);
        CU_ASSERT_EQUAL(b_pieno->k,5);  // dopo 5 consumazioni e 5 produzioni verifico che il buffer sia tornato pieno
    }

    /* Produzione e consumazione concorrente non bloccante*/
    void test_produttori_consumatori_concorrenti_non_bloccanti_buffer_pieno(void){
        clean_suite_buffer_pieno();
        init_suite_buffer_pieno();
        int i=0;
        for (i=0;i<5;i++){
            pthread_create(&threads_c[i],NULL,(void*)cons_non_bloccante_buffer_pieno,NULL);        // creo 5 thread concorrenti produttori e 5 consumatori non bloccanti, con un intervallo di 1 secondo tra le loro creazioni
            sleep(1);
            pthread_create(&threads_p[i],NULL,(void*)prod_non_bloccante_buffer_pieno,esempi[i]);
            sleep(1);
        }
        pthread_join(threads_c[0],NULL);            // aspetto che terminino
        pthread_join(threads_p[0],NULL);
        pthread_join(threads_c[1],NULL);
        pthread_join(threads_p[1],NULL);
        pthread_join(threads_c[2],NULL);
        pthread_join(threads_p[2],NULL);
        pthread_join(threads_c[3],NULL);
        pthread_join(threads_p[3],NULL);
        pthread_join(threads_c[4],NULL);
        pthread_join(threads_p[4],NULL);
        CU_ASSERT_EQUAL(b_pieno->k,5);  // dopo 5 consumazioni e 5 produzioni verifico che il buffer sia tornato pieno
    }


/*Main - esecuzione dei test */

    int main (){
        CU_pSuite pSuite = NULL;

        /* Inizializzazione del registro dei test */
        if (CUE_SUCCESS != CU_initialize_registry())
            return CU_get_error();

        /* Aggiunta della prima suite di test al registro */
        pSuite = CU_add_suite("Suite buffer unitario vuoto",init_suite_buffer_vuoto_unitario,clean_suite_buffer_vuoto_unitario);
        if (NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }

        /* Aggiunta dei test alla prima suite */
        if ((NULL == CU_add_test(pSuite, "Test put_bloccante() ",  test_produttore_bloccante_buffer_unitario_vuoto))||
            (NULL == CU_add_test(pSuite, "Test put_non_bloccante() ", test_produttore_non_bloccante_buffer_unitario_vuoto))||
            (NULL == CU_add_test(pSuite, "Test get_non_bloccante() ", test_consumatore_non_bloccante_buffer_unitario_vuoto))||
            (NULL == CU_add_test(pSuite, "Test get_bloccante() ", test_consumatore_bloccante_buffer_unitario_vuoto))||
            (NULL == CU_add_test(pSuite, "Test produttore e consumatore bloccanti, prima consumatore ", test_prod_cons_bloccante_buffer_unitario_vuoto_prima_cons))||
            (NULL == CU_add_test(pSuite, "Test produttore e consumatore non bloccanti, prima consumatore ", test_prod_cons_non_bloccante_buffer_unitario_vuoto_prima_cons))|| 
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti bloccanti ", test_produttori_consumatori_concorrenti_bloccanti_buffer_unitario_vuoto))||
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti non bloccanti ", test_produttori_consumatori_concorrenti_non_bloccanti_buffer_unitario_vuoto)))
        {
            CU_cleanup_registry();
            return CU_get_error();
        }

         /*Aggiunta della seconda suite di test al registro*/
        pSuite = CU_add_suite("Suite buffer unitario pieno",init_suite_buffer_pieno_unitario,clean_suite_buffer_pieno_unitario);
        if (NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }

        /* Aggiunta dei test alla seconda suite */
        if ((NULL == CU_add_test(pSuite, "Test get_bloccante() ", test_consumatore_bloccante_buffer_unitario_pieno))||
            (NULL == CU_add_test(pSuite, "Test get_non_bloccante() ", test_consumatore_non_bloccante_buffer_unitario_pieno))||
            (NULL == CU_add_test(pSuite, "Test put_non_bloccante() ", test_produttore_non_bloccante_buffer_unitario_pieno))||
            (NULL == CU_add_test(pSuite, "Test put_bloccante() ", test_produttore_bloccante_buffer_unitario_pieno))||
            (NULL == CU_add_test(pSuite, "Test produttore e consumatore non bloccanti, prima produttore ", test_prod_cons_non_bloccante_buffer_unitario_pieno_prima_prod))||  
            (NULL == CU_add_test(pSuite, "Test produttore e consumatore bloccanti, prima produttore ", test_prod_cons_bloccante_buffer_unitario_pieno_prima_prod))||
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti bloccanti ", test_produttori_consumatori_concorrenti_bloccanti_buffer_unitario_pieno))||
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti non bloccanti ", test_produttori_consumatori_concorrenti_non_bloccanti_buffer_unitario_pieno)))

        {
            CU_cleanup_registry();
            return CU_get_error();
        }

         /*Aggiunta della terza suite di test al registro*/
        pSuite = CU_add_suite("Suite buffer vuoto (dimensione 5)",init_suite_buffer_vuoto,clean_suite_buffer_vuoto);
        if (NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }

        /* Aggiunta dei test alla terza suite*/
        if ((NULL == CU_add_test(pSuite, "Test produttori concorrenti bloccanti ", test_produttori_concorrenti_bloccanti_buffer_vuoto))||
            (NULL == CU_add_test(pSuite, "Test produttori concorrenti non bloccanti ", test_produttori_concorrenti_non_bloccanti_buffer_vuoto))||
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti bloccanti ", test_produttori_consumatori_concorrenti_bloccanti_buffer_vuoto))||
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti non bloccanti ", test_produttori_consumatori_concorrenti_non_bloccanti_buffer_vuoto)))

        {
            CU_cleanup_registry();
            return CU_get_error();
        }


             /*Aggiunta della quarta suite di test al registro*/
        pSuite = CU_add_suite("Suite buffer pieno (dimensione 5)",init_suite_buffer_pieno,clean_suite_buffer_pieno);
        if (NULL == pSuite) {
            CU_cleanup_registry();
            return CU_get_error();
        }

        /* Aggiunta dei test alla quarta suite*/
        if ((NULL == CU_add_test(pSuite, "Test consumatori concorrenti bloccanti ", test_consumatori_concorrenti_bloccanti_buffer_pieno))||
            (NULL == CU_add_test(pSuite, "Test consumatori concorrenti non bloccanti ", test_consumatori_concorrenti_non_bloccanti_buffer_pieno))||
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti bloccanti ", test_produttori_consumatori_concorrenti_bloccanti_buffer_pieno))||
            (NULL == CU_add_test(pSuite, "Test produttori e consumatori concorrenti non bloccanti ", test_produttori_consumatori_concorrenti_non_bloccanti_buffer_pieno)))

        {
            CU_cleanup_registry();
            return CU_get_error();
        }
        /* Esecuzione di tutti i test con l'iterfaccia di base */
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        printf("\n");
        CU_basic_show_failures(CU_get_failure_list());
        printf("\n\n");

        /* Pulizia del registro dei test e return */
        CU_cleanup_registry();
        return CU_get_error();
    }