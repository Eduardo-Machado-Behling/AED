#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

int* findFrequentTreeSum( struct TreeNode* root, int* returnSize );