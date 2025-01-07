#ifndef LCCALS_STACK_H
#define LCCALS_STACK_H

#include <stdint.h>

#ifndef STACK_SIZE
#define STACK_SIZE 4096U
#endif

#define STACK_HEADER_SIZE 17U

/*
 * Stack Layout:
 * Offset (B) |     Type    | Content             | Reason            |
 *      0     | uint32_t    | Push Bytes Argument |  Function Args    |
 *      4     | uint32_t    | Pop Bytes Argument  |  Function Args    |
 *      8     | uint32_t    | Peek Bytes Argument |  Function Args    |
 *     12     | uint32_t    | Used Bytes Amount   |  Function Args    |
 *     16     | uint8_t     | Function errnum     |  Function Error   |
 *     17     | void*       | Data Array          |  Data storage     |
 *     ...
 */
void *CreateStack(void);

uint32_t *StackGetPushArg(void *stack);
uint32_t *StackGetPopArg(void *stack);
uint32_t *StackGetPeekArg(void *stack);
uint8_t *StackGetErrnum(void *stack);

void StackPush(void *stack);
void StackPop(void *stack);
void *StackPeek(void *stack);

void DestroyStack(void **stack);

#endif
