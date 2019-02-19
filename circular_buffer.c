#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"

void initCircularBuffer(struct circularBuffer* bufferPtr, int* data, int maxLength) {
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->maxLength = maxLength;
  bufferPtr->size = 0;
}

int containsCircularBuffer(struct circularBuffer* bufferPtr, int value) {
 	for(int i = 0; i<= bufferPtr->size; i++){
		if(bufferPtr->data[i] == value){
			return value;
		}
	}
		return INT_MIN;
}

int addElementCircularBuffer(struct circularBuffer* bufferPtr, int value)  {
  int next = ((bufferPtr->tail + 1) % bufferPtr->maxLength);
	if(bufferPtr->size < bufferPtr->maxLength){
		bufferPtr->data[bufferPtr->tail] = value;
		bufferPtr->tail = next;
		bufferPtr->size++;
		return value;
	}
	return INT_MIN;
}

int removeValueCircularBuffer(struct circularBuffer* bufferPtr, int value) {
  int next = ((bufferPtr->tail - 1) % bufferPtr->maxLength);
	while(containsCircularBuffer(bufferPtr, value) == value){
		for(int i = 0; i <= bufferPtr->maxLength; i++){
			if(bufferPtr->data[i] == value){
				bufferPtr->data[i] = INT_MIN;
				bufferPtr->size--;
				if(next == -1){
					next = 3;
					bufferPtr->tail = next;
				} else {
					bufferPtr->tail = next;
				}
			}
		}
		for(int o=0; o <= bufferPtr->size; o++){
			if(bufferPtr->data[o] == INT_MIN){
				while(o <= bufferPtr->size){
					bufferPtr->data[o] = bufferPtr->data[o + 1];
					o++;
				}
			}
		}
		return value; 
	}
	return INT_MIN;
}

int removeHeadCircularBuffer(struct circularBuffer* bufferPtr) {
	if(bufferPtr->size = 0){
		return INT_MIN;
	}
 	int value = bufferPtr->data[bufferPtr->head];
	int next = ((bufferPtr->head + 1) % bufferPtr->maxLength);
	bufferPtr->data[bufferPtr->head] = 0;
	bufferPtr->head = next;
	bufferPtr->size --;
	return value;
}

int getElementCircularBuffer(struct circularBuffer* bufferPtr) {
  if(bufferPtr->size != 0){
    int first = bufferPtr->data[bufferPtr->head];
    return first;
  } else {
    return INT_MIN;
  }
}