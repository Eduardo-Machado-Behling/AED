#include "solution.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

struct ListNode* addTwoNumbers( struct ListNode* l1, struct ListNode* l2 ) {
    struct ListNode* l  = malloc( sizeof( struct ListNode ) );
    struct ListNode* it = l;

    uint8_t carry = 0;
    while ( l1 || l2 || carry ) {
        int va = 0;
        if ( l1 ) {
            va = l1->val;
            l1 = l1->next;
        }
        int vb = 0;
        if ( l2 ) {
            vb = l2->val;
            l2 = l2->next;
        }

        int val = va + vb + carry;
        it->val = val % 10;
        carry   = val > 9;

        it->next = !( l1 || l2 ) && carry == 0 ? NULL : malloc( sizeof( struct ListNode ) );
        if ( it )
            it = it->next;
    }

    return l;
}
// struct ListNode* addTwoNumbers( struct ListNode* l1, struct ListNode* l2 ) {
//     struct {
//         uint8_t data[MAX_NODE_AMOUNT];
//         size_t used;
//     } stack = { .data = { 0 }, .used = 0 };

//     do {
//         int va = 0;
//         if ( l1 ) {
//             va = l1->val;
//             l1 = l1->next;
//         }
//         int vb = 0;
//         if ( l2 ) {
//             vb = l2->val;
//             l2 = l2->next;
//         }

//         stack.data[stack.used++] = va + vb;
//     } while ( l1 || l2 );

//     struct ListNode* l  = malloc( sizeof( struct ListNode ) );
//     struct ListNode* it = l;

//     uint8_t carry = 0;
//     for ( size_t i = 0; i < stack.used; i++ ) {
//         int val = stack.data[i] + carry;
//         it->val = val % 10;
//         carry   = val > 9;

//         it->next = i == stack.used - 1 ? NULL : malloc( sizeof( struct ListNode ) );
//         if ( it )
//             it = it->next;
//     }
// }