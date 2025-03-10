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

#ifdef __DEBUG
#include <stdio.h>
void dumpArena( void* arena, const char* name );
#endif

inline static uint32_t* ArenaGetAccessCapacity( void* arena );
inline static uint32_t* ArenaGetDataUsed( void* arena );
inline static uint32_t* ArenaGetDataCapacity( void* arena );

inline static uint32_t* ArenaGetAccessorsSegment( void* arena );
inline static void* ArenaGetDataSegment( void* arena );

inline static uint32_t* ArenaGetAccessorFromBlock( void* arena,
                                                   const arenaAccessor_t* block );

inline static uint32_t* ArenaGetBlockSize( void* arena, void* accessor );
inline static uint32_t* ArenaGetBlockSizeFromBlock( void* arena,
                                                    arenaAccessor_t* block );

inline static void* ArenaGetBlockDataFromSize( uint32_t* size );

inline static void Swap( void* a, void* b );
static void MoveMemBlock( void* a, void* aEnd );

static void* ArenaPushAccessor( void** arena );
static void ArenaPushData( void** arena );

static uint32_t* ArenaFindAvailableAccessor( void* arena );
static void ArenaUpdateAccessors( void* arena, uint32_t* accessor,
                                  uint32_t* size );

static void* CreateArena( void );
static void DestroyArena( void** arena );

static void* gArenaInstance = NULL;

/*
=================
CreateArena

Creates a Arena instance.
=================
*/
void* CreateArena( void ) {
    void* arena = malloc( ARENA_INITIAL_SIZE );
    memset( arena, 0x00, ARENA_INITIAL_SIZE );

    if ( !arena ) {
        return NULL;
    }

    *ArenaGetDataUsed( arena )       = 0;
    *ArenaGetDataCapacity( arena )   = ARENA_DATA_INITIAL_SIZE;
    *ArenaGetAccessCapacity( arena ) = ARENA_ACCESSORS_INITIAL_SIZE;
    *ArenaGetArg( arena )            = 0U;

    memset( ArenaGetAccessorsSegment( arena ), ARENA_AVAILABLE_ACCESSOR_VALUE,
            *ArenaGetAccessCapacity( arena ) );

    return arena;
}

/*
=================
DestroyArenaHelper

A helper function to use DestroyArena() on atexit();
=================
*/
static void DestroyArenaHelper( void ) {
    DestroyArena( &gArenaInstance );
}

/*
=================
GetArenaSingleton

Returns a shared instance of a Arena,
in the first call of this function, it creates its instance.

Can return NULL only on first call,
if failed to create instance OR failed to register DestroyArena() on atexit()
=================
*/
void** GetArenaSingleton( void ) {
    if ( !gArenaInstance ) {
        gArenaInstance = CreateArena();
        if ( !gArenaInstance ) {
            return NULL;
        }
        if ( atexit( DestroyArenaHelper ) ) {
            return NULL;
        }
    }

    return &gArenaInstance;
}

/*
=================
ArenaGetArg

Returns the address for the uint32_t that is used on the following function as a argument:
- ArenaAlloc/ARENA_ALLOC: Amount of bytes to allocate;
- ArenaRealloc/ARENA_REALLOC: New size in Bytes for the BLOCK;
=================
*/
uint32_t* ArenaGetArg( void* arena ) {
    return (uint32_t*) arena;
}

/*
=================
ArenaGetData

Returns the actually address of the BLOCK on memory,
for convenance can use the macros ARENA_GET(DATATYPE, BLOCK) to return a DATATYPE* of the BLOCK,
or ARENA_GETI(BLOCK) where it returns the a address of the BLOCK as the variable DATATYPE, i.e.
* OBS: ARENA_GETI can only be used on variables, use ARENA_GET otherwise.
`
char* buffer = ARENA_ALLOC(1024);

void* usingArenaGetData = ArenaGetData(*GetArenaSingleton(), buffer);
char* usingARENA_GET = ARENA_GET(char, buffer);
char* usingARENA_GETI = ARENA_GETI(buffer);
`
=================
*/
void* ArenaGetData( void* arena, const arenaAccessor_t* block ) {
    return OFFSET_BY( ArenaGetDataSegment( arena ),
                      *ArenaGetAccessorFromBlock( arena, block ) + sizeof( uint32_t ) );
}

