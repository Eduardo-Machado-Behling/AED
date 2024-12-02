#include "solution.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int isPrefixOfWord(const char *sentence, const char *searchWord) {
  const char *word = sentence;
  size_t prefixLen = strlen(searchWord);
  int currentWord = 1;

  for (; true; currentWord++) {
    const char *end = strchr(word, ' ');

    if (strncmp(searchWord, word, prefixLen) == 0) {
      break;
    }

    if (!end) {
      word = end;
      break;
    }
    word = ++end;
  }

  return !word ? -1 : currentWord;
}
