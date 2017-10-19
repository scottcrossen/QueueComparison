// @Copyright 2017 Scott Leland Crossen

#include <pthread.h>
#include "global.h"

typedef struct mm_queue_t {
  int               file_descriptor;
  char*             buffer;
  unsigned int      head;
  unsigned int      tail;
  unsigned int      size;
  unsigned int      allocated;
  full_behavior_t   full_behavior;
  empty_behavior_t  empty_behavior;
  pthread_cond_t    readable;
  pthread_cond_t    writeable;
  pthread_mutex_t   lock;
} mm_queue_t;

mm_queue_t* createMMQueue(unsigned int size, full_behavior_t full_behavior, empty_behavior_t empty_behavior);

void destroyMMQueue(mm_queue_t* this);

void enqueueMM(mm_queue_t* this, char data);

void dequeueMM(mm_queue_t* this, char* data);
