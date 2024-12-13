#include "solution.h"

#include <stdlib.h>

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume
 * caller calls free().
 */
int **generate(int numRows, int *returnSize, int **returnColumnSizes) {
  int *columnsSize = (int *)malloc(numRows * sizeof(int));
  int **res = (int **)malloc(numRows * sizeof(int *));

  // For early return cases
  *returnColumnSizes = NULL;
  *returnSize = 0;

  // Failed to allocate memory
  if (!res || !columnsSize) {
    return NULL;
  }

  for (int i = 0; i < numRows; i++) {
    columnsSize[i] = i + 1;
    res[i] = malloc(sizeof(int) * columnsSize[i]);
    if (!res[i]) {
      return NULL;
    }

    int currColumn = 0;

    res[i][currColumn++] = 1;
    for (; i > 0 && currColumn < i; currColumn++) {
      res[i][currColumn] = res[i - 1][currColumn - 1] + res[i - 1][currColumn];
    }
    if (currColumn < columnsSize[i]) {
      res[i][currColumn] = 1;
    }
  }

  *returnColumnSizes = columnsSize;
  *returnSize = numRows;
  return res;
}
