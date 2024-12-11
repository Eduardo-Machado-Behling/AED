#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef size_t ( *f_hashfunc )( void* key );
typedef struct hashMap_t hashMap_t;

hashMap_t* CreateHashMap( size_t bucketsAmount, size_t bucketsInitialSize, size_t keySize, size_t elemSize, f_hashfunc hashFunc );

bool HashMapSet( const hashMap_t* hashmap, void* key, void* value );
void* HashMapGet( const hashMap_t* hashmap, void* key );

void HashMapClear( const hashMap_t* hashmap );

void DestroyHashMap( hashMap_t** hashmap );

#endif
