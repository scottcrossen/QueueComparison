// @Copyright 2017 Scott Leland Crossen

#include <pthread.h>
#include "global.h"

typedef enum {EMPTY, READY, FULL} queue_status;

typedef struct array_queue_t {
  char*               array;
  unsigned int        head;
  unsigned int        tail;
  unsigned int        allocated;
  queue_status        status;
  full_behavior_t     full_behavior;
  empty_behavior_t    empty_behavior;
  pthread_cond_t      readable;
  pthread_cond_t      writeable;
  pthread_mutex_t     lock;
} array_queue_t;

array_queue_t* createArrayQueue(unsigned int size, full_behavior_t full_behavior, empty_behavior_t empty_behavior);

void destroyArrayQueue(array_queue_t* this);

void enqueueArray(array_queue_t* this, char data);

void dequeueArray(array_queue_t* this, char* data);
