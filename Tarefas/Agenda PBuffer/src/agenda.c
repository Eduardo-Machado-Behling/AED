#include "agenda.h"
#include "common.h"
#include "locals_stack.h"

#include <stdlib.h>
#include <string.h>

static void *AgendaGetData(void *agenda) {
  return (void *)OFFSET_BY(agenda, 28U);
}

void *CreateAgenda() {
  void *agenda = malloc(AGENDA_INITIAL_SIZE);
  if (!agenda) {
    return NULL;
  }

  *AgendaGetUsed(agenda) = AGENDA_HEADER_SIZE;
  *AgendaGetCapacity(agenda) = AGENDA_INITIAL_SIZE;
  *AgendaGetIndexArg(agenda) = 0U;
  *AgendaGetSearchArg(agenda) = NULL;

  void **stack = AgendaGetLocalsStack(agenda);
  *stack = CreateStack();
  if (!*stack) {
    DestroyAgenda(&agenda);
  }

  return agenda;
}

uint32_t *AgendaGetUsed(void *agenda) { return (uint32_t *)agenda; }
uint32_t *AgendaGetCapacity(void *agenda) {
  return (uint32_t *)OFFSET_BY(agenda, 4U);
}

uint32_t *AgendaGetIndexArg(void *agenda) {
  return (uint32_t *)OFFSET_BY(agenda, 8U);
}
void **AgendaGetSearchArg(void *agenda) {
  return (void **)OFFSET_BY(agenda, 12U);
}

void **AgendaGetLocalsStack(void *agenda) {
  return (void **)OFFSET_BY(agenda, 20U);
}
// Need to use "AgendaSetGetArg" to specify index,
// returns NULL on invalid index.
void *AgendaGetEntry(void *agenda) {
  void **stack = AgendaGetLocalsStack(agenda);

  *StackGetPushArg(*stack) = 4U;
  StackPush(*stack);
  *StackGetPeekArg(*stack) = 0U;
  uint32_t *i = StackPeek(*stack);

  *StackGetPushArg(*stack) = 4U;
  StackPush(*stack);
  *StackGetPeekArg(*stack) = 0U;
  uint32_t *elementCount = StackPeek(*stack);
  *elementCount = 0;

  for (*i = 0; *elementCount < *AgendaGetIndexArg(agenda) &&
               *i < (*AgendaGetUsed(agenda) - AGENDA_HEADER_SIZE);
       (*i)++) {
    if (*OFFSET_BY(AgendaGetData(agenda), *i) == ELEMENT_START_FLAG) {
      (*elementCount)++;
    }
  }

  void *res = NULL;
  if (*elementCount == *AgendaGetIndexArg(agenda)) {
    res = OFFSET_BY(AgendaGetData(agenda), *i);
  }

  *StackGetPopArg(*stack) = 8U;
  StackPop(*stack);
  return res;
}

// Need to use "AgendaGetSearchArg" to specify the name to search,
// returns NULL on not found.
void *AgendaSearch(void *agenda) {
  void **stack = AgendaGetLocalsStack(agenda);

  *StackGetPushArg(*stack) = 4U;
  StackPush(*stack);
  *StackGetPeekArg(*stack) = 0U;
  uint32_t *i = StackPeek(*stack);
  *StackGetPushArg(*stack) = 1U;
  StackPush(*stack);
  *StackGetPeekArg(*stack) = 0U;
  uint32_t *found = StackPeek(*stack);
  *found = 0;

  for (*i = 0; !*found && *i < (*AgendaGetUsed(agenda) - AGENDA_HEADER_SIZE);
       (*i)++) {
    if (*OFFSET_BY(AgendaGetData(agenda), *i) == ELEMENT_START_FLAG) {
      if (strcmp((const char *)OFFSET_BY(AgendaGetData(agenda), *i + 1),
                 *AgendaGetSearchArg(agenda)) == 0) {
        *found = 1;
      }
    }
  }

  void *res = NULL;
  if (found) {
    res = OFFSET_BY(AgendaGetData(agenda), *i);
  }

  *StackGetPopArg(*stack) = 5U;
  StackPop(*stack);
  return res;
}

// TODO(yadard): Crazy
void AgendaRemove(void *agenda) {}

void DestroyAgenda(void **agenda) {
  if (!agenda) {
    return;
  }

  void *ag = *agenda;
  *agenda = NULL;

  if (ag) {
    DestroyStack(AgendaGetLocalsStack(ag));
    free(ag);
  }
}
