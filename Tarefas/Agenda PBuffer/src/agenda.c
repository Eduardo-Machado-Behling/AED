#include "agenda.h"
#include "arena.h"
#include "common.h"
#include "darray.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void **AgendaGetData(void *agenda) {
  return (void **)ARENA_GET(void, agenda);
}

static void **AgendaGetPrefix(void *agenda) {
  return OFFSET_BY_TYPED(void **, ARENA_GET(void, agenda), 8U);
}

static uint32_t const *AgendaGetUsed(void *agenda) {
  return DArraySize(*AgendaGetData(agenda));
}

static void AgendaDynamicCopy(void *agenda, const void *src,
                              const uint32_t *bytesAmount);

void *CreateAgenda(void) {
  void *agenda = ARENA_ALLOC(AGENDA_SIZE);
  if (!agenda) {
    return NULL;
  }

  // Need a aux variable, because the compiler
  // wasn't working with:
  // *AgendaGetData(agenda) = CreateDArray();
  //                          _       _
  //  For some magical reason  \(-_-)/
  void *aux = CreateDArray();
  *AgendaGetData(agenda) = aux;

  aux = CreateDArray();
  *AgendaGetPrefix(agenda) = aux;

  *AgendaGetIndexArg(agenda) = 0U;
  *AgendaGetSearchArg(agenda) = NULL;

  return agenda;
}

uint32_t *AgendaGetIndexArg(void *agenda) {
  return DArrayGetArg(*AgendaGetData(agenda));
}

const char **AgendaGetSearchArg(void *agenda) {
  return OFFSET_BY_TYPED(const char **, ARENA_GET(void, agenda), 16U);
}

// Need to use "AgendaSetGetArg" to specify index,
// returns NULL on invalid index.
void *AgendaGetEntry(void *agenda) {
  uint32_t *index =
      DARRAY_AT(*AgendaGetPrefix(agenda), *AgendaGetIndexArg(agenda), uint32_t);

  if (!index) {
    return NULL;
  }

  *DArrayGetArg(*AgendaGetData(agenda)) = *index;

  return DArrayAt(*AgendaGetData(agenda));
}

// Need to use "AgendaGetSearchArg" to specify the name to search,
// returns NULL on not found.
void *AgendaSearch(void *agenda) {
  ARENA_SIH(uint32_t, i);

  void *entry = NULL;
  for (*ARENA_GETI(i) = 0; 1; *ARENA_GETI(i) += 1) {
    entry = AGENDA_GET_ENTRY(agenda, *ARENA_GETI(i));
    if (!entry) {
      break;
    }

    if (strcmp(entry, ARENA_GET(const char, *AgendaGetSearchArg(agenda))) ==
        0) {
      break;
    }
  }

  return entry;
}

void AgendaAdd(void *agenda, const char *name, uint8_t *age,
               const char *email) {
  ARENA_SIH(uint32_t, i);
  ARENA_SIH(uint32_t, used);
  ARENA_SIH(uint32_t, bytesAdded);

  void **prefix = AgendaGetPrefix(agenda);
  void **data = AgendaGetData(agenda);

  *ARENA_GETI(bytesAdded) = 0;
  *ARENA_GETI(i) = strlen(ARENA_GETI(name)) + 1;
  *ARENA_GETI(used) = *AgendaGetUsed(agenda);

  uint32_t *temp = DARRAY_PUSH(*prefix, uint32_t);
  *temp = *ARENA_GETI(used);

  AgendaDynamicCopy(agenda, name, i);
  // copy failed
  if (*ARENA_GETI(used) == *AgendaGetUsed(agenda)) {
    goto copy_failed;
  }

  *ARENA_GETI(bytesAdded) += *ARENA_GETI(i);

  *ARENA_GETI(i) = sizeof(*age);
  *ARENA_GETI(used) = *AgendaGetUsed(agenda);
  AgendaDynamicCopy(agenda, age, i);
  // copy failed
  if (*ARENA_GETI(used) == *AgendaGetUsed(agenda)) {
    goto copy_failed;
  }
  *ARENA_GETI(bytesAdded) += *ARENA_GETI(i);

  *ARENA_GETI(i) = strlen(ARENA_GETI(email)) + 1;
  *ARENA_GETI(used) = *AgendaGetUsed(agenda);
  AgendaDynamicCopy(agenda, email, i);
  // copy failed
  if (*ARENA_GETI(used) == *AgendaGetUsed(agenda)) {
    goto copy_failed;
  }

  return;

copy_failed:
  *DArrayGetArg(*data) = *ARENA_GETI(bytesAdded);
  DArrayPop(*data);
  DARRAY_POP(*prefix, uint32_t);
}

