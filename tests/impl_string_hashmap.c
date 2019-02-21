#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "impl_string_hashmap.h"

void dump_table( ht_table_t* hash_table)
{
    printf("NB Buckets : %ld\n" , hash_table->number_of_hash_table_buckets);
    printf("Offset : %ld bytes\n" , hash_table->hash_object_offset);
    
    printf("---- Content : \n");
    for(unsigned i = 0 ; i < hash_table->number_of_hash_table_buckets; i++)
    {
        ht_object_t* current = hash_table->nodes[i];
        printf("Bucket[%u]:\t" , i );
        for( ; current != NULL ; current = current->next)
        {
            string_t* item = (string_t*) ( (unsigned char*)current - hash_table->hash_object_offset);
            printf("\"%s\", ref_count:%d\t" , item->string , current->ref_count );
        }
        printf("\n");
    }
    
    printf("------\n");
}



void hash_string_destroy( void* a)
{
    assert(a);
    string_t* object = (string_t*) a;
    object->intrusive_ht_object.ref_count = 0;
    object->intrusive_ht_object.next = NULL;
    object->intrusive_ht_object.prev = NULL;
}

void* hash_string_new(const void* a)
{
    assert(a);
    string_t* object = (string_t*) a;
    object->intrusive_ht_object.ref_count = 0;
    object->intrusive_ht_object.next = NULL;
    object->intrusive_ht_object.prev = NULL;
    return &(object->intrusive_ht_object);  
}

bool hash_string_comparator(const void *a ,const  void* b)
{
    return strcmp( ((string_t *) a)->string , \
                ((string_t *) b)->string ) == 0 ? true : false; 
}

int hash_string_generate_value(const void* a)
{
    return (int) ( ((string_t*) a)->string[0] - 'a') % SIZE_HASMAP;
}
