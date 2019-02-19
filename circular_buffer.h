#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H


struct circularBuffer{
  int * data;
  int head;
  int tail;
  int maxLength;
  int length;
  int size;
  char isFull;
};


/* 
 * Data structure used to hold a circular buffer.
 *
 * In your elevator simulator, you will use a circular buffer in order to keep 
 * track of which outside elevator buttons have been pushed, hence keeping track 
 * of where passengers are waiting to be picked up. 
 *
 * You are suggested to use the following representation of button pushes
 *
 * 0: Button push on level 0
 * 1: Button push on level 1, going up.
 * -1: Button push on level 1, going down.
 * 2: Button push on level 2, going up.
 * -2: Button push on level 2, going down.
 * ...
 * NUM_LEVEL-2: Button push on level NUM_LEVEL-2, going up.
 * -(NUM_LEVEL-2): Button push on level NUM_LEVEL-2, going down.
 * -(NUM_LEVEL-1): Button push on level NUM_LEVEL-1, going down.
 *
 * Hence, the following values are used to represent button pushes:
 *   -(NUM_LEVEL-1), -(NUM_LEVEL-2), ..., -2, -1, 0, 1, 2, ..., NUM_LEVEL-2
 *
 */
void initCircularBuffer(struct circularBuffer* bufferPtr, int* data, int maxLen);



/* 
 * This function should check if the buffer pointed to by bufferPtr
 * contains one or more elements with the value specified by the 
 * 'value' argument.
 *
 * The function should return:
 *  - 'value' if the an element with the input value was found in the queue.
 *  - INT_MIN (defined in limits.h) if value was not found.
 */ 
int containsCircularBuffer(struct circularBuffer* bufferPtr, int value);

/*
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was added.
 */
int addElementCircularBuffer(struct circularBuffer* bufferPtr, int value);



/* 
 * This function should remove all elements in the buffer whose
 * value matches the 'value' argument.
 * 
 * The function should return:
 *  The argument value: if at least one element was removed from queue.
 *  INT_MIN (defined in limits.h) if no element was removed. 
 */
int removeValueCircularBuffer(struct circularBuffer* bufferPtr, int value);


/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     the queue was empty when the function was called.       
 */
int removeHeadCircularBuffer(struct circularBuffer* bufferPtr);

/* 
 * Get the value of the oldest element without removing it.
 * If buffer is empty, return INT_MIN (defined in limits.h) 
 */
int getElementCircularBuffer(struct circularBuffer* bufferPtr);


#endif

