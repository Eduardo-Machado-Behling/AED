#include "arena.h"
#include "common.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_INITIAL_SIZE \
    ( ARENA_DATA_INITIAL_SIZE + ARENA_ACCESSORS_INITIAL_SIZE + ARENA_HEADER_SIZE )

#define ARENA_AVAILABLE_ACCESSOR_VALUE UINT32_MAX

#define ARENA_INVALID_DATA_OFFSET UINT32_MAX

#define ARENA_GET_OVERALL_USED( ARENA )                               \
    ( *ArenaGetAccessCapacity( ARENA ) + *ArenaGetDataUsed( ARENA ) + \
      ARENA_HEADER_SIZE )

#define ARENA_GET_OVERALL_CAPACITY( ARENA )                               \
    ( *ArenaGetAccessCapacity( ARENA ) + *ArenaGetDataCapacity( ARENA ) + \
      ARENA_HEADER_SIZE )

#define ARENA_GET_AVAILABLE_MEMORY_DATA( ARENA ) \
    ( *ArenaGetDataCapacity( ARENA ) - *ArenaGetDataUsed( ARENA ) )

#ifdef __DEBUG__
#include <stdio.h>
void dumpArena( void* arena, const char* name );
#endif

inline static uint32_t* ArenaGetAccessCapacity( void* arena );
inline static uint32_t* ArenaGetDataUsed( void* arena );
inline static uint32_t* ArenaGetDataCapacity( void* arena );

inline static void*** ArenaGetAccessorsSegment( void* arena );
inline static void* ArenaGetDataSegment( void* arena );

inline static uint32_t* ArenaGetBlockSize( void* accessor );

inline static void Swap( void* a, void* b );
inline static void MoveMemBlock( void* a, void* aEnd );

static void ArenaPushAccesser( void** arena, void** var );
static void* ArenaPushData( void** arena );

static void*** ArenaFindAvailableAccessor( void* arena );
static void ArenaTransferAccessors( void* newArena, void* oldArena, void** accessor,
                                    const uint32_t* size );
static void ArenaUpdateAccessors( void* arena, void** accessor,
                                  const uint32_t* size );

static void* CreateArena( void );
static void DestroyArena( void** arena );

static void* gArenaInstace = NULL;

void* CreateArena( void ) {
    void* arena = malloc( ARENA_INITIAL_SIZE );
    if ( !arena ) {
        return NULL;
    }
    memset( arena, 0x00, ARENA_INITIAL_SIZE );

    *ArenaGetDataUsed( arena )       = 0;
    *ArenaGetDataCapacity( arena )   = ARENA_DATA_INITIAL_SIZE;
    *ArenaGetAccessCapacity( arena ) = ARENA_ACCESSORS_INITIAL_SIZE;
    *ArenaGetArg( arena )            = 0U;

    return arena;
}

static void DestroyArenaHelper( void ) {
    DestroyArena( &gArenaInstace );
}

void** GetArenaSingleton( void ) {
    if ( !gArenaInstace ) {
        gArenaInstace = CreateArena();
        if ( !gArenaInstace ) {
            return NULL;
        }
        if ( atexit( DestroyArenaHelper ) ) {
            return NULL;
        }
    }

    return &gArenaInstace;
}

uint64_t* ArenaGetArg( void* arena ) {
    return (uint64_t*) arena;
}

void ArenaAlloc( void** arena, void** block ) {
    *block = ArenaPushData( arena );
    ArenaPushAccesser( arena, block );
}

void ArenaFree( void* arena, void** block ) {
    void** accessor = block;
    uint32_t* size  = ArenaGetBlockSize( *accessor );
    void* data      = *accessor;

    // store for later, because reorganizing the memory
    // changes the address of size.
    uint64_t* deletedSize = ArenaGetArg( arena );
    *deletedSize          = *size;

    // If isn't the in the end of the dataBuffer, need to
    // reorganize
    if ( *ArenaGetDataUsed( arena ) - (uint32_t) GET_OFFSET( *accessor, ArenaGetDataSegment( arena ) ) > *size + sizeof( uint32_t ) ) {
        memmove( size, OFFSET_BY( data, *size ),
                 *ArenaGetDataUsed( arena ) - GET_OFFSET( *accessor, ArenaGetDataSegment( arena ) ) - *size );

        ArenaUpdateAccessors( arena, accessor, size );
    }

    *accessor = NULL;
    *ArenaGetDataUsed( arena ) -= sizeof( uint32_t ) + *deletedSize;
}

