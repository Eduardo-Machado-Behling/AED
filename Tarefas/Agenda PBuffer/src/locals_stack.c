#include "locals_stack.h"
#include "common.h"

#include <stdlib.h>

static uint32_t *StackGetUsed(void *stack) {
  return (uint32_t *)(OFFSET_BY(stack, 12U));
}

static void *StackGetData(void *stack) {
  return (void *)(OFFSET_BY(stack, STACK_HEADER_SIZE));
}

void *CreateStack(void) {
  void *stack = malloc(STACK_SIZE);

  if (!stack) {
    return NULL;
  }

  *StackGetUsed(stack) = 0;
  *StackGetPushArg(stack) = 0;
  *StackGetPopArg(stack) = 0;
  *StackGetPeekArg(stack) = 0;
  *StackGetErrnum(stack) = 0;

  return stack;
}

uint32_t *StackGetPushArg(void *stack) { return (uint32_t *)stack; }

uint32_t *StackGetPopArg(void *stack) {
  return (uint32_t *)(OFFSET_BY(stack, 4U));
}

uint32_t *StackGetPeekArg(void *stack) {
  return (uint32_t *)(OFFSET_BY(stack, 8U));
}

uint8_t *StackGetErrnum(void *stack) { return OFFSET_BY(stack, 16U); }

void StackPush(void *stack) {
  if (*StackGetPushArg(stack) >
      (STACK_SIZE - STACK_HEADER_SIZE - *StackGetUsed(stack))) {
    *StackGetErrnum(stack) = 1;
    return;
  }

  *StackGetUsed(stack) += *StackGetPushArg(stack);
  *StackGetErrnum(stack) = 0;
}

void StackPop(void *stack) { *StackGetUsed(stack) -= *StackGetPopArg(stack); }
void *StackPeek(void *stack) {
  return OFFSET_BY(StackGetData(stack),
                   *StackGetUsed(stack) - *StackGetPeekArg(stack));
}

void DestroyStack(void **stack) {
  if (!stack) {
    return;
  }

  void *st = *stack;
  *stack = NULL;

  if (st) {
    free(st);
  }
}
