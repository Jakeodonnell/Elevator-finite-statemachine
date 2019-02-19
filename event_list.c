#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
#include"event_list.h"

void initEventList(struct eventList* list) {
  list->firstElement = NULL;
}

unsigned int addEvent(struct eventList* list, int type, unsigned int time, int source, int destination) {
	struct event * new_event;
	new_event = (struct event *)malloc(sizeof(struct event));
	new_event->time = time;
	new_event->type = type;
	new_event->source = source;
	new_event->destination = destination;

	struct eventListElement * elem;
	elem = (struct eventListElement *)malloc(sizeof(struct eventListElement));
	elem->data = new_event;

	if (list->firstElement == NULL) //list is empty
	{
		elem->nextElement = NULL;
		list->firstElement = elem;
		return time;
	}
	struct eventListElement * ptr_prev;
	struct eventListElement * ptr;
	ptr_prev = list->firstElement;
	ptr = list->firstElement;
	while(ptr != NULL) 
	{
		if (ptr->data->time <= time)
		{
			ptr_prev = ptr;
			ptr = ptr->nextElement;
		} else {
			if (ptr_prev == ptr)
			{
				elem->nextElement = list->firstElement;
				list->firstElement = elem;
			} else {
				ptr_prev->nextElement = elem;
				elem->nextElement = ptr;
			}
			return time;
		}
	}
	if (ptr == NULL)
	{
		ptr_prev->nextElement = elem;
		elem->nextElement = NULL;
		return time;
	}
 	return INT_MIN;
}



struct event *getNextEvent(struct eventList* list, struct event *nextElementEvent){
	if (list->firstElement == NULL)
	{
		return NULL;
	} else {
		struct eventListElement * ptr;
		ptr = list->firstElement; //points to first in list
		struct event *retval;
		retval = ptr->data;
		*nextElementEvent = *retval;
		list->firstElement = ptr->nextElement; //set first position in list to the second element
		free(ptr);
		return retval;
	}
}