void* ArenaRealloc( void** arena, void** block ) {
    void** accessor   = block;
    uint32_t* size    = ArenaGetBlockSize( accessor );
    void* data        = *block;
    uint64_t* newSize = ArenaGetArg( *arena );

    if ( ARENA_GET_AVAILABLE_MEMORY_DATA( *arena ) + *size < *newSize ) {
        void* newBlock =
            malloc( ARENA_GET_OVERALL_CAPACITY( *arena ) +
                    RESIZING_FORMULA( ARENA_GET_AVAILABLE_MEMORY_DATA( *arena ),
                                      ( *newSize - *size ), ARENA_DATA_GROWTH_AMOUNT ) );
        if ( !newBlock ) {
            return NULL;
        }

        memset( newBlock, 0x00,
                ARENA_GET_OVERALL_CAPACITY( *arena ) +
                    RESIZING_FORMULA( ARENA_GET_AVAILABLE_MEMORY_DATA( *arena ),
                                      ( *newSize - *size ), ARENA_DATA_GROWTH_AMOUNT ) );

        // Copy the Header, Accessors and Data from oldBlock until block
        memcpy( newBlock, *arena,
                ARENA_HEADER_SIZE + *ArenaGetAccessCapacity( *arena ) +
                    GET_OFFSET( size, ArenaGetDataSegment( *arena ) ) );

        // Copy the remaining data, skipping the current block
        memcpy( OFFSET_BY( ArenaGetDataSegment( newBlock ),
                           GET_OFFSET( size, ArenaGetDataSegment( *arena ) ) ),
                OFFSET_BY( data, *size ),
                *ArenaGetDataUsed( *arena ) -
                    GET_OFFSET( size, ArenaGetDataSegment( *arena ) ) - *size -
                    sizeof( uint32_t )

        );

        *ArenaGetDataCapacity( newBlock ) +=
            RESIZING_FORMULA( ARENA_GET_AVAILABLE_MEMORY_DATA( *arena ),
                              ( *newSize - *size ), ARENA_DATA_GROWTH_AMOUNT );
        *ArenaGetDataUsed( newBlock ) += *newSize - *size;

        uint32_t* dest =
            OFFSET_BY( ArenaGetDataSegment( newBlock ),
                       *ArenaGetDataUsed( *arena ) - *size - sizeof( uint32_t ) );
        *dest = *newSize;
        memcpy( OFFSET_BY( dest, sizeof( uint32_t ) ), data, *size );

        ArenaTransferAccessors( newBlock, *arena, accessor, size );

        free( *arena );
        *arena = newBlock;
    } else {
        // isn't last block
        if ( *ArenaGetDataUsed( *arena ) - GET_OFFSET( data, ArenaGetDataSegment( *arena ) ) > *size ) {
            uint8_t* i = OFFSET_BY( data, *size );
            for ( ; i !=
                    OFFSET_BY( ArenaGetDataSegment( *arena ), *ArenaGetDataUsed( *arena ) );
                  i++ ) {
                uint32_t* toSwap = OFFSET_BY( i, -( *size + sizeof( uint32_t ) ) );
                MoveMemBlock( toSwap, i );
                size = OFFSET_BY( size, 1 );
            }

            ArenaUpdateAccessors( *arena, accessor, size );
            *accessor = OFFSET_BY( size, sizeof( uint32_t ) );
        }

        *ArenaGetDataUsed( *arena ) += *newSize - *size;
        *size = *newSize;
    }

    return *block;
}

static void DestroyArena( void** arena ) {
    if ( !arena ) {
        return;
    }

    void* da = *arena;
    *arena   = NULL;
    if ( da ) {
        free( da );
    }
}

static void*** ArenaFindAvailableAccessor( void* arena ) {
    for ( void*** i = ArenaGetAccessorsSegment( arena );
          i != ArenaGetDataSegment( arena ); i++ ) {

        if ( *i == NULL ) {
            return i;
        }
    }

    return NULL;
}

