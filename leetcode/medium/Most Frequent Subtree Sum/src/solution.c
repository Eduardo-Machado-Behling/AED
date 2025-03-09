#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

// O(na)  | na = nodesAmount
int sumRecursive( struct TreeNode* root, int** ret, int* size, int* returnSize ) {
    if ( !root )
        return 0;

    int sum = root->val;
    sum += sumRecursive( root->left, ret, size, returnSize );
    sum += sumRecursive( root->right, ret, size, returnSize );
    printf( "sum: %d\n", sum );
    if ( *returnSize == *size ) {
        printf( "Realloc" );
        int newSize  = *size * 2;
        void* newRet = realloc( *ret, sizeof( int ) * newSize );
        if ( !newRet ) {
            printf( "Cooked!!\n" );
            return 0;
        }
        *ret  = newRet;
        *size = newSize;
    }
    ret[0][( *returnSize )++] = sum;
    return sum;
}

struct pair_t {
    int key;
    int count;
};

// O (n²)
int getMost( int* ret, int returnSize ) {
    struct pair_t* aux = malloc( sizeof( struct pair_t ) * returnSize );
    int max            = 1;

    // O(n²)
    for ( int i = 0; i < returnSize; i++ ) {
        int found        = 0;
        struct pair_t* p = NULL;

        // O(i)
        for ( int j = 0; j < i; j++ ) {
            if ( aux[j].key == ret[i] ) {
                aux[j].count++;
                p     = aux + j;
                found = 1;
                break;
            }
        }
        if ( !found ) {
            aux[i].key   = ret[i];
            aux[i].count = 1;
            p            = aux + i;
        }

        if ( p->count > max ) {
            max = p->count;
        }
    }

    printf( "aux:\n" );
    for ( int i = 0; i < returnSize; i++ ) {
        printf( "\t[%d]: (%d, %d)\n", i, aux[i].key, aux[i].count );
    }

    int s = 0;
    // O(n)
    for ( int i = 0; i < returnSize; i++ ) {
        if ( aux[i].count == max ) {
            ret[s++] = aux[i].key;
        }
    }

    free( aux );
    return s;
}

// O(na² + na)
// O(na²)
int* findFrequentTreeSum( struct TreeNode* root, int* returnSize ) {
    int size    = 10000;
    int* ret    = malloc( sizeof( int ) * size );
    *returnSize = 0;

    sumRecursive( root, &ret, &size, returnSize );
    *returnSize = getMost( ret, *returnSize );
    return ret;
}