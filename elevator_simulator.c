#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "event_list.h"	
#include "circular_buffer.h"
#include "passenger_list.h"
#include "elevator.h"


/* 
 * Assume the levels are numbered 0, 1, ... , NUM_LEVELS-1
 */
#define NUM_LEVELS 7

/*
 * Maximum number of passengers that can be in the elevator at the same time. 
 */ 
#define ELEVATOR_CAPACITY 10


void readButtonPushFile(struct eventList *_eventList);


/*
 * This integer represents the simulation clock. 
 * The time corresponds to seconds from the start of the simulation. 
 */
unsigned int time = 0;


/* 
 * Data structure holding button push queue 
 */
struct circularBuffer pushedButtonsQueue;

/* 
 * Data structure holding button queue data
 * 
 * Possible button pushes:
 *   Level 0: Up
 *   Level 1: Up, Down
 *   Level 2: Up, Down
 *   ...
 *   Level NUM_LEVELS-2: Up, Down
 *   Level NUM_LEVELS-1: Down 
 */
int pushedButtonsQueueData[(NUM_LEVELS * 2) - 2];


/* 
 * Date structure keeping track of the individuals that are waiting to go up
 * and down, respectively, at each of the levels.
 *
 * In waitingUp (where it is possible to go up from floor 0, ... ,NUM_LEVELS-2):
 *   waitingUp[0]: Passengers that want to go up from floor 0
 *   waitingUp[1]: Passengers that want to go up from floor 1
 *   ...
 *   waitingUp[NUM_LEVELS-2]: Passengers that want to go up from floor NUM_LEVELS-2
 *
 * In waitingDown (where it is possible to go down from floor 1,..., NUM_LEVELS-1):
 *   waitingDown[0]: Passengers that want to go down from floor 1
 *   waitingDown[2]: Passengers that want to go down from floor 2
 *   ...
 *   waitingDown[NUM_LEVELS-2]: Passengers that want to go down from floor NUM_LEVELS-1
 */ 
struct passengerList waitingUp[NUM_LEVELS - 1];
struct passengerList waitingDown[NUM_LEVELS - 1];
  

/*
 * Data structure holding the event list.
 * Make it global so that the elevator can easily add new events.
 */
struct eventList _eventList;

  


int main(void) {

  struct elevator _elevator;
  struct event currentEvent;

  
  
  
  /* Do some initialization */
  initEventList(&_eventList);

  /*
   * Button pushes are provided as input to the simulator in a file named "button_pushes".
   * 
   * The button pushes file should contain one row for each button push you want to provide
   * to the simulator. Each row corresponds to one passenger that wants to go from one 
   * particular floor to another floor.
   * 
   * Each row contains three integers separated by a single space: 
   *   time, source_floor, destination_floor
   * 
   * For example, we want to represent the following passangers:
   *   1. A passenger that wants to go from floor 2 to floor 4 at time 7.
   *   2. A passenger that wants to go from floor 3 to floor 2 at time 10.
   * 
   * This means that the corresponding button push file will contain the following two rows: 
   * 7 2 4
   * 10 3 2
   *
   * Please note that there should not be any blank row at the beginning or end of the file.
   *
   */
  readButtonPushFile(&_eventList);
  
  initCircularBuffer(&pushedButtonsQueue, pushedButtonsQueueData, (NUM_LEVELS * 2) - 2);

  for(int i = 0; i < NUM_LEVELS-1; i++) {
    initPassengerList(&(waitingUp[i]));
    initPassengerList(&(waitingDown[i]));
  }

  initElevator(&_elevator, NUM_LEVELS);
  

  /*
   * This is the main loop of the simulator, and it will run as long as there are 
   * events in the event list. 
   *
   * The events in this simulator is "button push", "depart", "arrive", and any other 
   * events you find suitable to add.
   *
   * In each iteration the simulator will perform the following steps:
   * 1. Get the next event from the event list (or event queue).
   * 2. Advance the simulation clock to the time of the next event. Obviously the 
   *    simulation clock will not be advanced if the time for the  
   * 3. If next event is an "button push" event, add passengers to the waiting list 
   *    at the floor of the button push. 
   * 4. If next event is an "button push" event, add button push to the button push queue. 
   * 5. Call the state function of elevator state machine to determine what do next.    
   */     
  while(getNextEvent(&_eventList, &currentEvent) != NULL) {
    struct event newEvent;
   
    /* Increment simulation time */
    time = currentEvent.time;

    //printf("\n------------------------------------------------\n");
    //printf("now time: %d\n", time);
    //printf("\nHandling event:\nsource: %d\ndest: %d\ntime: %d\n\n", currentEvent.source, currentEvent.destination, currentEvent.time);

    
    
            
    /*
     * Manage button push events outside elevator state machine
     */
    if (currentEvent.type == EVENT_BUTTON_PUSH) {

      

      
      if (currentEvent.destination > currentEvent.source) {
	addPassenger(&(waitingUp[currentEvent.source]), currentEvent.time, currentEvent.source, currentEvent.destination, SORT_TIME);

	if (containsCircularBuffer(&pushedButtonsQueue, currentEvent.source) == INT_MIN ) {
	  addElementCircularBuffer(&pushedButtonsQueue, currentEvent.source);
	}
      }
	
	
      
      /* Passenger wants to go down */
      else if (currentEvent.destination < currentEvent.source) {
	addPassenger(&(waitingDown[currentEvent.source-1]), currentEvent.time, currentEvent.source, currentEvent.destination, SORT_TIME);

	if (containsCircularBuffer(&pushedButtonsQueue, -currentEvent.source) == INT_MIN ) {
	  addElementCircularBuffer(&pushedButtonsQueue, -currentEvent.source);
	}
      }
    
      /* Disregard passengers that wants to go the same level they are already in */
      else {}
    }

    

    _elevator.stateFuncPtr(&_elevator, &currentEvent);


        
    
  } /* End while */

  return 0;
} /* end function main() */




  void readButtonPushFile(struct eventList *_eventList) {

    FILE *eventFile;
    char s[100];
    unsigned int time;
    int source, destination, retVal;

    /* Read event file */
    eventFile = fopen("button_pushes", "r");
  
  
    if(eventFile == NULL) {
      printf("Error: Cannot find event specification file <events>\n");
      exit(1);
    }

    /* Read first line and do nothing (it does not contain any event specification) */
    fgets(s, 100, eventFile);

    /* Read remaining lines. Requires that there are no empty line at the end of the file */
    while(fscanf(eventFile, "%u%d%d", &time, &source, &destination) == 3) {
      printf("time: %d, source: %d, dest: %d \n", time, source, destination);  
      addEvent(_eventList, EVENT_BUTTON_PUSH, time, source, destination);
    }

  
  
    fclose(eventFile);
  
  } /* End function readEventFile() */