static void ArenaPushAccesser( void** arena, void** var ) {
    void* offsetToData = (void*) *ArenaGetArg( *arena );
    if ( !offsetToData ) {
        return;
    }

    void*** availableAccessor = ArenaFindAvailableAccessor( *arena );
    if ( availableAccessor ) {
        *availableAccessor = var;
        return;
    }

    // Couldn't find a Accessor Available, allocate more accessor
    void* newBlock = malloc(
        ARENA_GET_OVERALL_CAPACITY( *arena ) +
        RESIZING_FORMULA( 0, sizeof( uint32_t ), ARENA_ACCESSORS_GROWTH_AMOUNT ) );
    if ( !newBlock ) {
        return;
    }

    memset( newBlock, 00,
            ARENA_GET_OVERALL_CAPACITY( *arena ) +
                RESIZING_FORMULA( 0, sizeof( uint32_t ), ARENA_ACCESSORS_GROWTH_AMOUNT ) );

    memcpy( newBlock, *arena, ARENA_HEADER_SIZE + *ArenaGetAccessCapacity( *arena ) );
    *ArenaGetAccessCapacity( newBlock ) += ARENA_ACCESSORS_GROWTH_AMOUNT;

    void*** it     = ArenaGetAccessorsSegment( newBlock );
    void*** itOrig = ArenaGetAccessorsSegment( *arena );
    for ( ; itOrig != ArenaGetDataSegment( *arena ); it++, itOrig++ ) {
        if ( *itOrig ) {
            **it = OFFSET_BY( newBlock, GET_OFFSET( **itOrig, *arena ) + ARENA_ACCESSORS_GROWTH_AMOUNT );
        }
    }

    memcpy( ArenaGetDataSegment( newBlock ), ArenaGetDataSegment( *arena ),
            *ArenaGetDataUsed( *arena ) );

    *it  = var;
    **it = OFFSET_BY( newBlock, GET_OFFSET( *var, *arena ) + ARENA_ACCESSORS_GROWTH_AMOUNT );

    free( *arena );
    *arena = newBlock;
}

static void* ArenaPushData( void** arena ) {
    uint64_t* amountBytesToAllocate = ArenaGetArg( *arena );
    if ( *ArenaGetDataCapacity( *arena ) - *ArenaGetDataUsed( *arena ) <
         *amountBytesToAllocate + sizeof( uint32_t ) ) {
        void* newBlock =
            malloc( ARENA_GET_OVERALL_CAPACITY( *arena ) +
                    RESIZING_FORMULA( *ArenaGetDataCapacity( *arena ) -
                                          *ArenaGetDataUsed( *arena ),
                                      *amountBytesToAllocate + sizeof( uint32_t ),
                                      ARENA_DATA_GROWTH_AMOUNT ) );
        if ( !newBlock ) {
            *ArenaGetArg( *arena ) = ARENA_INVALID_DATA_OFFSET;
            return NULL;
        }

        memset( newBlock, 0x00,
                ARENA_GET_OVERALL_CAPACITY( *arena ) +
                    RESIZING_FORMULA( *ArenaGetDataCapacity( *arena ) -
                                          *ArenaGetDataUsed( *arena ),
                                      *amountBytesToAllocate + sizeof( uint32_t ),
                                      ARENA_DATA_GROWTH_AMOUNT ) );

        // Copy the Header, Accessors and Data from oldBlock
        memcpy( newBlock, *arena,
                ARENA_HEADER_SIZE + *ArenaGetAccessCapacity( *arena ) +
                    *ArenaGetDataUsed( *arena ) );

        *ArenaGetDataCapacity( newBlock ) += RESIZING_FORMULA(
            *ArenaGetDataCapacity( *arena ) - *ArenaGetDataUsed( *arena ),
            *amountBytesToAllocate + sizeof( uint32_t ), ARENA_DATA_GROWTH_AMOUNT );

        void*** it     = ArenaGetAccessorsSegment( newBlock );
        void*** itOrig = ArenaGetAccessorsSegment( *arena );
        for ( ; itOrig != ArenaGetDataSegment( *arena ); it++, itOrig++ ) {
            if ( *itOrig ) {
                **it = OFFSET_BY( newBlock, GET_OFFSET( **itOrig, *arena ) + ARENA_ACCESSORS_GROWTH_AMOUNT );
            }
        }

        free( *arena );
        *arena                = newBlock;
        amountBytesToAllocate = ArenaGetArg( *arena );
    }

    uint32_t* data =
        OFFSET_BY( ArenaGetDataSegment( *arena ), *ArenaGetDataUsed( *arena ) );

    *data = *amountBytesToAllocate;
    *ArenaGetDataUsed( *arena ) += *amountBytesToAllocate + sizeof( uint32_t );

    return OFFSET_BY( data, sizeof( uint32_t ) );
}

