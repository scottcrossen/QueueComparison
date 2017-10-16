// @Copyright 2017 Scott Leland Crossen

typedef struct queue_node {
  char                value;
  struct queue_node*  next;
} queue_node;

typedef struct linked_list_queue {
  queue_node*         head;
  queue_node*         tail;
  unsigned int        size;
} linked_list_queue;

linked_list_queue* createQueue();

void destroyQueue(linked_list_queue* this);

void enqueue(linked_list_queue* this, char data);

void dequeue(linked_list_queue* this, char* data);
