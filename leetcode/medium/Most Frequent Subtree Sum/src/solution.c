#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

struct pair_t {
    int key;
    int val;
};

struct darray_t {
    int used;
    int size;
    int elementSize;
    uint8_t* data;
};

struct hashmap_t {
    struct darray_t* buckets;
    int bucketsAmount;
};

// O(1)
int hash( struct hashmap_t* hashmap, int key ) {
    return abs( key % hashmap->bucketsAmount );
}

// O(1)
void initDarray( struct darray_t* darray, int initialSize, int elementSize ) {
    darray->size        = initialSize;
    darray->used        = 0;
    darray->elementSize = elementSize;

    darray->data = malloc( initialSize * elementSize );
}

// O(1)
void appendDarray( struct darray_t* darray, void* val ) {
    if ( darray->used == darray->size ) {
        int newSize   = darray->size * 2;
        void* newData = realloc( darray->data, newSize * darray->elementSize );
        if ( !newData ) {
            return;
        }

        darray->data = newData;
        darray->size = newSize;
    }

    memcpy( darray->data + darray->used++ * darray->elementSize, val, darray->elementSize );
}

// O(1)
void* atDarray( struct darray_t* darray, int i ) {
    return darray->data + i * darray->elementSize;
}

// O(1)
void clearDarray( struct darray_t* darray ) {
    darray->used = 0;
}

// O(1)
void initHashmap( struct hashmap_t* hashmap, int bucketsAmount, int bucketsSize ) {
    hashmap->bucketsAmount = bucketsAmount;
    hashmap->buckets       = malloc( sizeof( *hashmap->buckets ) * bucketsAmount );

    for ( size_t i = 0; i < bucketsAmount; i++ ) {
        initDarray( hashmap->buckets + i, bucketsSize, sizeof( struct pair_t ) );
    }
}

// O(1)
void freeHashmap( struct hashmap_t* hashmap ) {
    for ( int i = 0; i < hashmap->bucketsAmount; i++ ) {
        free( hashmap->buckets[i].data );
    }
    free( hashmap->buckets );
}

// O(1)
struct pair_t* getHashmap( struct hashmap_t* hashmap, int key ) {
    int i = hash( hashmap, key );

    struct darray_t* bucket = hashmap->buckets + i;

    i = 0;
    for ( ; i < bucket->used; i++ ) {
        struct pair_t* p = atDarray( bucket, i );
        if ( p->key == key ) {
            return p;
        }
    }

    struct pair_t p = { .key = key, .val = 0 };
    appendDarray( bucket, &p );
    return atDarray( bucket, i );
}

// O(na)  | na = nodesAmount
int sumRecursive( struct TreeNode* root, struct hashmap_t* counter, struct darray_t* ret, int* max ) {
    if ( !root )
        return 0;

    int sum = root->val;
    sum += sumRecursive( root->left, counter, ret, max );
    sum += sumRecursive( root->right, counter, ret, max );

    struct pair_t* p = getHashmap( counter, sum );
    p->val++;
    if ( p->val >= *max ) {
        if ( p->val > *max ) {
            *max = p->val;
            clearDarray( ret );
        }
        appendDarray( ret, &( p->key ) );
    }

    return sum;
}

// O(n)
int* findFrequentTreeSum( struct TreeNode* root, int* returnSize ) {
    struct hashmap_t counter;
    struct darray_t ret;
    int max = 1;

    // O(1)
    initDarray( &ret, 10000, sizeof( int ) );
    // O(1)
    initHashmap( &counter, 20, 1000 );

    // O(n)
    sumRecursive( root, &counter, &ret, &max );
    *returnSize = ret.used;

    // O(1)
    freeHashmap( &counter );
    return (int*) ret.data;
}