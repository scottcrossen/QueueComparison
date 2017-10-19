// @Copyright 2017 Scott Leland Crossen

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mmqueue.h"

void resizeMM(mm_queue_t* this);
int handleMMEmpty(mm_queue_t* this);
int handleMMFull(mm_queue_t* this);

mm_queue_t* createMMQueue(unsigned int size, full_behavior_t full_behavior, empty_behavior_t empty_behavior) {
  mm_queue_t* this = malloc(sizeof(mm_queue_t));
  int page_size = getpagesize();
  int obj_size = sizeof(char);
  this->allocated = (size * obj_size / page_size + ((size * obj_size) % page_size != 0)) * page_size;
  this->size = size;
  if ((this->file_descriptor = shm_open("queue_region", O_RDWR|O_CREAT, 0600)) == -1) {
    printf("Error on obtaining anonymous file: %s\r\n", strerror(errno));
  }
  if (ftruncate(this->file_descriptor, this->allocated) != 0){
    printf("Could not set size of anonymous file: %s\r\n", strerror(errno));
  }
  if((this->buffer = (char *) mmap(NULL, 2 * this->allocated, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
    printf("Could not allocate virtual memory: %s\r\n", strerror(errno));
  }
  if(mmap(this->buffer, this->allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED){
    printf("Could not map first buffer into virtual memory: %s\r\n", strerror(errno));
  }
  if(mmap(this->buffer + this->allocated / obj_size, this->allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED){
    printf("Could not map second buffer into virtual memory: %s\r\n", strerror(errno));
  }
  this->head = 0;
  this->tail = 0;
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

void destroyMMQueue(mm_queue_t* this) {
  int obj_size = sizeof(char);
  if(munmap(this->buffer, this->allocated) != 0){
    printf("Could not unmap buffer: %s\r\n", strerror(errno));
  }
  if(munmap(this->buffer + this->allocated / obj_size, this->allocated) != 0){
    printf("Could not unmap buffer: %s\r\n", strerror(errno));
  }
  if(close(this->file_descriptor) != 0){
    printf("Could not close anonymous file: %s\r\n", strerror(errno));
  }

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

void enqueueMM(mm_queue_t* this, char data) {
  pthread_mutex_lock(&this->lock);
  if (this->tail - this->head >= this->size) {
    if (handleMMFull(this) != 0) {
      pthread_mutex_unlock(&this->lock);
      return;
    }
  }
  /* uncomment the following for worse performance but better adherence to
  assignment specifications: (reduces speed by 3) */
  /*
  if (this->head >= this->allocated / sizeof(char)) {
    this->head = this->head - this->allocated;
    this->tail = this->tail - this->allocated;
  }
  */
  *(this->buffer+(++this->tail % (2 * this->allocated))) = data;
  pthread_cond_signal(&this->readable);
  pthread_mutex_unlock(&this->lock);
}

void dequeueMM(mm_queue_t* this, char* data) {
  pthread_mutex_lock(&this->lock);
  if (this->head >= this->tail) {
    if(handleMMEmpty(this) != 0) {
      *(data) = 0;
      pthread_mutex_unlock(&this->lock);
      return;
    }
  }
  *(data) = *(this->buffer+(++this->head % (2 * this->allocated)));
  pthread_cond_signal(&this->writeable);
  pthread_mutex_unlock(&this->lock);
}

int handleMMEmpty(mm_queue_t* this) {
  switch (this->empty_behavior) {
    case EMPTY_WAIT:
      printf("Queue empty: Waiting for data before removing\r\n");
      while(this->head >= this->tail) {
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

int handleMMFull(mm_queue_t* this) {
  switch (this->full_behavior) {
    case FULL_RESIZE:
      printf("Queue full: Resizing queue\r\n");
      resizeMM(this);
      return 0;
    case FULL_WAIT:
      printf("Queue full: Waiting for space before adding\r\n");
      while(this->tail - this->head >= this->size) {
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

void resizeMM(mm_queue_t* this) {
  int obj_size = sizeof(char);
  unsigned int new_size = this->size * 2 * obj_size;
  if (new_size >= this->allocated / sizeof(char)) {
    unsigned int new_allocated = this->allocated * 2;
    if (ftruncate(this->file_descriptor, new_allocated) != 0){
      printf("Could not set size of anonymous file: %s\r\n", strerror(errno));
    }
    char* new_buffer;
    if((new_buffer = (char *) mmap(NULL, 2 * new_allocated, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
      printf("Could not reallocate virtual memory: %s\r\n", strerror(errno));
    }
    if(mmap(new_buffer, new_allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED) {
      printf("Could not map first buffer into virtual memory: %s\r\n", strerror(errno));
    }
    if(mmap(new_buffer + new_allocated / obj_size, new_allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED) {
      printf("Could not map second buffer into virtual memory: %s\r\n", strerror(errno));
    }
    unsigned int new_tail = new_size;
    unsigned int new_head = new_size;
    while (this->head < this->tail) {
      *(new_buffer + (++new_tail)) = *(this->buffer+(++this->head % (2 * this->allocated)));
    }
    if(munmap(this->buffer, this->allocated) != 0){
      printf("Could not unmap buffer: %s\r\n", strerror(errno));
    }
    if(munmap(this->buffer + this->allocated / obj_size, this->allocated) != 0){
      printf("Could not unmap buffer: %s\r\n", strerror(errno));
    }
    this->head = new_head;
    this->tail = new_tail;
    this->buffer = new_buffer;
  }
  this->size = new_size;
}
