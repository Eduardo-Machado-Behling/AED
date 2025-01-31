#ifndef COMMON_H
#define COMMON_H

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

#define OFFSET_BY_TYPED(TYPE, BUFFER, BYTES)                                   \
  ((TYPE)((uint8_t *)(BUFFER) + (BYTES)))
#define OFFSET_BY(BUFFER, BYTES) ((void *)((uint8_t *)(BUFFER) + (BYTES)))
#define GET_OFFSET(A, B)                                                       \
  (((uint8_t *)MAX((uint8_t *)A, (uint8_t *)B)) -                              \
   ((uint8_t *)MIN((uint8_t *)A, (uint8_t *)B)))

#define RESIZING_FORMULA(AVAILABLE_BYTES, BYTES_TO_ALLOCATE, GROWTH_RATE)      \
  ((((BYTES_TO_ALLOCATE) - (AVAILABLE_BYTES)) / (GROWTH_RATE) + 1) *           \
   (GROWTH_RATE))

#endif
