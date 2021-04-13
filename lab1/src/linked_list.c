#include "linked_list.h"

linked_list list_create()
{
	return NULL;
}

void list_iter(linked_list list, linked_list_iterator *iter)
{
	iter->list = list;
	return;
}

void *next(linked_list_iterator *iter)
{
	if (iter->list == NULL)
		return NULL;
	void *data = iter->list->data;
	iter->list = iter->list->next;
	return data;
}

int list_insert(linked_list *listp, size_t position, void *data)
{
	size_t current = 0;
	for (current = 0; *listp && current != position; ++current)
		listp = &(*listp)->next;

	if (current != position)
		return 1;

	linked_list single_list = (linked_list)malloc(sizeof(linked_list_node));

	if (single_list == NULL)
		return 2;

	single_list->data = data;
	single_list->next = *listp;
	*listp = single_list;
	return 0;
}

int list_push(linked_list *listp, void *data)
{
	while (*listp)
		listp = &(*listp)->next;

	linked_list single_list = (linked_list)malloc(sizeof(linked_list_node));

	if (single_list == NULL)
		return 2;

	single_list->data = data;
	single_list->next = *listp;
	*listp = single_list;
	return 0;
}

int list_delete(linked_list *listp, size_t position)
{
	size_t current = 0;
	for (current = 0; *listp && current != position; ++current)
		listp = &(*listp)->next;

	if (current != position)
		return 1;

	linked_list to_be_deleted = *listp;
	*listp = to_be_deleted->next;
	free(to_be_deleted->data);
	free(to_be_deleted);

	return 0;
}

int list_drop(linked_list *listp)
{
	while (*listp) {
		linked_list to_be_deleted = *listp;
		*listp = to_be_deleted->next;
		free(to_be_deleted->data);
		free(to_be_deleted);
	}
	return 0;
}
