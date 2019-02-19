#ifndef PASSENGER_LIST_H
#define PASSENGER_LIST_H


/*
 * Suggest sort orders, which are explained in more detail in the instructions file. 
 */
#define SORT_TIME 0
#define SORT_DESTINATION_ASCENDING 1
#define SORT_DESTINATION_DESCENDING 2



struct passenger {
  unsigned int time;
  int sourceLevel;
  int destinationLevel;
};


struct passengerListElement{
  struct passenger *data; 
  struct passengerListElement *nextElement;
};

struct passengerList{
  struct passengerListElement *firstElement;
};


/* 
 * This function should add passenger to the passenger linked list.
 * The list should be sorted based on the value of the sortOrder argument. 
 *
 * At least the following sort orderst needs to be supported:
 *  Sort on time (closest in time first): sortOrder == SORT_TIME
 *  Sort on destination, ascending: sortOrder == SORT_DESTINATION_ASCENDING
 *  Sort on destination, Descending: sortOrder == SORT_DESTINATION_DESCENDING
 */
unsigned addPassenger(struct passengerList* list, unsigned int time, int sourceLevel, int destinationLevel, int sortOrder);


/* 
 * Check if argument passenger list is empty.
 *
 * Return values:
 *  1 if list is empty
 *  0 if list is not empty
 *
 */
int passengerListIsEmpty(struct passengerList* list);


/*
 * Initialize empty passenger list, i.e., setting list->firstElement = NULL
 */
void initPassengerList(struct passengerList* list);






/* This function should get and remove next passenger from the passenger queue
 * (implemented as a linked list). 
 * 
 * It takes the following arguments as input:
 *   list - A pointer to the passenger list
 *   nextPassenger - A pointer to a passenger instance where the data of the next passenger  
 *               should be stored.  
 *
 * Return values: 
 *   NULL - If the passenger list was empty at the time of calling the function. 
 *   nextPassenger - The next passenger pointer otherwise. 
 *  
 * Don't forget to free memory of the element that was removed from the linked list.
 *
 * Otherwise you have created a memory leak. 
 */
struct passenger *getNextPassenger(struct passengerList* list, struct passenger *nextPassager);



/* This function should get, without removing, next passenger from the passenger queue 
 * (implemented as a linked list). 
 *
 *
 * It takes the following arguments as input:
 *   list - A pointer to the passenger list
 *   nextPassenger - A pointer to a passenger instance where the data of the 
 * next passenger should be stored.  
 *
 * Return values: 
 *   NULL - If the passenger list was empty at the time of calling the function. 
 *   nextPassenger - The next passenger pointer otherwise. 
 *   
 */
struct passenger *peekNextPassenger(struct passengerList* list, struct passenger *nextPassager);



#endif
