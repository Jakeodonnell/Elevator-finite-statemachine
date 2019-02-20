#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "elevator.h"
#include "event_list.h"
#include "circular_buffer.h"

extern unsigned int time;
extern struct circularBuffer pushedButtonsQueue;
extern struct passengerList waitingUp[];
extern struct passengerList waitingDown[];
extern struct eventList _eventList;

int elevatorOn;
int waitTime;
typedef int bool;
#define true 1
#define false 0
bool down = false;
int uppdown = 0;

void initElevator(struct elevator *_elevator, int numLevels)
{
  _elevator->currentPosition = 0;
  _elevator->destination = 0;
  _elevator->stateFuncPtr = elevatorIdle;
  _elevator->numLevels = numLevels;
  _elevator->numPassengers = 0;
  _elevator->capacity = 10;
  initPassengerList(&(_elevator->passengers));
}

/* 
 * Example of state function for idle state. This shows how things work, and 
 * you are expected to modify this function.
 */
void elevatorIdle(struct elevator *_elevator, struct event *curEvent)
{
  
  if (curEvent->type == EVENT_BUTTON_PUSH)
  {
    //If elevator at position of button push:
    //   Open doors and create departure event
    if (_elevator->currentPosition == curEvent->source)
    {
      addEvent(&_eventList, EVENT_DEPARTURE, time + 10, curEvent->source, curEvent->destination,0);
      _elevator->stateFuncPtr = elevatorDoorsOpen;
    }

    //
    // If button push at some other position than elevator:
    //   Move elevator to floor of button push.
    //   Create arrival event.
    else
    {
      _elevator->currentPosition = curEvent->source;
      _elevator->destination = curEvent->destination;
      calcTime(time, curEvent->source, curEvent->destination);
      //addEvent(&_eventList, EVENT_ARRIVE, time + calcTime(time, curEvent->source, curEvent->destination), curEvent->source, _elevator->destination);
      _elevator->stateFuncPtr = elevatorMoving;
    }
  } /* if(_event.type == EVENT_BUTTON_PUSH) */
}

void elevatorMoving(struct elevator *_elevator, struct event *curEvent){
    //printf("SRC: %d, DEST: %d TIME: %d\n",curEvent->source, curEvent->destination, time);
    if(curEvent->source == curEvent->destination){
      _elevator->stateFuncPtr = elevatorDoorsOpen;
    }
    switch (curEvent->type){
    case EVENT_DEPARTURE:
    printf("--------------------------\n");
    //printf("-----DEPARTURE:-----\n SRC: %d, DEST: %d TIME: %d\n",curEvent->source, curEvent->destination, time);
    //Set elevator to depart from source and its destination to event destination when departing
    _elevator->currentPosition = curEvent->source;
    if(curEvent->button == 1){
    _elevator->numPassengers ++;
    printf("PASSANGER WILL ENTER\n");
    }
    printf("DEPARTING FROM: %d TIME: %d\n", _elevator->currentPosition, time);
    //addEvent(&_eventList, EVENT_ARRIVE, time + abs(curEvent->source - curEvent->destination)*5, curEvent->source, _elevator->destination);
    _elevator->destination = curEvent->destination;

    break;

    case EVENT_ARRIVE:
    //printf("-----ARRIVE:-----\n SRC: %d, DEST: %d TIME: %d\n",curEvent->source, curEvent->destination, time);
    //Set elevator to be att destination when arrived
    _elevator->currentPosition = curEvent->destination;
    addEvent(&_eventList, EVENT_DEPARTURE, time, _elevator->currentPosition, curEvent->source, 0);
    printf("ARRIVED AT: %d TIME: %d \n", _elevator->currentPosition, time);
  if(curEvent->button == 1){
    _elevator->numPassengers --;
    printf("PASSANGER WILL GO OUT\n");
  }
    _elevator->stateFuncPtr = elevatorDoorsOpen;
    break;

    case EVENT_BUTTON_PUSH:
    calcTime(time, curEvent->source, curEvent->destination);
    //addEvent(&_eventList, EVENT_DEPARTURE, time + calcTime(time, curEvent->source, curEvent->destination), curEvent->source, _elevator->destination);
    break;
    }
}

int calcTime(int time, int source, int dest){
  waitTime += abs(elevatorOn - source)*5 + 10;
  addEvent(&_eventList, EVENT_DEPARTURE, waitTime + time, elevatorOn, source, 0);
  addEvent(&_eventList, EVENT_ARRIVE, waitTime + time + 10, elevatorOn, source, 0);

  printf("Departing from src: %d, time: %d-------- Arriving at dest: %d, time: %d\n", elevatorOn, waitTime + time, source, waitTime + time + 10);
  //printf("Arrive time for dest: %d = %d\n", source, waitTime + time + 10);

  waitTime += abs(source - dest)*5 + 10;
  addEvent(&_eventList, EVENT_DEPARTURE, waitTime + time, source, dest, 1);
  addEvent(&_eventList, EVENT_ARRIVE, waitTime + time + 10, source, dest, 1);
  printf("Departing from src: %d, time: %d-------- Arriving at dest: %d, time: %d\n", source, waitTime + time, dest, waitTime + time + 10);

  //printf("Departure time for src: %d, dest: %d = %d\n", source, dest, waitTime + time);
  //printf("Arrive time for dest: %d = %d\n", dest, waitTime + time + 10);

  //The "middle" step
  elevatorOn = dest;
  return waitTime;
}

void elevatorDoorsOpen(struct elevator *_elevator, struct event *curEvent){
  printf("doors open\n\n");
  _elevator->destination = curEvent->source;
  _elevator->stateFuncPtr = elevatorMoving;
}

void printEvent(struct event *curEvent)
{
  switch (curEvent->type)
  {
  case EVENT_BUTTON_PUSH:
    printf("\nEVENT:\nType: BUTTON_PUSH");
    break;
  case EVENT_ARRIVE:
    printf("\nEVENT:\nType: ARRIVE");
    break;
  case EVENT_DEPARTURE:
    printf("\nEVENT:\nType: DEPATURE");
    break;
  }
  printf("\nTime: %d \nSource: %d \nDestination: %d\n\n", curEvent->time, curEvent->source, curEvent->destination);
  printf("-------------------\n");
}