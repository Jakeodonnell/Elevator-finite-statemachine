#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "passenger_list.h"

/*
 * It is suggested to use a compare function to simplify the implementation of your
 * addPassenger function. 
 */
int comparePassengers(struct passenger *passenger1, struct passenger *passenger2, int sortOrder);

int comparePassengers(struct passenger *passenger1, struct passenger *passenger2, int sortOrder)
{
  if (sortOrder == SORT_TIME)
  {
    if (passenger1->time == passenger2->time)
    {
      return 0;
    }
    else if (passenger1->time < passenger2->time)
    {
      return -1;
    }
    else
    {
      return 1;
    }
  }

  else if (sortOrder == SORT_DESTINATION_ASCENDING)
  {
    if (passenger1->destinationLevel == passenger2->destinationLevel)
    {
      return 0;
    }
    else if (passenger1->destinationLevel < passenger2->destinationLevel)
    {
      return -1;
    }
    else
    {
      return 1;
    }
  }

  else if (sortOrder == SORT_DESTINATION_DESCENDING)
  {
    if (passenger1->destinationLevel == passenger2->destinationLevel)
    {
      return 0;
    }
    else if (passenger1->destinationLevel < passenger2->destinationLevel)
    {
      return 1;
    }
    else
    {
      return -1;
    }
  }

  return 0;
} /* End comparePassengers (...) */

void initPassengerList(struct passengerList *list)
{
  list->firstElement = NULL;
}

unsigned int addPassenger(struct passengerList* list, unsigned int time, int sourceLevel, int destinationLevel, int sortOrder){
	//Creating passenger1, this is the passenger to be compared with.
  struct passengerListElement* passElement = malloc(sizeof(passElement));
  struct passenger* passenger1 = malloc(sizeof(passenger1));

  //Passing inparrameters to the element
	passenger1->destinationLevel = destinationLevel;
  passenger1->sourceLevel = sourceLevel;
  passenger1->time = time;

  //Setting passanger to the correspoding element data
  passElement->data = passenger1;

  //If list is empty or the first element is sorted to be larger.
	if(list->firstElement == NULL || (comparePassengers(passenger1, list->firstElement->data, sortOrder) < 0)){
    //moves the first element to be the next of passanger 1
    passElement->nextElement = NULL;
		passElement->nextElement = list->firstElement;
    //first element is now the passanger1
		list->firstElement = passElement;
		} else {
		struct passengerListElement* passElement2 = list->firstElement;
    passElement2->data = list->firstElement->data;
    //while passelement2 
		while(passElement2->nextElement!=NULL && (comparePassengers(passenger1, passElement2->data, sortOrder) > 0)){
      //checks the next element in the list.
        passElement2 = passElement2->nextElement;
        passElement2->data = passElement2->nextElement->data;
		}
    //setting everything in order
		passElement->nextElement = passElement2->nextElement;
		passElement2 = passElement->nextElement;
	}
}

struct passenger *getNextPassenger(struct passengerList *list, struct passenger *nextPassager)
{
  if (list->firstElement == NULL)
  {
    return INT_MIN;
  }
  else
  {
    nextPassager = list->firstElement;
    list->firstElement = list->firstElement->nextElement;
    return nextPassager;
    free(nextPassager);
    return 1;
  }
  return NULL;
}

struct passenger *peekNextPassenger(struct passengerList *list, struct passenger *nextPassager)
{
  if (list->firstElement == NULL)
  {
    return NULL;
  }
  else
  {
    nextPassager = list->firstElement;
    list->firstElement = list->firstElement->nextElement;
    return nextPassager;
  }
}


int passengerListIsEmpty(struct passengerList* list) {
  if (list->firstElement == NULL)
  {
    return 1;
  }
  return 0;
}
