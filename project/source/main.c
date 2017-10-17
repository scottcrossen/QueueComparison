// @Copyright 2017 Scott Leland Crossen

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "arrayqueue.h"
#include "linkedqueue.h"
#include "mmqueue.h"

int main(int argc, char **argv) {
  printf("****************\r\n");
  printf("Program starting\r\n");
  linked_queue_t* linked_queue = createLinkedQueue(1/*getpagesize()*/);
  array_queue_t* array_queue = createArrayQueue(1/*getpagesize()*/);
  mm_queue_t* mm_queue = createMMQueue(1/*getpagesize()*/);
  printf("Queues created\r\n");
  char a;
  printf("Testing linked list queue\r\n");
  enqueueLinked(linked_queue, 'a');
  enqueueLinked(linked_queue, 'b');
  dequeueLinked(linked_queue, &a); printf("%c\r\n", a);
  enqueueLinked(linked_queue, 'c');
  dequeueLinked(linked_queue, &a); printf("%c\r\n", a);
  dequeueLinked(linked_queue, &a); printf("%c\r\n", a);
  dequeueLinked(linked_queue, &a); printf("%c\r\n", a);
  printf("Testing array queue\r\n");
  enqueueArray(array_queue, 'a');
  enqueueArray(array_queue, 'b');
  dequeueArray(array_queue, &a); printf("%c\r\n", a);
  enqueueArray(array_queue, 'c');
  dequeueArray(array_queue, &a); printf("%c\r\n", a);
  dequeueArray(array_queue, &a); printf("%c\r\n", a);
  dequeueArray(array_queue, &a); printf("%c\r\n", a);
  printf("Testing memory mapped queue\r\n");
  enqueueMM(mm_queue, 'a');
  enqueueMM(mm_queue, 'b');
  dequeueMM(mm_queue, &a); printf("%c\r\n", a);
  enqueueMM(mm_queue, 'c');
  dequeueMM(mm_queue, &a); printf("%c\r\n", a);
  dequeueMM(mm_queue, &a); printf("%c\r\n", a);
  dequeueMM(mm_queue, &a); printf("%c\r\n", a);
  printf("Deleting queues\r\n");
  destroyLinkedQueue(linked_queue);
  destroyArrayQueue(array_queue);
  destroyMMQueue(mm_queue);
  printf("Program finished\r\n");
  printf("****************\r\n");
}

double now() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}
