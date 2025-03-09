#include "unity.h"

#include "solution.h"
#include <stdlib.h>

struct TreeNode* createNode( int val ) {
    struct TreeNode* node = malloc( sizeof( struct TreeNode ) );

    node->val   = val;
    node->left  = NULL;
    node->right = NULL;

    return node;
}

void freeTree( struct TreeNode* root ) {
    if ( !root )
        return;

    freeTree( root->left );
    freeTree( root->right );

    printf( "free: root->val %d\n", root->val );
    free( root );
}

void setUp( void ) {
}

void tearDown( void ) {
}

//    5
//   / \
//  2  -3
void Test1( void ) {
    struct TreeNode* input = createNode( 5 );
    input->left            = createNode( 2 );
    input->right           = createNode( -3 );

    const int expected[]   = { 2, -3, 4 };
    const int expectedSize = sizeof( expected ) / sizeof( *expected );

    int returnSize;
    int* res = findFrequentTreeSum( input, &returnSize );

    TEST_ASSERT_EQUAL_INT_ARRAY( expected, res, returnSize );

    free( res );
    freeTree( input );
}

//    5
//   / \
//  2  -5
void Test2( void ) {
    struct TreeNode* input = createNode( 5 );
    input->left            = createNode( 2 );
    input->right           = createNode( -5 );

    const int expected[]   = { 2 };
    const int expectedSize = sizeof( expected ) / sizeof( *expected );

    int returnSize;
    int* res = findFrequentTreeSum( input, &returnSize );

    TEST_ASSERT_EQUAL_INT_ARRAY( expected, res, returnSize );

    free( res );
    freeTree( input );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( Test1 );
    RUN_TEST( Test2 );

    return UNITY_END();
}
