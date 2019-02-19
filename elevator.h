#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "passenger_list.h"
#include "event_list.h"


struct elevator {
  int currentPosition;
  int destination;
  int numLevels;
  int numPassengers;
  int capacity;
  struct passengerList passengers;

  /* Function ptr*/
  void (*stateFuncPtr)(struct elevator*, struct event*);
};


/* 
 * We need some state function prototypes, for exemple, the following.
 */
void printEvent( struct event* curEvent);
void elevatorIdle(struct elevator *_elevator, struct event* curEvent);
void elevatorMoving(struct elevator *_elevator, struct event* curEvent);
void elevatorDoorsOpen(struct elevator *_elevator, struct event* curEvent);

/*
 * Initialize the elevator, for example, setting the initial position and
 * inital state. 
 * This function from the main function in elevator_simulator.c
 */ 
void initElevator(struct elevator *elevator, int numLevels);






#endif
