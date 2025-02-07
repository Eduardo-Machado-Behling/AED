#include "solution.h"

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_CLASSES 100000

struct entry {
  int students;
  int passed;
  int reps;
};

void appendMin(struct entry minStack[MAX_CLASSES], int *used,
               struct entry entry) {
  if (*used == MAX_CLASSES) {
    return;
  }

  int i = *used - 1;
  for (; i >= 0 && minStack[i].students < entry.students; i--) {
    minStack[i + 1] = minStack[i];
  }

  for (; i >= 0 && minStack[i].students == entry.students &&
         minStack[i].reps > entry.reps;
       i--) {
    minStack[i + 1] = minStack[i];
  }

  minStack[i + 1] = entry;
  (*used)++;
}

double maxAverageRatio(int **classes, int classesSize, int *classesColSize,
                       int extraStudents) {
  struct entry minStack[MAX_CLASSES];
  int used = 0;

  for (int i = 0; i < classesSize; i++) {
    int reps = classes[i][1] - classes[i][0];
    appendMin(minStack, &used,
              (struct entry){.students = classes[i][1],
                             .passed = classes[i][0],
                             .reps = reps});
  }

  int offset = 0;
  for (int i = 0; i < extraStudents; i++) {
    int ri = used - offset - 1;
    struct entry *entry = minStack + ri;

    if (ri > 0) {
      if (entry->students > entry[-1].students && entry[-1].reps != 0) {
        offset++;
        entry--;
      }
    }

    ++(entry->students);
    ++(entry->passed);
  }

  double res = 0;
  for (int i = 0; i < used; i++) {
    res += (minStack[i].passed) / (double)(minStack[i].students);
  }
  res /= used;

  return res;
}
