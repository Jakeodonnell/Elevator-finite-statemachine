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
  
  printf("\n-----idle Handling event: source: %d dest:%d-----\n", curEvent->source, curEvent->destination);
  printf("TIME: %d\n", time);

  if (curEvent->type == EVENT_BUTTON_PUSH)
  {
    //If elevator at position of button push:
    //   Open doors and create departure event
    if (_elevator->currentPosition == curEvent->source)
    {
      addEvent(&_eventList, EVENT_DEPARTURE, time + 10, curEvent->source, curEvent->destination);
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

      printf("current possition: %d\n", _elevator->currentPosition);
      printf("current destination: %d\n", _elevator->destination);
      calcTime(time, curEvent->source, curEvent->destination);
      //addEvent(&_eventList, EVENT_ARRIVE, time + calcTime(time, curEvent->source, curEvent->destination), curEvent->source, _elevator->destination);
      _elevator->stateFuncPtr = elevatorMoving;
    }
  } /* if(_event.type == EVENT_BUTTON_PUSH) */
}

void elevatorMoving(struct elevator *_elevator, struct event *curEvent){
    //printf("SRC: %d, DEST: %d TIME: %d\n",curEvent->source, curEvent->destination, time);

    switch (curEvent->type){
    case EVENT_DEPARTURE:
    printf("TIME: %d\n", time);
    //printf("-----DEPARTURE:-----\n SRC: %d, DEST: %d TIME: %d\n",curEvent->source, curEvent->destination, time);
    //Set elevator to depart from source and its destination to event destination when departing
    _elevator->currentPosition = curEvent->source;
    printf("DEPARTING FROM: %d\n", _elevator->currentPosition);
    //addEvent(&_eventList, EVENT_ARRIVE, time + abs(curEvent->source - curEvent->destination)*5, curEvent->source, _elevator->destination);
    _elevator->destination = curEvent->destination;

    break;

    case EVENT_ARRIVE:
    //printf("-----ARRIVE:-----\n SRC: %d, DEST: %d TIME: %d\n",curEvent->source, curEvent->destination, time);
    //Set elevator to be att destination when arrived
    _elevator->currentPosition = curEvent->destination;
    addEvent(&_eventList, EVENT_DEPARTURE, time + 10, _elevator->currentPosition, curEvent->source);
    printf("ARRIVED AT: %d\n\n", _elevator->currentPosition);
    _elevator->stateFuncPtr = elevatorDoorsOpen;
    break;

    case EVENT_BUTTON_PUSH:
    calcTime(time, curEvent->source, curEvent->destination);
    //addEvent(&_eventList, EVENT_DEPARTURE, time + calcTime(time, curEvent->source, curEvent->destination), curEvent->source, _elevator->destination);
    break;
    }
}

int calcTime(int time, int source, int dest, struct elevator *_elevator, struct event *curEvent)
{
  waitTime += abs(elevatorOn - source)*5 + 10;

  addEvent(&_eventList, EVENT_DEPARTURE, waitTime + time, elevatorOn, source);
  addEvent(&_eventList, EVENT_ARRIVE, waitTime + time + 10, elevatorOn, source);

  printf("Departure time for src: %d, dest: %d = %d\n", elevatorOn, source, waitTime + time);
  printf("Arrive time for dest: %d = %d\n", source, waitTime + time + 10);

  waitTime += abs(source - dest)*5 + 10;
  addEvent(&_eventList, EVENT_DEPARTURE, waitTime + time, source, dest);
  addEvent(&_eventList, EVENT_ARRIVE, waitTime + time + 10, source, dest);

  printf("Departure time for src: %d, dest: %d = %d\n", source, dest, waitTime + time);
  printf("Arrive time for dest: %d = %d\n", dest, waitTime + time + 10);

  //The "middle" step
  elevatorOn = dest;
  return waitTime;
}

void elevatorDoorsOpen(struct elevator *_elevator, struct event *curEvent)
{
  //printf("\n-----doors Handling event: source: %d dest:%d-----\n", curEvent->source, curEvent->destination);
  //printf("doors open\n");
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