// @Copyright 2017 Scott Leland Crossen

#include <stdio.h>
#include <stdlib.h>
#include "arrayqueue.h"

void resizeArray(array_queue_t* this);
int handleArrayEmpty(array_queue_t* this);
int handleArrayFull(array_queue_t* this);

array_queue_t* createArrayQueue(unsigned int size, full_behavior_t full_behavior, empty_behavior_t empty_behavior) {
	array_queue_t* this = malloc(sizeof(array_queue_t));
  this->array = malloc(sizeof(char) * size);
  this->head = 0;
  this->tail = 0;
  this->allocated = size;
  this->status = (size > 0) ? EMPTY : FULL;
  this->full_behavior = full_behavior;
  this->empty_behavior = empty_behavior;

  if(pthread_mutex_init(&this->lock, NULL) != 0){
    fprintf(stderr, "Could not initialize mutex\r\n");
    abort();
  }
  if(pthread_cond_init(&this->readable, NULL) != 0){
    fprintf(stderr, "Could not initialize readable condition variable\r\n");
    abort();
  }
  if(pthread_cond_init(&this->writeable, NULL) != 0){
    fprintf(stderr, "Could not initialize writable condition variable\r\n");
    abort();
  }

  return this;
}

void destroyArrayQueue(array_queue_t* this) {
  free(this->array);
  this->array = NULL;

  if(pthread_mutex_destroy(&this->lock) != 0){
    fprintf(stderr, "Could not destroy mutex\r\n");
    abort();
  }
  if(pthread_cond_destroy(&this->readable) != 0){
    fprintf(stderr, "Could not destroy readable condition variable\r\n");
    abort();
  }
  if(pthread_cond_destroy(&this->writeable) != 0){
    fprintf(stderr, "Could not destroy writable condition variable\r\n");
    abort();
  }

  free(this);
  this = NULL;
}

void enqueueArray(array_queue_t* this, char data) {
  pthread_mutex_lock(&this->lock);
  if (this->status == FULL) {
    if (handleArrayFull(this) != 0) {
      pthread_mutex_unlock(&this->lock);
      return;
    }
  }
  this->status = READY;
  *(this->array + (this->tail++ % this->allocated)) = data;
  if (this->head % this->allocated == this->tail % this->allocated) {
    this->status = FULL;
  }
  pthread_cond_signal(&this->readable);
  pthread_mutex_unlock(&this->lock);
}

void dequeueArray(array_queue_t* this, char* data) {
  pthread_mutex_lock(&this->lock);
  if (this->status == EMPTY) {
    if(handleArrayEmpty(this) != 0) {
      *data = 0;
      pthread_mutex_unlock(&this->lock);
      return;
    }
  }
  this->status = READY;
  *data = *(this->array + (this->head++ % this->allocated));
  if (this->head % this->allocated == this->tail % this->allocated) {
    this->status = EMPTY;
  }
  pthread_cond_signal(&this->writeable);
  pthread_mutex_unlock(&this->lock);
}

int handleArrayEmpty(array_queue_t* this) {
  switch (this->empty_behavior) {
    case EMPTY_WAIT:
      printf("Queue empty: Waiting for data before removing\r\n");
      while(this->status == EMPTY) {
        pthread_cond_wait(&this->writeable, &this->lock);
      }
      return 0;
    case EMPTY_IGNORE:
      printf("Queue empty: Returning null\r\n");
      return -1;
    case EMPTY_ABORT:
      printf("Queue empty: Aborting\r\n");
      fprintf(stderr, "queue empty\r\n");
      abort();
    default:
      printf("Queue empty: Unknown behavior type - Ignoring\r\n");
      return -2;
  }
}

int handleArrayFull(array_queue_t* this) {
  switch (this->full_behavior) {
    case FULL_RESIZE:
      printf("Queue full: Resizing queue\r\n");
      resizeArray(this);
      return 0;
    case FULL_WAIT:
      printf("Queue full: Waiting for space before adding\r\n");
      while (this->status == FULL) {
        pthread_cond_wait(&this->writeable, &this->lock);
      }
      return 0;
    case FULL_IGNORE:
      printf("Queue full: Dropping added data\r\n");
      return -1;
    case FULL_ABORT:
      printf("Queue full: Aborting\r\n");
      fprintf(stderr, "queue full\r\n");
      abort();
    default:
      printf("Queue full: Unknown behavior type - dropping\r\n");
      return -2;
  }
}

void resizeArray(array_queue_t* this) {
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