/*
=================
ArenaAlloc

Allocates *(ArenaGetArg()) Bytes and returns the BLOCK's accessor.
can return ARENA_INVALID_ACCESSOR if a reallocation fails
=================
*/
arenaAccessor_t* ArenaAlloc( void** arena ) {
    ArenaPushData( arena );
    return ArenaPushAccessor( arena );
}

/*
=================
ArenaFree

Frees a BLOCK, this doesn't call FREE() just marks the BLOCK's memory as available.
If BLOCK is the last (higher address value) BLOCK just soft delete (cheap).
Else the proceeding BLOCK are shifted to the left and erase the data (costly).
=================
*/
void ArenaFree( void* arena, arenaAccessor_t* block ) {
    uint32_t* accessor = ArenaGetAccessorFromBlock( arena, block );
    uint32_t* size     = ArenaGetBlockSize( arena, accessor );
    void* data         = ArenaGetData( arena, block );

    // store for later, because reorganizing the memory
    // changes the address of size.
    uint32_t* deletedSize = ArenaGetArg( arena );
    *deletedSize          = *size;

    // If isn't the in the end of the dataBuffer, need to
    // reorganize
    if ( *ArenaGetDataUsed( arena ) - *accessor > *size + sizeof( uint32_t ) ) {
        memmove( size, OFFSET_BY( data, *size ),
                 *ArenaGetDataUsed( arena ) - *accessor - *size );

        ArenaUpdateAccessors( arena, accessor, size );
    }

    *accessor = ARENA_AVAILABLE_ACCESSOR_VALUE;
    *ArenaGetDataUsed( arena ) -= sizeof( uint32_t ) + *deletedSize;
}

/*
=================
ArenaRealloc

Reallocs a BLOCK to be *(ArenaGetArg()) size.

IF BLOCK is the last (rightmost) BLOCK is just increased (cheap),
ELSE BLOCK is moved to be the last (rightmost) and then is increase (expensive),
in both cases can cause a Arena resizing, if there isn't enough available memory.
=================
*/
arenaAccessor_t* ArenaRealloc( void** arena, arenaAccessor_t* block ) {
    uint32_t* accessor = ArenaGetAccessorFromBlock( *arena, block );
    uint32_t* size     = ArenaGetBlockSize( *arena, accessor );
    void* data         = ArenaGetBlockDataFromSize( size );
    uint32_t* newSize  = ArenaGetArg( *arena );

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

        ArenaUpdateAccessors( newBlock, accessor, size );
        *ArenaGetAccessorFromBlock( newBlock, block ) =
            GET_OFFSET( dest, ArenaGetDataSegment( newBlock ) );

        free( *arena );
        *arena = newBlock;
    } else {
        // isn't last block
        if ( *ArenaGetDataUsed( *arena ) - *accessor > *size + sizeof( uint32_t ) ) {
            uint8_t* i = OFFSET_BY( data, *size );
            for ( ; i !=
                    OFFSET_BY( ArenaGetDataSegment( *arena ), *ArenaGetDataUsed( *arena ) );
                  i++ ) {
                uint32_t* toSwap = OFFSET_BY( i, -( *size + sizeof( uint32_t ) ) );
                MoveMemBlock( toSwap, i );
                size = OFFSET_BY( size, 1 );
            }

            ArenaUpdateAccessors( *arena, accessor, size );
            *accessor = GET_OFFSET( size, ArenaGetDataSegment( *arena ) );
        }

        *ArenaGetDataUsed( *arena ) += *newSize - *size;
        *size = *newSize;
    }

    return block;
}

/*
=================
DestroyArena

Free a Arena instance
=================
*/
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

/*
=================
ArenaFindAvailableAccessor

returns the address of a available accessor,
returns NULL if none are available.
=================
*/
static uint32_t* ArenaFindAvailableAccessor( void* arena ) {
    for ( uint32_t* i = ArenaGetAccessorsSegment( arena );
          i != (uint32_t*) ArenaGetDataSegment( arena ); i++ ) {

        if ( *i == ARENA_AVAILABLE_ACCESSOR_VALUE ) {
            return i;
        }
    }

    return NULL;
}

