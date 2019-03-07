#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include "hash_map.h"


void*
ht_object_get(struct ht_table *hash_table,
              const void *lookup_object,
              bool create_if_does_not_exist)
{
    if(!hash_table || !lookup_object)
    	return NULL;

    pthread_mutex_lock(&hash_table->lock);

    int hash = hash_table->hash_object_generate_value(lookup_object);

    bool added = false;
    ht_object_t* returned_result = NULL;

    for( ht_object_t* iter =  hash_table->nodes[hash];
			      	  iter != NULL && !added ;
    	              iter = iter->next)
    {
	    if( hash_table->hash_object_comparator( lookup_object , \
	                    (void*) ( (unsigned char*)iter - hash_table->hash_object_offset)) ){
	        iter->ref_count++;
	        added = true;
    	    returned_result = iter;
	    }
    }
    
    if(create_if_does_not_exist && !added)
    {

        ht_object_t* head =  hash_table->nodes[hash];

        ht_object_t* new_object = hash_table->hash_object_new( lookup_object );	    
        new_object->next = head;       
        new_object->ref_count = 1;
        hash_table->nodes[hash] = new_object;
        
        if(head)
            head->prev = new_object;

        returned_result = new_object;
    }
    
    pthread_mutex_unlock(&hash_table->lock);
    

    return returned_result;
}

void
ht_object_put(struct ht_table *hash_table,
              void *hash_table_object)
{
    if(!hash_table || !hash_table_object)
        	return;

    pthread_mutex_lock(&hash_table->lock);

    ht_object_t* current_object = (ht_object_t *) hash_table_object;
    assert(current_object->ref_count > 0);
    current_object->ref_count--;

    if( current_object->ref_count == 0 )
    {
		if(current_object->prev)
			current_object->prev->next = current_object->next;
	
		if(current_object->next)
			current_object->next->prev = current_object->prev;    

		hash_table->hash_object_destroy( current_object );
    }
	    
    pthread_mutex_unlock(&hash_table->lock);
}


void
ht_destroy(struct ht_table *hash_table)
{

    pthread_mutex_lock(&hash_table->lock);
    for(unsigned i = 0 ; i < hash_table->number_of_hash_table_buckets; i++)
    {
        ht_object_t* current = hash_table->nodes[i];
        while(current != NULL)
        {
            ht_object_t* next = current->next;
			hash_table->hash_object_destroy(current);
            current = next;
        }
    }
    free(hash_table->nodes);
    pthread_mutex_unlock(&hash_table->lock);
    
    pthread_mutex_destroy( &hash_table->lock );
    free(hash_table);
}


struct ht_table *
ht_new(size_t number_of_hash_table_buckets,
       off_t hash_object_offset,
       void * (*hash_object_new)(const void *),
       void (*hash_object_destroy)(void *),
       bool (*hash_object_comparator)(const void *, const void *),
       int (*hash_object_generate_value)(const void *))
{
	ht_table_t* created_ht = (ht_table_t*) malloc( sizeof(ht_table_t));
	
	created_ht->number_of_hash_table_buckets = number_of_hash_table_buckets;
	created_ht->hash_object_new = hash_object_new;
	created_ht->hash_object_destroy = hash_object_destroy;
	created_ht->hash_object_comparator = hash_object_comparator;
	created_ht->hash_object_generate_value = hash_object_generate_value;
	created_ht->hash_object_offset = hash_object_offset;
    
    created_ht->nodes = (ht_object_t**) (malloc(sizeof(ht_object_t*) *\
    							 number_of_hash_table_buckets));
    pthread_mutex_init(&(created_ht->lock), NULL);
    
    for(unsigned i = 0 ; i < number_of_hash_table_buckets ; i++)
           created_ht->nodes[i] = NULL;

	return created_ht;
}


