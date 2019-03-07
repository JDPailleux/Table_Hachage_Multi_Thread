#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>
#include <hash_map.h>
#include"impl_string_hashmap.h"

#define NB_THREADS 4


typedef struct threads_param
{
    ht_table_t* ht;
	string_t a;
} threads_param_t;

void* thread_callback(void* args)
{
    threads_param_t* params = (threads_param_t*) args;
    int i =0;
	
    while( i < 100)
    {
        i++;
        void* handler_data = ht_object_get( params->ht , (void*) &(params->a) , true);
/*        ht_object_put(params->ht , handler_a);*/
    }
    return 0;
}

/**
  * This test uses 26 threads. Each thread has its string object and 
  * manipulate it through the table
  * To succeed, the test should plot a table containing 26 string_t objects,
  * with 100 ref_counts per node 
*/
int main(int argc, char* argv[])
{
	pthread_t threads_pool[NB_THREADS];
	threads_param_t params_pool[NB_THREADS];
    
    struct timeval start, end;
    gettimeofday(&start, NULL);


    ht_table_t* ht = ht_new( SIZE_HASMAP , __builtin_offsetof(string_t, intrusive_ht_object) ,
    							  hash_string_new ,
                                  hash_string_destroy,
                                  hash_string_comparator,
                                  hash_string_generate_value);
                                     	                                  
	for(int i = 0 ; i < NB_THREADS; i++)
	{
	    params_pool[i].ht = ht;
		char carac = 'a' + i%26;
		sprintf( params_pool[i].a.string , "%c%c%c" , carac,carac,carac);	
	    pthread_create(&threads_pool[i], NULL, thread_callback, &(params_pool[i]));	
	}
	 
 	for(int i = 0 ; i < NB_THREADS; i++)
	    pthread_join(threads_pool[i], NULL);

    dump_table(ht);
    ht_destroy(ht);
    
    gettimeofday(&end, NULL);
    printf("Elapsed time : %ld us\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));
}

