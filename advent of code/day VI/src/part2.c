#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "darray.h"

typedef struct {
  int x, y;
} vec2D_t;

enum dir_t {
  DIR_UP,
  DIR_RIGHT,
  DIR_DOWN,
  DIR_LEFT,

  DIR_COUNT,
};

const vec2D_t DELTAS[4] = {
    {.x = 0, .y = -1}, {.x = 1, .y = 0}, {.x = 0, .y = 1}, {.x = -1, .y = 0}};
const char PATH_SYMBOLS[4] = {'^', '>', 'v', '<'};

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

struct traverseState_t {
  vec2D_t pos;
  enum dir_t delta;
};

int HasLoop(dArray_t *map, vec2D_t pos, enum dir_t delta) {
  pos.x -= DELTAS[delta].x;
  pos.y -= DELTAS[delta].y;

  struct traverseState_t startState = {.pos = pos, .delta = delta};
  static dArray_t *wallsSeen = NULL;
  if (!wallsSeen) {
    wallsSeen = CreateDArray(255, sizeof(struct traverseState_t), NULL);
  } else {
    DArrayClear(wallsSeen);
  }

  delta = (delta + 1) % DIR_COUNT;
  while (true) {
    pos.x += DELTAS[delta].x;
    pos.y += DELTAS[delta].y;
    struct traverseState_t state = {.pos = pos, .delta = delta};
    if (memcmp(&startState, &state, sizeof(struct traverseState_t)) == 0) {
      return 0;
    }
    for (size_t i = 0; i < DArrayUsed(wallsSeen); i++) {
      if (memcmp(DArrayAt(wallsSeen, i), &state,
                 sizeof(struct traverseState_t)) == 0) {
        return 0;
      }
    }

    dArray_t **row = DArrayAt(map, pos.y);
    if (!row) {
      return 0;
    }

    char *tile = DArrayAt(*row, pos.x);

    if (!tile) {
      return 0;
    }

    if (*tile == '#') {
      DArrayPush(wallsSeen, &state);
      pos.x -= DELTAS[delta].x;
      pos.y -= DELTAS[delta].y;

      delta = (delta + 1) % DIR_COUNT;
    } else if (PATH_SYMBOLS[delta] == *tile) {
      return 1;
    }
  }
}

int GetVisitedPath(dArray_t *map, vec2D_t pos) {
  enum dir_t delta = DIR_UP;
  int count = 0;

  while (true) {
    pos.x += DELTAS[delta].x;
    pos.y += DELTAS[delta].y;

    dArray_t **row = DArrayAt(map, pos.y);
    if (!row) {
      return count;
    }

    char *tile = DArrayAt(*row, pos.x);

    if (!tile) {
      return count;
    }

    if (*tile == '#') {
      pos.x -= DELTAS[delta].x;
      pos.y -= DELTAS[delta].y;

      delta = (delta + 1) % DIR_COUNT;
    } else {
      printf("(%d, %d)\n", pos.x, pos.y);
      *tile = PATH_SYMBOLS[delta];
      if (HasLoop(map, pos, delta)) {
        count++;
        /*print(map);*/
        // ChangePath(map, pos, delta);
        printf("-----------\n\n");
      }
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

  /*print(map);*/
  int res = GetVisitedPath(map, pos);
  /*print(map);*/

  printf("res %d\n", res);
  fclose(in);
}
