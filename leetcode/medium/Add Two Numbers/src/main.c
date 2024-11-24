#include "unity.h"

#include "solution.h"
#include <stdlib.h>

struct ListNode* createNode( int val ) {
    struct ListNode* res = malloc( sizeof( struct ListNode ) );
    res->next            = NULL;
    res->val             = val;

    return res;
}

struct ListNode* listFromArray( int* arr, int size ) {
    struct ListNode* l = createNode( arr[0] );

    struct ListNode* it = l;
    for ( size_t i = 1; i < size; i++ ) {
        it->next = createNode( arr[i] );
        it       = it->next;
    }

    return l;
}

void eraseList( struct ListNode* l ) {
    struct ListNode *prev = l, *curr = l->next;

    do {
        free( prev );

        prev = curr;
        if ( curr )
            curr = curr->next;
    } while ( curr );
}

bool equalList( struct ListNode* a, struct ListNode* b ) {
    while ( a->next && b->next ) {
        if ( a->val != b->val )
            return false;

        a = a->next;
        b = b->next;
    }

    if ( a->next != b->next )
        return false;

    return true;
}

void setUp( void ) {
}

void tearDown( void ) {
}

void test_1( void ) {
    int a1[]            = { 2, 4, 3 };
    int a2[]            = { 5, 6, 4 };
    int expectedArray[] = { 7, 0, 8 };

    struct ListNode* l1 = listFromArray( a1, sizeof( a1 ) / sizeof( int ) );
    struct ListNode* l2 = listFromArray( a2, sizeof( a2 ) / sizeof( int ) );

    struct ListNode* le = listFromArray( expectedArray, sizeof( expectedArray ) / sizeof( int ) );
    struct ListNode* lr = addTwoNumbers( l1, l2 );

    TEST_ASSERT_TRUE( equalList( le, lr ) );
    eraseList( l1 );
    eraseList( l2 );
    eraseList( le );
    eraseList( lr );
}

void test_2( void ) {
    int a1[]            = { 0 };
    int a2[]            = { 0 };
    int expectedArray[] = { 0 };

    struct ListNode* l1 = listFromArray( a1, sizeof( a1 ) / sizeof( int ) );
    struct ListNode* l2 = listFromArray( a2, sizeof( a2 ) / sizeof( int ) );

    struct ListNode* le = listFromArray( expectedArray, sizeof( expectedArray ) / sizeof( int ) );
    struct ListNode* lr = addTwoNumbers( l1, l2 );

    TEST_ASSERT_TRUE( equalList( le, lr ) );
    eraseList( l1 );
    eraseList( l2 );
    eraseList( le );
    eraseList( lr );
}

void test_3( void ) {
    int a1[]            = { 9, 9, 9, 9, 9, 9, 9 };
    int a2[]            = { 9, 9, 9, 9 };
    int expectedArray[] = { 8, 9, 9, 9, 0, 0, 0, 1 };

    struct ListNode* l1 = listFromArray( a1, sizeof( a1 ) / sizeof( int ) );
    struct ListNode* l2 = listFromArray( a2, sizeof( a2 ) / sizeof( int ) );

    struct ListNode* le = listFromArray( expectedArray, sizeof( expectedArray ) / sizeof( int ) );
    struct ListNode* lr = addTwoNumbers( l1, l2 );

    TEST_ASSERT_TRUE( equalList( le, lr ) );
    TEST_ASSERT_TRUE( equalList( le, lr ) );
    eraseList( l1 );
    eraseList( l2 );
    eraseList( le );
    eraseList( lr );
}

void test_4( void ) {
    int a1[]            = { 8, 3, 2 };
    int a2[]            = { 9, 2, 1 };
    int expectedArray[] = { 7, 6, 3 };

    struct ListNode* l1 = listFromArray( a1, sizeof( a1 ) / sizeof( int ) );
    struct ListNode* l2 = listFromArray( a2, sizeof( a2 ) / sizeof( int ) );

    struct ListNode* le = listFromArray( expectedArray, sizeof( expectedArray ) / sizeof( int ) );
    struct ListNode* lr = addTwoNumbers( l1, l2 );

    TEST_ASSERT_TRUE( equalList( le, lr ) );
    TEST_ASSERT_TRUE( equalList( le, lr ) );
    eraseList( l1 );
    eraseList( l2 );
    eraseList( le );
    eraseList( lr );
}

int main( void ) {
    UNITY_BEGIN();

    RUN_TEST( test_1 );
    RUN_TEST( test_2 );
    RUN_TEST( test_3 );
    RUN_TEST( test_4 );

    return UNITY_END();
}
