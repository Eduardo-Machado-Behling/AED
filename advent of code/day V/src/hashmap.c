#include "hashmap.h"
#include "darray.h"

#include <stdlib.h>
#include <string.h>

typedef struct hashMap_t {
    dArray_t** buckets;
    size_t bucketsAmount;
    size_t elemSize;
    size_t keySize;
    f_hashfunc hashFunc;
} hashMap_t;

struct hashmapEntry_t {
    void* key;
    void* value;
};

static void DestroyEntry( void* entryPtr ) {
    if ( !entryPtr )
        return;

    struct hashmapEntry_t* entry = entryPtr;
    if ( entry->key )
        free( entry->key );

    if ( entry->value )
        free( entry->value );
}

hashMap_t* CreateHashMap( size_t bucketsAmount, size_t bucketsInitialSize, size_t keySize, size_t elemSize, f_hashfunc hashFunc ) {
    hashMap_t* hashmap = malloc( sizeof( hashMap_t ) );
    if ( !hashmap ) {
        return NULL;
    }

    hashmap->bucketsAmount = bucketsAmount;
    hashmap->elemSize      = elemSize;
    hashmap->keySize       = keySize;
    hashmap->hashFunc      = hashFunc;
    hashmap->buckets       = malloc( sizeof( dArray_t* ) * bucketsAmount );
    if ( !hashmap->buckets ) {
        DestroyHashMap( &hashmap );
        return NULL;
    }

    for ( size_t i = 0; i < bucketsAmount; i++ ) {
        hashmap->buckets[i] = CreateDArray( bucketsInitialSize, sizeof( struct hashmapEntry_t ), (f_destructor) ( DestroyEntry ) );
        if ( !hashmap->buckets[i] ) {
            DestroyHashMap( &hashmap );
            return NULL;
        }
    }

    return hashmap;
}

bool HashMapSet( const hashMap_t* hashmap, void* key, void* value ) {
    size_t bucketIndex = hashmap->hashFunc( key ) % hashmap->bucketsAmount;

    for ( size_t i = 0; i < DArrayUsed( hashmap->buckets[bucketIndex] ); i++ ) {
        struct hashmapEntry_t* entry = DArrayAt( hashmap->buckets[bucketIndex], i );
        if ( memcmp( key, entry->key, hashmap->keySize ) == 0 ) {
            memcpy( entry->value, value, hashmap->elemSize );
            return true;
        }
    }

    struct hashmapEntry_t entry = { .key = NULL, .value = NULL };

    entry.key = malloc( hashmap->keySize );
    if ( !entry.key ) {
        return false;
    }
    memcpy( entry.key, key, hashmap->keySize );

    entry.value = malloc( hashmap->elemSize );
    if ( !entry.value ) {
        free( entry.key );
        return false;
    }
    memcpy( entry.value, value, hashmap->elemSize );

    DArrayPush( hashmap->buckets[bucketIndex], &entry );
}

void* HashMapGet( const hashMap_t* hashmap, void* key ) {
    size_t bucketIndex = hashmap->hashFunc( key ) % hashmap->bucketsAmount;

    for ( size_t i = 0; i < DArrayUsed( hashmap->buckets[bucketIndex] ); i++ ) {
        struct hashmapEntry_t* entry = DArrayAt( hashmap->buckets[bucketIndex], i );
        if ( memcmp( key, entry->key, hashmap->keySize ) == 0 ) {
            return entry->value;
        }
    }

    return NULL;
}

void HashMapClear( const hashMap_t* hashmap ) {
    for ( size_t i = 0; i < hashmap->bucketsAmount; i++ ) {
        DArrayClear( hashmap->buckets[i] );
    }
}

void DestroyHashMap( hashMap_t** hashmap ) {
    if ( !hashmap )
        return;

    hashMap_t* hm = *hashmap;
    *hashmap      = NULL;

    if ( !hm )
        return;

    if ( hm->buckets ) {
        for ( size_t i = 0; i < hm->bucketsAmount; i++ ) {
            dArray_t* da = hm->buckets[i];
            DestroyDArray( &da );
        }
        free( hm->buckets );
    }

    free( hm );
}