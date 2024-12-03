#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *addSpaces(const char *s, const int *spaces, int spacesSize) {
  size_t newLen = strlen(s) + spacesSize + 1;
  char *res = malloc(newLen);

  size_t j = 0;
  for (size_t i = 0; i < newLen; i++) {
    if (i - j == spaces[j]) {
      res[i] = ' ';
      j++;
    } else {
      res[i] = *s++;
    }
  }

  return res;
}
