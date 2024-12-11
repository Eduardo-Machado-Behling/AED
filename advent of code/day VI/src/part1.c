#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"

typedef struct {
  int x, y;
} vec2D_t;

void print(dArray_t *map) {
  for (size_t i = 0; i < DArrayUsed(map); i++) {
    dArray_t **row = DArrayAt(map, i);
    for (size_t j = 0; j < DArrayUsed(*row); j++) {
      char *ch = DArrayAt(*row, j);
      fputc(*ch, stdout);
    }
    fputc('\n', stdout);
  }
}

int GetVisitedPath(dArray_t *map, vec2D_t pos) {
  vec2D_t delta = {.x = 0, .y = -1};
  int count = 0;

  while (true) {
    pos.x += delta.x;
    pos.y += delta.y;

    dArray_t **row = DArrayAt(map, pos.y);
    if (!row) {
      return count;
    }

    char *tile = DArrayAt(*row, pos.x);

    if (!tile) {
      return count;
    }

    if (*tile == '#') {
      pos.x -= delta.x;
      pos.y -= delta.y;
      int temp = delta.x;
      delta.x = -1 * delta.y;
      delta.y = temp;
    } else if (*tile == '.') {
      *tile = 'X';
      count++;
    }
  }

  return count;
}

int main(void) {
  FILE *in = fopen("input.txt", "r");
  if (!in) {
    fprintf(stderr, "input file wasn't open for the following reason: \"%s\"\n",
            strerror(errno));
    return 1;
  }

  dArray_t *map =
      CreateDArray(64, sizeof(dArray_t *), (f_destructor)(DestroyDArray));
  dArray_t *row = NULL;
  int ch = 0;
  bool first = true;
  vec2D_t pos = {0};
  while ((ch = fgetc(in)) != EOF) {
    if (first || ch == '\n') {
      row = CreateDArray(64, sizeof(char), NULL);
      DArrayPush(map, (void *)&row);
      first = false;
    }

    if (isprint(ch)) {
      DArrayPush(row, &ch);
    }

    if (ch == '^') {
      pos.x = (int)DArrayUsed(row) - 1;
      pos.y = (int)DArrayUsed(map) - 1;
    }
  }

  print(map);
  int res = GetVisitedPath(map, pos);
  print(map);

  printf("res %d\n", res);
  fclose(in);
}
