#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hash_map.h>
#include"impl_string_hashmap.h"

typedef struct threads_param
{
    ht_table_t* ht;
    string_t* a;
} threads_param_t;

void* thread1_callback(void* args)
{
    int i =0;
    threads_param_t* params = (threads_param_t*) args;
    void* handler_a = NULL;
    while( i < 100)
    {
        i++;
        handler_a = ht_object_get( params->ht , params->a , false);
    }

    i = 0;
    while( i < 100 )
    {
        i++;
        ht_object_put(params->ht , handler_a);
    }
    return NULL;
}


void* thread2_callback(void* args)
{
    threads_param_t* params = (threads_param_t*) args;
    int i =0;
    while( i < 100)
    {
        i++;
        void* handler_a = ht_object_get( params->ht , params->a , false);
        ht_object_put(params->ht , handler_a);
    }
    return 0;
}


/**
  * This test uses two threads that manipulate the same string_t object and perform 
  * get and put operations to validate the concurrency implementation 
  * To succeed, the test should plot an empty hash table at the end 
*/
int main(int argc, char* argv[])
{
    string_t a;
    strcpy( a.string , "hello world");
 
    ht_table_t* ht = ht_new( SIZE_HASMAP , __builtin_offsetof(string_t, intrusive_ht_object),
    								  hash_string_new ,
                                      hash_string_destroy,
                                      hash_string_comparator,
                                      hash_string_generate_value);
    pthread_t thread_1;
    pthread_t thread_2;

    threads_param_t params;
    params.ht = ht;
    params.a = &a;
    
    pthread_create(&thread_1, NULL, thread1_callback, &params);
    pthread_create(&thread_2, NULL, thread2_callback, &params);
    
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    dump_table(ht);
    ht_destroy(ht);
    
    return 0;
}

