#ifndef __IMPL_STRING_TABLE_H__
#define __IMPL_STRING_TABLE_H__

#include <hash_map.h>

#define SIZE_HASMAP 10
#define STRING_SIZE 100

typedef struct string
{
    char string[STRING_SIZE];
    ht_object_t intrusive_ht_object;
    uint32_t crc;
}string_t;


/**
 * dump_table - print in the shell, the content of the hashtable
 * for debug purposes 
  * @hash_table:  The hash table to print  
*/
void dump_table( ht_table_t* hash_table);

/*
 * Implementation of the API of the hash_table
*/ 

int hash_string_generate_value(const void* a);
bool hash_string_comparator(const void *a ,const  void* b);
void* hash_string_new(const void* a);
void hash_string_destroy(void* a);

#endif
