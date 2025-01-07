#ifndef AGENDA_H
#define AGENDA_H

#include <stdint.h>

#define ELEMENT_START_FLAG 0x02
#define ELEMENT_END_FLAG 0x04

#define AGENDA_HEADER_SIZE 20U

#ifndef AGENDA_INITIAL_SIZE
#define AGENDA_INITIAL_SIZE 1024U
#endif

#ifndef AGENDA_GROWTH_AMOUNT
#define AGENDA_GROWTH_AMOUNT 512U
#endif

/*
 * Agenda Layout:
 * Offset (B) |     Type    | Content             | Reason            |
 *      0     | uint32_t    | Buffer Capacity (B) |  Dynamic Resizing |
 *      4     | uint32_t    | Buffer Used (B)     |  Dynamic Resizing |
 *      8     | uint32_t    | GetEntry index      |  FunctionArena Args    |
 *     12     | const char* | Name to search      |  Function Args    |
 *     20     | void*       | Local Vars Stack    |  Locals Vars      |
 *     28     | void*       | Data Array          |  Data storage     |
 *     ...
 */

void *CreateAgenda();

uint32_t *AgendaGetUsed(void *agenda);
uint32_t *AgendaGetCapacity(void *agenda);

uint32_t *AgendaGetIndexArg(void *agenda);
void **AgendaGetSearchArg(void *agenda);
void **AgendaGetLocalsStack(void *agenda);

// Need to use "AgendaSetGetArg" to specify index,
// returns NULL on invalid index.
void *AgendaGetEntry(void *agenda);

// Need to use "AgendaGetSearchArg" to specify the name to search,
// returns NULL on not found.
void *AgendaSearch(void *agenda);

void AgendaRemove(void *agenda);

void DestroyAgenda(void **agenda);

#endif