static void ArenaTransferAccessors( void* newArena, void* oldArena, void** accessor,
                                    const uint32_t* size ) {
    for ( void*** j = ArenaGetAccessorsSegment( oldArena );
          j != ArenaGetDataSegment( oldArena ); j++ ) {
        if ( !*j ) {
            continue;
        }

        if ( **j > *accessor ) {
            **j = OFFSET_BY( newArena, GET_OFFSET( j, oldArena ) );
        } else {
            **j = OFFSET_BY( newArena, GET_OFFSET( j, oldArena ) - ( *size + sizeof( uint32_t ) ) );
        }
    }
}

static void ArenaUpdateAccessors( void* arena, void** accessor,
                                  const uint32_t* size ) {
    for ( void*** j = ArenaGetAccessorsSegment( arena );
          j != ArenaGetDataSegment( arena ); j++ ) {
        if ( *j && **j > *accessor ) {
            **j = OFFSET_BY( **j, -( *size + sizeof( uint32_t ) ) );
        }
    }
}

inline static uint32_t* ArenaGetAccessCapacity( void* arena ) {
    return OFFSET_BY( arena, 8U );
}
inline static uint32_t* ArenaGetDataUsed( void* arena ) {
    return OFFSET_BY( arena, 12U );
}
inline static uint32_t* ArenaGetDataCapacity( void* arena ) {
    return OFFSET_BY( arena, 16U );
}

inline static void*** ArenaGetAccessorsSegment( void* arena ) {
    return OFFSET_BY( arena, ARENA_HEADER_SIZE );
}

inline static void* ArenaGetDataSegment( void* arena ) {
    return OFFSET_BY( arena, *ArenaGetAccessCapacity( arena ) + ARENA_HEADER_SIZE );
}

inline static void Swap( void* a, void* b ) {
    *(uint8_t*) a ^= *(uint8_t*) b;
    *(uint8_t*) b ^= *(uint8_t*) a;
    *(uint8_t*) a ^= *(uint8_t*) b;
}

inline static void MoveMemBlock( void* a, void* aEnd ) {
    for ( uint8_t* it = aEnd; a != it; it-- ) {
        Swap( it, it - 1 );
    }
}

inline static uint32_t* ArenaGetBlockSize( void* accessor ) {
    return (uint32_t*) OFFSET_BY( accessor, -4 );
}

inline static void* ArenaGetBlockDataFromSize( uint32_t* size ) {
    return OFFSET_BY( size, sizeof( uint32_t ) );
}

#ifdef __DEBUG__
void dumpArena( void* arena, const char* name ) {
    FILE* file = fopen( name, "w" );
    if ( !file ) {
        printf( "Failed to dump arena!\n" );
        return;
    }

    fputs( "Headder:\n", file );
    uint8_t* it = arena;
    for ( uint32_t* i = (uint32_t*) it; i != OFFSET_BY( arena, ARENA_HEADER_SIZE );
          i++, it += 4 ) {
        fprintf( file, "%08llx|\t%08x (%u)\n", GET_OFFSET( i, arena ), *i, *i );
    }

    fputs( "\n Accessor:\n", file );
    for ( uint32_t* i = (uint32_t*) it; i != ArenaGetDataSegment( arena );
          i++, it += 4 ) {
        fprintf( file, "%08llx|\t%08x (%u)\n", GET_OFFSET( i, arena ), *i, *i );
    }

    fputs( "\n Data:\n", file );
    for ( uint8_t* i = it;
          i != OFFSET_BY( ArenaGetDataSegment( arena ), *ArenaGetDataCapacity( arena ) );
          i++ ) {
        fprintf( file, "%08llx|\t%02x (%u | %c)\n", GET_OFFSET( i, arena ), *i, *i, *i );
    }

    fclose( file );
    * /
}
#endif
