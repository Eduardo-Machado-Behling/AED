#include "avl.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct node_t {
  void *data;
  struct node_t *left, *right;
  int height;
} node_t;

typedef struct avl {
  f_comparator_t compare;
  f_destructor_t destructor;
  node_t *node;
} avl_t;

enum accessCommand_t { NONE = 0, RIGHT = 0b01, LEFT = 0b10, RIGHTLEFT = 0b11 };

typedef enum accessCommand_t (*f_action_t)(node_t *node, avl_t *compare,
                                           void *context);

typedef struct {
  f_action_t onAccess;
  f_action_t onAfter;
} action_t;

static node_t *create_node(void *data, node_t *left, node_t *right);
static void balance(node_t *node);
static void left_rotation(node_t **node);
static void right_rotation(node_t **node);
static node_t *traverse(node_t *node, action_t action, avl_t *avl,
                        void *context);

avl_t *create_avl(f_comparator_t compare, f_destructor_t destructor) {
  avl_t *avl = malloc(sizeof(avl_t));
  if (!avl) {
    return NULL;
  }

  avl->compare = compare;
  avl->destructor = destructor ? destructor : free;

  return avl;
}

static enum accessCommand_t avlInsertOnAccessAction(node_t *node, avl_t *avl,
                                                    void *context) {
  int comp = avl->compare(node->data, context, context);

  if (!node->left && comp < 0) {
    node->left = create_node(context, NULL, NULL);
  } else if (!node->right && comp > 0) {
    node->right = create_node(context, NULL, NULL);
  } else {
    return comp < 0 ? LEFT : RIGHT;
  }

  return 0;
}

static void avlInsertOnAfterAction(node_t *node, avl_t *avl, void *context) {
  node->height++;
}

bool avl_insert(avl_t *avl, void *data) {
  return traverse(avl->node,
                  (action_t){.onAccess = avlInsertOnAccessAction, .onAfter = },
                  avl, data);
}

bool avl_remove(avl_t *avl, void *data) {}

static enum accessCommand_t avl_search_traverse(node_t *node, avl_t *avl,
                                                void *context) {
  int comp = avl->compare(node->data, context, context);

  if (comp == 0) {
    return NONE;
  }

  return comp < 0 ? LEFT : RIGHT;
}

void *avl_search(avl_t *avl, void *data) {
  return traverse(avl->node, avl_search_traverse, avl, data);
}

static enum accessCommand_t avl_free_action(node_t *node, avl_t *avl,
                                            void *context) {
  avl->destructor(node);
  return RIGHTLEFT;
}

void destroy_val(avl_t **avl) {
  if (!avl) {
    return;
  }

  avl_t *a = *avl;
  *avl = NULL;

  if (a) {
    if (a->node) {
      traverse(a->node, avl_free_action, a, NULL);
    }
    free(a);
  }
}

static node_t *traverse(node_t *node, f_on_access action, avl_t *avl,
                        void *context) {
  if (!node) {
    return NULL;
  }

  node_t *nodeTemp[2] = {node->left, node->left};
  uint8_t res = action(node, avl, context);
  if (res == NONE) {
    return node;
  }

  node_t *resAction = NULL;
  for (uint8_t i = 0; i < 1; i++) {
    uint8_t mask = 0x1U << i;
    if (res & mask) {
      resAction = traverse(nodeTemp[i], action, avl, context);
      if (!resAction) {
        return resAction;
      }
    }
  }

  return NULL;
}

static node_t *create_node(void *data, node_t *left, node_t *right) {
  node_t *node = malloc(sizeof(node_t));
  if (!node) {
    return NULL;
  }

  node->left = left;
  node->right = right;
  node->data = data;
  node->height = 1;

  return node;
}

static void balance(node_t *node) {}

static void left_rotation(node_t **node) {
  node_t *temp = node[0]->right;
  node[0]->right = temp->left;
  temp->left = node[0];
  *node = temp;
}

static void right_rotation(node_t **node) {
  node_t *temp = node[0]->left;

  node[0]->left = temp->right;
  temp->right = node[0];
  *node = temp;
}
