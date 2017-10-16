// @Copyright 2017 Scott Leland Crossen

typedef struct queue_node_t {
  char                  value;
  struct queue_node_t*  next;
} queue_node_t;

typedef struct linked_queue_t {
  queue_node_t*         head;
  queue_node_t*         tail;
} linked_queue_t;

linked_queue_t* createLinkedQueue();

void destroyLinkedQueue(linked_queue_t* this);

void enqueueLinked(linked_queue_t* this, char data);

void dequeueLinked(linked_queue_t* this, char* data);
