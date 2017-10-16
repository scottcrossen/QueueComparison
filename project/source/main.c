// @Copyright 2017 Scott Leland Crossen

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "linkedqueue.h"
#include "arrayqueue.h"
#include "mmqueue.h"

int main(int argc, char **argv) {
  printf("%s\r\n", "****************");
  printf("%s\r\n", "Program starting");
  linked_queue_t* linked_queue = createLinkedQueue();
  array_queue_t* array_queue = createArrayQueue(2);
  mm_queue_t* mm_queue = createMMQueue(2);
  printf("%s\r\n", "Queues created");


  char a;
  enqueueLinked(linked_queue, 'a');
  enqueueLinked(linked_queue, 'b');
  enqueueLinked(linked_queue, 'c');
  dequeueLinked(linked_queue, &a); printf("%c\r\n", a);
  dequeueLinked(linked_queue, &a); printf("%c\r\n", a);
  dequeueLinked(linked_queue, &a); printf("%c\r\n", a);
  enqueueArray(array_queue, 'a');
  enqueueArray(array_queue, 'b');
  enqueueArray(array_queue, 'c');
  dequeueArray(array_queue, &a); printf("%c\r\n", a);
  dequeueArray(array_queue, &a); printf("%c\r\n", a);
  dequeueArray(array_queue, &a); printf("%c\r\n", a);
  enqueueMM(mm_queue, 'a');
  enqueueMM(mm_queue, 'b');
  enqueueMM(mm_queue, 'c');
  dequeueMM(mm_queue, &a); printf("%c\r\n", a);
  dequeueMM(mm_queue, &a); printf("%c\r\n", a);
  dequeueMM(mm_queue, &a); printf("%c\r\n", a);


  printf("%s\r\n", "Deleting queues");
  destroyLinkedQueue(linked_queue);
  destroyArrayQueue(array_queue);
  destroyMMQueue(mm_queue);
  printf("%s\r\n", "Program finished");
  printf("%s\r\n", "****************");
}


double when() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}
