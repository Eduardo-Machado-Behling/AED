#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// Did it with some outside help with the intuition
bool canJump(const int *nums, int numsSize) {
  int reachable = nums[0];

  for (int i = 0; i <= reachable && reachable < numsSize; i++) {
    reachable = MAX(reachable, i + nums[i]);
  }

  return reachable >= numsSize - 1;
}
