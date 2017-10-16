// @Copyright 2017 Scott Leland Crossen

typedef enum {EMPTY, READY, FULL} queue_status;

typedef struct array_queue_t {
  char*               array;
  unsigned int        head;
  unsigned int        tail;
  unsigned int        allocated;
  queue_status        status;
} array_queue_t;

array_queue_t* createArrayQueue(unsigned int size);

void destroyArrayQueue(array_queue_t* this);

void enqueueArray(array_queue_t* this, char data);

void dequeueArray(array_queue_t* this, char* data);
