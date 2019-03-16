#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "impl_string_hashmap.h"

const unsigned char CRC7_POLY = 0x91;
unsigned char CRCTable[256];
 
unsigned char getCRCForByte(unsigned char val)
{
  unsigned char j;
 
  for (j = 0; j < 8; j++)
  {
    if (val & 1)
      val ^= CRC7_POLY;
    val >>= 1;
  }
 
  return val;
}
 
void buildCRCTable()
{
  int i;
 
  // fill an array with CRC values of all 256 possible bytes
  for (i = 0; i < 64; i+=4)
  {
    CRCTable[i] = getCRCForByte(i);
    CRCTable[i+1] = getCRCForByte(i+1);
    CRCTable[i+2] = getCRCForByte(i+2);
    CRCTable[i+3] = getCRCForByte(i+3);
  }
}
 
unsigned char getCRC2(unsigned char message[], unsigned char length)
{
  unsigned char i, crc = 0;
 
  for (i = 0; i < length; i++)
    crc = CRCTable[crc ^ message[i]];
  return CRCTable[crc];
}

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
    // object->intrusive_ht_object.next = NULL;
    object->intrusive_ht_object.prev = NULL;
}

void* hash_string_new(const void* a)
{
    assert(a);
    string_t* object = (string_t*) a;
    object->crc = getCRC2(object->string, STRING_SIZE);
    object->intrusive_ht_object.ref_count = 0;
    object->intrusive_ht_object.next = NULL;
    object->intrusive_ht_object.prev = NULL;
    return &(object->intrusive_ht_object);  
}

bool hash_string_comparator(const void *a ,const  void* b)
{   
    char a_crc = getCRC2(((string_t *) a)->string, STRING_SIZE);
    if (a_crc != ((string_t *) b)->crc) 
        return false;

    return strcmp( ((string_t *) a)->string , \
                ((string_t *) b)->string ) == 0 ? true : false; 
}

int hash_string_generate_value(const void* a)
{
    return (int) ( ((string_t*) a)->string[0] - 'a') % SIZE_HASMAP;
}

int hash_string_generate_value2(const void* a)
{
    return (int) ( ((string_t*) a)->string[0]) % SIZE_HASMAP;
}

int hash_string_generate_value3(const void* a)
{
  int hash = 0;
  for(int i = 0; i < STRING_SIZE; i++){
    hash += (int) ( ((string_t*) a)->string[0] - 'a');
  }
  return hash % SIZE_HASMAP;
}
