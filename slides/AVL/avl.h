#ifndef AVL_H
#define AVL_H

#include <stdbool.h>

typedef struct avl avl_t;
typedef void (*f_destructor_t)(void *data);
typedef int (*f_comparator_t)(const void *a, const void *b, void *context);

avl_t *create_avl(f_comparator_t compare, f_destructor_t destructor);

bool avl_insert(avl_t *avl, void *data);
bool avl_remove(avl_t *avl, void *data);
void *avl_search(avl_t *avl, void *data);

void destroy_val(avl_t **avl);

#endif
