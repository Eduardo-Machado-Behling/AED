#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>

#define ARENA_ACCESSORS_INITIAL_SIZE 24U
#define ARENA_ACCESSORS_GROWTH_AMOUNT 24U

#define ARENA_DATA_INITIAL_SIZE 200U
#define ARENA_DATA_GROWTH_AMOUNT 200U

#define ARENA_HEADER_SIZE 20U

#define ARENA_INVALID_ACCESSOR (arenaAccessor_t*) UINT32_MAX

/*
 * Arena Layout:
 * Offset (B) |     Type    | Content                 | Reason            |
 *      0     | uint32_t    | Multi Purpose Arg       |  Function Args    |
 *      8     | uint32_t    | Buffer Capacity (B)     |  Accessers        |
 *     12     | uint32_t    | Buffer Used (B)         |  Data             |
 *     16     | uint32_t    | Buffer Capacity (B)     |  Data             |
 *     20     | void*       | Data Array              |  Data storage     |
 *     ...
 */

#define ARENA_ALLOC( VAR, BYTES )                       \
    ( *ArenaGetArg( *GetArenaSingleton() ) = ( BYTES ), \
      ArenaAlloc( GetArenaSingleton(), (void**) &( VAR ) ) )

#define ARENA_REALLOC( BLOCK, BYTES )                   \
    ( *ArenaGetArg( *GetArenaSingleton() ) = ( BYTES ), \
      ArenaRealloc( GetArenaSingleton(), &BLOCK ) )

#define ARENA_FREE( BLOCK ) ArenaFree( *GetArenaSingleton(), &BLOCK )

#define ARENA_STATIC_INITIALIZER_HELPER( TYPE, VAR, BYTES ) \
    static TYPE* VAR = NULL;                                \
    if ( !( VAR ) ) {                                       \
        ARENA_ALLOC( VAR, BYTES );                          \
    }

#define ARENA_SIH( TYPE, VAR ) \
    ARENA_STATIC_INITIALIZER_HELPER( TYPE, VAR, sizeof( TYPE ) )
#define ARENA_SIHB( TYPE, VAR, BYTES ) \
    ARENA_STATIC_INITIALIZER_HELPER( TYPE, VAR, BYTES )

typedef void arenaAccessor_t;

void** GetArenaSingleton( void );

uint64_t* ArenaGetArg( void* arena );

void ArenaAlloc( void** arena, void** block );
void* ArenaRealloc( void** arena, void** block );
void ArenaFree( void* arena, void** block );

#endif