/*
=================
ArenaPushAccessor

Finds a accessor to host the Data offset and return its address,
IF there isn't one available, will resize Arena to create more accessor.
=================
*/
static void* ArenaPushAccessor( void** arena ) {
    uint32_t* offsetToData = ArenaGetArg( *arena );
    if ( *offsetToData == ARENA_INVALID_DATA_OFFSET ) {
        return ARENA_INVALID_ACCESSOR;
    }

    uint32_t* availableAccessor = ArenaFindAvailableAccessor( *arena );
    if ( availableAccessor ) {
        *availableAccessor = *offsetToData;
        return (arenaAccessor_t*) GET_OFFSET( availableAccessor, *arena );
    }

    // Couldn't find a Accessor Available, allocate more accessor
    void* newBlock = malloc(
        ARENA_GET_OVERALL_CAPACITY( *arena ) +
        RESIZING_FORMULA( 0, sizeof( uint32_t ), ARENA_ACCESSORS_GROWTH_AMOUNT ) );

    if ( !newBlock ) {
        return ARENA_INVALID_ACCESSOR;
    }

    memset( newBlock, 0x00, ARENA_GET_OVERALL_CAPACITY( *arena ) + RESIZING_FORMULA( 0, sizeof( uint32_t ), ARENA_ACCESSORS_GROWTH_AMOUNT ) );

    memcpy( newBlock, *arena, ARENA_HEADER_SIZE + *ArenaGetAccessCapacity( *arena ) );
    *ArenaGetAccessCapacity( newBlock ) += ARENA_ACCESSORS_GROWTH_AMOUNT;

    memset( OFFSET_BY( ArenaGetAccessorsSegment( newBlock ),
                       *ArenaGetAccessCapacity( *arena ) ),
            ARENA_AVAILABLE_ACCESSOR_VALUE,
            *ArenaGetAccessCapacity( newBlock ) - *ArenaGetAccessCapacity( *arena ) );

    memcpy( ArenaGetDataSegment( newBlock ), ArenaGetDataSegment( *arena ),
            *ArenaGetDataUsed( *arena ) );

    uint32_t* nextAvailable = OFFSET_BY( ArenaGetAccessorsSegment( newBlock ),
                                         *ArenaGetAccessCapacity( *arena ) );
    *nextAvailable          = *offsetToData;

    free( *arena );
    *arena = newBlock;

    return (arenaAccessor_t*) GET_OFFSET( nextAvailable, *arena );
}

static void ArenaPushData( void** arena ) {
    uint32_t* amountBytesToAllocate = ArenaGetArg( *arena );
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
            return;
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

        free( *arena );
        *arena                = newBlock;
        amountBytesToAllocate = ArenaGetArg( *arena );
    }

    uint32_t* data =
        OFFSET_BY( ArenaGetDataSegment( *arena ), *ArenaGetDataUsed( *arena ) );

    *data = *amountBytesToAllocate;
    *ArenaGetDataUsed( *arena ) += *amountBytesToAllocate + sizeof( uint32_t );

    *ArenaGetArg( *arena ) = GET_OFFSET( data, ArenaGetDataSegment( *arena ) );
}

/*
=================
ArenaUpdateAccessors

Update all accessors to account for the shifting of memory in functions like:
- ArenaFree/ARENA_FREE;
- ArenaRealloc/ARENA_REALLOC;
=================
*/
static void ArenaUpdateAccessors( void* arena, uint32_t* accessor,
                                  uint32_t* size ) {
    for ( uint32_t* j = ArenaGetAccessorsSegment( arena );
          j != ArenaGetDataSegment( arena ); j++ ) {
        if ( *j != ARENA_AVAILABLE_ACCESSOR_VALUE && *j > *accessor ) {
            *j -= *size + sizeof( uint32_t );
        }
    }
}

/*
=================
ArenaGetAccessCapacity

returns the address of the uint32_t that contains the amount of Bytes allocated for accessors.
=================
*/
inline static uint32_t* ArenaGetAccessCapacity( void* arena ) {
    return OFFSET_BY( arena, 4U );
}
/*
=================
ArenaGetDataUsed

returns the address of the uint32_t that contains the amount of Bytes used on Data.
=================
*/
inline static uint32_t* ArenaGetDataUsed( void* arena ) {
    return OFFSET_BY( arena, 8U );
}

