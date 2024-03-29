#ifndef __IMPL_STRING_TABLE_H__
#define __IMPL_STRING_TABLE_H__

#include <hash_map.h>

#define SIZE_HASMAP 10
#define STRING_SIZE 100

typedef struct string
{
    char string[STRING_SIZE];
    ht_object_t intrusive_ht_object;
    char crc; // crc code to check if the value is correct
}string_t;

/*
 * getCRC,
 * this function compute a checksum for a given I/O request.
 */
uint32_t getCRC(char *message, uint32_t length);

/*
 * Advanced getCRC,
 */
unsigned char getCRCForByte(unsigned char val);
void buildCRCTable();
unsigned char getCRC2(unsigned char message[], unsigned char length);
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

int hash_string_generate_value2(const void* a);
int hash_string_generate_value3(const void* a);

#endif
