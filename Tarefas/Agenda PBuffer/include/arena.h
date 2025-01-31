#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>

#define ARENA_ACCESSORS_INITIAL_SIZE 20U
#define ARENA_ACCESSORS_GROWTH_AMOUNT 20U

#define ARENA_DATA_INITIAL_SIZE 200U
#define ARENA_DATA_GROWTH_AMOUNT 200U

#define ARENA_HEADER_SIZE 16U

#define ARENA_INVALID_ACCESSOR (arenaAccessor_t *)UINT32_MAX

/*
 * Arena Layout:
 * Offset (B) |     Type    | Content                 | Reason            |
 *      0     | uint32_t    | Multi Purpose Arg       |  Function Args    |
 *      4     | uint32_t    | Buffer Capacity (B)     |  Accessers        |
 *      8     | uint32_t    | Buffer Used (B)         |  Data             |
 *     12     | uint32_t    | Buffer Capacity (B)     |  Data             |
 *     16     | void*       | Data Array              |  Data storage     |
 *     ...
 */

#define ARENA_ALLOC(BYTES)                                                     \
  (*ArenaGetArg(*GetArenaSingleton()) = (BYTES),                               \
   ArenaAlloc(GetArenaSingleton()))

#define ARENA_REALLOC(BLOCK, BYTES)                                            \
  (*ArenaGetArg(*GetArenaSingleton()) = (BYTES),                               \
   ArenaRealloc(GetArenaSingleton(), BLOCK))

#define ARENA_FREE(BLOCK) ArenaFree(*GetArenaSingleton(), BLOCK)

#define ARENA_GETI(BLOCK) ARENA_GET(__typeof__(*BLOCK), BLOCK)

#define ARENA_GET(DATATYPE, BLOCK)                                             \
  ((DATATYPE *)(ArenaGetData(*GetArenaSingleton(), (arenaAccessor_t *)(BLOCK))))

#define ARENA_STATIC_INITIALIZER_HELPER(TYPE, VAR, BYTES)                      \
  static TYPE *VAR = ARENA_INVALID_ACCESSOR;                                   \
  if (VAR == ARENA_INVALID_ACCESSOR) {                                         \
    VAR = ARENA_ALLOC(BYTES);                                                  \
  }

#define ARENA_SIH(TYPE, VAR)                                                   \
  ARENA_STATIC_INITIALIZER_HELPER(TYPE, VAR, sizeof(TYPE))
#define ARENA_SIHB(TYPE, VAR, BYTES)                                           \
  ARENA_STATIC_INITIALIZER_HELPER(TYPE, VAR, BYTES)

typedef void arenaAccessor_t;

void **GetArenaSingleton(void);

uint32_t *ArenaGetArg(void *arena);
void *ArenaGetData(void *arena, const arenaAccessor_t *block);

arenaAccessor_t *ArenaAlloc(void **arena);
arenaAccessor_t *ArenaRealloc(void **arena, arenaAccessor_t *block);
void ArenaFree(void *arena, void *block);

#endif
