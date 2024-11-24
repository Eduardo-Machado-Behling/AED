#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct ListNode {
    int val;
    struct ListNode* next;
};

struct ListNode* createNode( int val );

struct ListNode* addTwoNumbers( struct ListNode* l1, struct ListNode* l2 );