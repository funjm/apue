#ifndef LINKED_LIST
#define LINKED_LIST

#include <stddef.h>
#include <stdlib.h>

typedef struct node {
	void *data;
	struct node *next;
} linked_list_node, *linked_list;

typedef struct iterator {
	linked_list_node *list;
} linked_list_iterator;

linked_list list_create();
int list_insert(linked_list *listp, size_t position, void *data);
int list_push(linked_list *listp, void *data);
int list_delete(linked_list *listp, size_t position);
int list_drop(linked_list *listp);
void list_iter(linked_list list, linked_list_iterator *iter);
void *next(linked_list_iterator *iter);

#endif