// TODO(yadard): Crazy
void AgendaRemove(void *agenda) {
  ARENA_SIH(uint32_t, i);
  ARENA_SIH(uint32_t, elemAmount);

  *ARENA_GETI(elemAmount) =
      DARRAY_ELEMENT_AMOUNT(*AgendaGetPrefix(agenda), uint32_t);
  for (*ARENA_GETI(i) = 0; *ARENA_GETI(i) < *ARENA_GETI(elemAmount);
       *ARENA_GETI(i) += 1) {
    void *entry = AGENDA_GET_ENTRY(agenda, *ARENA_GETI(i));
    if (!entry) {
      break;
    }

    if (strcmp(entry, ARENA_GET(const char, *AgendaGetSearchArg(agenda))) ==
        0) {
      if (*ARENA_GETI(i) + 1 == *ARENA_GETI(elemAmount)) {
        *DArrayGetArg(*AgendaGetData(agenda)) =
            *DARRAY_AT(*AgendaGetPrefix(agenda), *ARENA_GETI(i), uint32_t) -
            *DARRAY_AT(*AgendaGetPrefix(agenda), *ARENA_GETI(i) - 1, uint32_t);

        DArrayPop(*AgendaGetData(agenda));
        DARRAY_POP(*AgendaGetPrefix(agenda), uint32_t);
      } else {
        *DArrayGetArg(*AgendaGetData(agenda)) =
            *DARRAY_AT(*AgendaGetPrefix(agenda), *ARENA_GETI(i), uint32_t);

        *DArrayGetElemSize(*AgendaGetData(agenda)) =
            *DARRAY_AT(*AgendaGetPrefix(agenda), *ARENA_GETI(i) + 1, uint32_t) -
            *DARRAY_AT(*AgendaGetPrefix(agenda), *ARENA_GETI(i), uint32_t);

        DARRAY_REMOVE(*AgendaGetPrefix(agenda), *ARENA_GETI(i), uint32_t);
        DArrayRemove(*AgendaGetData(agenda));

        for (; 1; *ARENA_GETI(i) += 1) {
          uint32_t *index =
              DARRAY_AT(*AgendaGetPrefix(agenda), *ARENA_GETI(i), uint32_t);
          if (!index) {
            break;
          }
          *index -= *DArrayGetElemSize(*AgendaGetData(agenda));
        }
      }
      break;
    }
  }
}

void DestroyAgenda(void **agenda) {
  if (!agenda) {
    return;
  }

  void *ag = *agenda;
  *agenda = NULL;

  if (ag) {
    if (*AgendaGetData(ag)) {
      DestroyDArray(AgendaGetData(ag));
    }
    if (*AgendaGetPrefix(ag)) {
      DestroyDArray(AgendaGetPrefix(ag));
    }
    ARENA_FREE(ag);
  }
}

static void AgendaDynamicCopy(void *agenda, const void *src,
                              const uint32_t *bytesAmount) {
  void *data = *AgendaGetData(agenda);

  *DArrayGetArg(data) = *ARENA_GETI(bytesAmount);
  void *dest = DArrayPush(data);
  memmove(dest, ARENA_GETI(src), *ARENA_GETI(bytesAmount));
}
