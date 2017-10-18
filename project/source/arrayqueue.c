// @Copyright 2017 Scott Leland Crossen

#include "arrayqueue.h"
#include <stdio.h>
#include <stdlib.h>

void resizeArray(array_queue_t* this);

array_queue_t* createArrayQueue(unsigned int size) {
	array_queue_t* this = malloc(sizeof(array_queue_t));
  this->array = malloc(sizeof(char) * size);
  this->head = 0;
  this->tail = 0;
  this->allocated = size;
  this->status = (size > 0) ? EMPTY : FULL;
  return this;
}

void destroyArrayQueue(array_queue_t* this) {
  free(this->array);
  this->array = NULL;
  free(this);
  this = NULL;
}

void enqueueArray(array_queue_t* this, char data) {
  if (this->status == FULL) resizeArray(this);
  this->status = READY;
  *(this->array + (this->tail++ % this->allocated)) = data;
  if (this->head % this->allocated == this->tail % this->allocated)
    this->status = FULL;
}

void dequeueArray(array_queue_t* this, char* data) {
  if (this->status == EMPTY) { *data = 0; return; }
  this->status = READY;
  *data = *(this->array + (this->head++ % this->allocated));
  if (this->head % this->allocated == this->tail % this->allocated)
    this->status = EMPTY;
}

void resizeArray(array_queue_t* this) {
  printf("Warning: Resizing queue.");
  if (this->allocated == 0) {
    free(this->array);
    this->array = malloc(sizeof(char));
    this->allocated = 1;
    this->tail = 0;
    this->status = EMPTY;
  } else {
    char* new_array = malloc(sizeof(char) * this->allocated * 2);
    char* temp_ptr = new_array;
    do {
      *temp_ptr++ = *(this->array + (this->head++ % this->allocated));
    } while(this->head % this->allocated != this->tail % this->allocated);
    free(this->array);
    this->array = new_array;
    this->tail = this->allocated;
    this->allocated = this->allocated * 2;
    this->status = READY;
  }
  this->head = 0;
}
