#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>


typedef struct ht_object
{
    struct ht_object* next;
    struct ht_object* prev;
    int ref_count;
}ht_object_t;

typedef struct ht_table
{
    size_t number_of_hash_table_buckets;
    off_t hash_object_offset;
    void * (*hash_object_new)(const void *);
    void (*hash_object_destroy)(void *);
    bool (*hash_object_comparator)(const void *, const void *);
    int (*hash_object_generate_value)(const void *);

    /* Will need hash buckets in here..
     */
    ht_object_t** nodes;
   
    pthread_mutex_t lock;
    
} ht_table_t;	

/**
 * ht_object_get - Given a hash table and a lookup object which has the same
 *    underlying object type, ht_object_get() will return a pointer to an
 *    object which is resident in the hash table.  This object is guaranteed
7 *    to be valid until the caller has issued ht_object_put().
 * @hash_table:  The hash table for performing the lookup
 * @lookup_object:  Pointer to an opaque object which will be used for
 *    comparison against objects in the hash table.
 * @create_if_does_not_exist:  If 'true' the object will be created if it does
 *    not already exist in the hash table.  The creation of new hash table
 *    objects is guaranteed to produce only unique objects.  It cannot be true
 *    that 2 or more objects in the hash table are for the same object.
 */
void *
ht_object_get(struct ht_table *hash_table,
              const void *lookup_object,
              bool create_if_does_not_exist);

/**
 * ht_object_put - Given a hash table and previously obtained hash object,
 *    ht_object_put() is called when the user of the object no longer requires
 *    access to the hash table object.  Once ht_object_put() is called, the
 *    user must consider the hash object pointer as invalid.  Note that once an
 *    object is 'put' it may be freed.
 * @hash_table:  The hash table for performing the lookup
 * @hash_table_object:  The previously obtained hash object pointer.
 */
void
ht_object_put(struct ht_table *hash_table,
              void *hash_table_object);

/**
 * ht_new - Creates a new hash table according to the provided parameters.
 * @number_of_hash_table_buckets:  Number of hash table buckets.
 * @hash_object_offset:  The offset of the ht_object_t structure within the
 *   application object.  Each application object must contain an ht_object_t.
 *   (See below for ht_object_t).
 * @hash_object_new:  Function to be called whenever a new item must be added
 *   to the table.
 * @hash_object_destroy:  Function to be called when an object must be
 *   destroyed.
 * @hash_object_comparator:  Function which determines whether the lookup
 *   object matches an existing object in the table.
 * @hash_object_generate_value:  Function which operates on the application
 *   object to produce a value to be used as the first term in a modulus
*    operataion to determine the hash bucket number.
 * Returns a pointer to the newly allocated hash table.
 */
struct ht_table *
ht_new(size_t number_of_hash_table_buckets,
       off_t hash_object_offset,
       void * (*hash_object_new)(const void *),
       void (*hash_object_destroy)(void *),
       bool (*hash_object_comparator)(const void *, const void *),
       int (*hash_object_generate_value)(const void *));

/**
 * ht_destroy - Destroy an existing table and frees all memory which has been
 *   allocated by the table.
 */
void
ht_destroy(struct ht_table *hash_table);


#endif
