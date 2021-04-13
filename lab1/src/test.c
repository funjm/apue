#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _data {
	int i;
} custom_data;

custom_data *new_data(int i)
{
	custom_data *d = (custom_data *)malloc(sizeof(custom_data));
	d->i = i;
	return d;
}

int main()
{
	linked_list list = list_create();
	printf("push 0..5 to linked_list\n");
	for (int it = 0; it != 5; ++it) {
		custom_data *data = new_data(it);
		list_push(&list, data);
	}

	printf("insert 100 at position 3\n");
	custom_data *data_to_insert = new_data(100);
	if (list_insert(&list, 3, data_to_insert) != 0) {
		printf("error on inserting\n");
		return 1;
	}

	printf("delete item at position 1\n");
	if (list_delete(&list, 1) != 0) {
		printf("error on deleting\n");
		return 1;
	}

	printf("iterate over list\n");
	linked_list_iterator it;
	list_iter(list, &it);
	custom_data *data = NULL;
	while ((data = (custom_data *)next(&it)) != NULL)
		printf("next value = %d\n", data->i);

	printf("drop list\n");
	if (list_drop(&list) != 0) {
		printf("error on dropping\n");
		return 1;
	}

	return 0;
}
