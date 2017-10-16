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
  this->size = size;
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
  *(this->array + (this->tail++ % this->size)) = data;
  if (this->head % this->size == this->tail % this->size)
    this->status = FULL;
}

void dequeueArray(array_queue_t* this, char* data) {
  if (this->status == EMPTY) { *data = 0; return; }
  this->status = READY;
  *data = *(this->array + (this->head++ % this->size));
  if (this->head % this->size == this->tail % this->size)
    this->status = EMPTY;
}

void resizeArray(array_queue_t* this) {
  if (this->size == 0) {
    free(this->array);
    this->array = malloc(sizeof(char));
    this->size = 1;
    this->tail = 0;
    this->status = EMPTY;
  } else {
    char* new_array = malloc(sizeof(char) * this->size * 2);
    char* temp_ptr = new_array;
    do {
      *temp_ptr++ = *(this->array + (this->head++ % this->size));
    } while(this->head % this->size != this->tail % this->size);
    free(this->array);
    this->array = new_array;
    this->tail = this->size;
    this->size = this->size * 2;
    this->status = READY;
  }
  this->head = 0;
}