/*
=================
ArenaGetDataCapacity

returns the address of the uint32_t that contains the amount of Bytes allocated for Data.
=================
*/
inline static uint32_t* ArenaGetDataCapacity( void* arena ) {
    return OFFSET_BY( arena, 12U );
}

/*
=================
ArenaGetAccessorsSegment

returns the address of the start of the accessors segment.
=================
*/
inline static uint32_t* ArenaGetAccessorsSegment( void* arena ) {
    return OFFSET_BY( arena, ARENA_HEADER_SIZE );
}

/*
=================
ArenaGetDataSegment

returns the address of the start of the data segment.
=================
*/
inline static void* ArenaGetDataSegment( void* arena ) {
    return OFFSET_BY( arena, *ArenaGetAccessCapacity( arena ) + ARENA_HEADER_SIZE );
}

/*
=================
Swap

Inplace XOR swap algorithm.
=================
*/
inline static void Swap( void* a, void* b ) {
    *(uint8_t*) a ^= *(uint8_t*) b;
    *(uint8_t*) b ^= *(uint8_t*) a;
    *(uint8_t*) a ^= *(uint8_t*) b;
}

/*
=================
MoveMemBlock

Moves the memory in a to AEnd - 1 to the right, i.e.

in:
a               aEnd
v                 v
1, 2, 3, 4, 5, 6, 7, 8

out:
7, 1, 2, 3, 4, 5, 6, 8
=================
*/
static void MoveMemBlock( void* a, void* aEnd ) {
    for ( uint8_t* it = aEnd; a != it; it-- ) {
        Swap( it, it - 1 );
    }
}

/*
=================
ArenaGetAccessorFromBlock

helper function to get Accessor from arenaAccessor_t* BLOCK;
=================
*/
inline static uint32_t*
ArenaGetAccessorFromBlock( void* arena, const arenaAccessor_t* block ) {
    return OFFSET_BY_TYPED( uint32_t*, arena, (uint64_t) block );
}

/*
=================
ArenaGetBlockSize

Helper function that returns the address of the size indicator of the BLOCK
pointed by the accessor.
=================
*/
inline static uint32_t* ArenaGetBlockSize( void* arena, void* accessor ) {
    return OFFSET_BY_TYPED( uint32_t*, ArenaGetDataSegment( arena ),
                            *(uint32_t*) accessor );
}

/*
=================
ArenaGetBlockSizeFromBlock

Helper function that returns the address of the size indicator of the BLOCK
=================
*/
inline static uint32_t* ArenaGetBlockSizeFromBlock( void* arena,
                                                    arenaAccessor_t* block ) {
    return ArenaGetBlockSize( arena, ArenaGetAccessorFromBlock( arena, block ) );
}

/*
=================
ArenaGetBlockDataFromSize

returns the address of the data using the address of the size indicator.
=================
*/
inline static void* ArenaGetBlockDataFromSize( uint32_t* size ) {
    return OFFSET_BY( size, sizeof( uint32_t ) );
}

#ifdef __DEBUG
/*
=================
dumpArena

Dump the memory contents on the Arena, is used for DEBUG purposes,
called from GDB directly, it has a weird issue with fprintf() formats on
windows and linux
=================
*/
/*
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
        fprintf( file, "%08lx|\t%08x (%u)\n", GET_OFFSET( i, arena ), *i, *i );
    }

    fputs( "\n Accessor:\n", file );
    for ( uint32_t* i = (uint32_t*) it; i != ArenaGetDataSegment( arena );
          i++, it += 4 ) {
        fprintf( file, "%08lx|\t%08x (%u)\n", GET_OFFSET( i, arena ), *i, *i );
    }

    fputs( "\n Data:\n", file );
    for ( uint8_t* i = it;
          i != OFFSET_BY( ArenaGetDataSegment( arena ), *ArenaGetDataCapacity( arena ) );
          i++ ) {
        fprintf( file, "%08lx|\t%02x (%u | %c)\n", GET_OFFSET( i, arena ), *i, *i, *i );
    }

    fclose( file );
}
*/
#endif
