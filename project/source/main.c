// @Copyright 2017 Scott Leland Crossen

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "arrayqueue.h"
#include "linkedqueue.h"
#include "mmqueue.h"

void testLinked(linked_queue_t* linked_queue, unsigned int size, unsigned int multiplier);
void testArray(array_queue_t* array_queue, unsigned int size, unsigned int multiplier);
void testMM(mm_queue_t* mm_queue, unsigned int size, unsigned int multiplier);
double now();

int main(int argc, char **argv) {
  printf("****************\r\n");
  printf("Program starting\r\n");
  int page_size = getpagesize();
  double before;
  double after;
  unsigned int multiplier = 20;
  linked_queue_t* linked_queue = createLinkedQueue(page_size);
  array_queue_t* array_queue = createArrayQueue(page_size);
  mm_queue_t* mm_queue = createMMQueue(page_size);
  printf("Queues created\r\n");
  printf("Testing linked list queue\r\n");
  before = now();
  testLinked(linked_queue, page_size, multiplier);
  after = now();
  printf("The linked list   queue took %lf microseconds over %d operations for a total of %lf microseconds/operations\r\n", after - before, page_size * 3 * multiplier, (after - before) / (page_size * 3 * multiplier));
  printf("Testing array queue\r\n");
  before = now();
  testArray(array_queue, page_size, multiplier);
  after = now();
  printf("The array         queue took %lf microseconds over %d operations for a total of %lf microseconds/operations\r\n", after - before, page_size * 3 * multiplier, (after - before) / (page_size * 3 * multiplier));
  printf("Testing memory mapped queue\r\n");
  before = now();
  testMM(mm_queue, page_size, multiplier);
  after = now();
  printf("The memory-mapped queue took %lf microseconds over %d operations for a total of %lf microseconds/operations\r\n", after - before, page_size * 3 * multiplier, (after - before) / (page_size * 3 * multiplier));
  printf("Deleting queues\r\n");
  destroyLinkedQueue(linked_queue);
  destroyArrayQueue(array_queue);
  destroyMMQueue(mm_queue);
  printf("Program finished\r\n");
  printf("****************\r\n");
}

void testLinked(linked_queue_t* linked_queue, unsigned int testing_size, unsigned int multiplier) {
  for(int j = 0; j < multiplier; j++) {
    char garbage;
    for (int i = 0; i < testing_size; i++) {
      enqueueLinked(linked_queue, 'a');
    }
    for (int i = 0; i < testing_size / 2; i++) {
      dequeueLinked(linked_queue, &garbage);
    }
    for (int i = 0; i < testing_size / 2; i++) {
      enqueueLinked(linked_queue, 'a');
    }
    for (int i = 0; i < testing_size; i++) {
      dequeueLinked(linked_queue, &garbage);
    }
  }
}

void testArray(array_queue_t* array_queue, unsigned int testing_size, unsigned int multiplier) {
  for(int j = 0; j < multiplier; j++) {
    char garbage;
    for (int i = 0; i < testing_size; i++) {
      enqueueArray(array_queue, 'a');
    }
    for (int i = 0; i < testing_size / 2; i++) {
      dequeueArray(array_queue, &garbage);
    }
    for (int i = 0; i < testing_size / 2; i++) {
      enqueueArray(array_queue, 'a');
    }
    for (int i = 0; i < testing_size; i++) {
      dequeueArray(array_queue, &garbage);
    }
  }
}

void testMM(mm_queue_t* mm_queue, unsigned int testing_size, unsigned int multiplier) {
  for(int j = 0; j < multiplier; j++) {
    char garbage;
    for (int i = 0; i < testing_size; i++) {
      enqueueMM(mm_queue, 'a');
    }
    for (int i = 0; i < testing_size / 2; i++) {
      dequeueMM(mm_queue, &garbage);
    }
    for (int i = 0; i < testing_size / 2; i++) {
      enqueueMM(mm_queue, 'a');
    }
    for (int i = 0; i < testing_size; i++) {
      dequeueMM(mm_queue, &garbage);
    }
  }
}

double now() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) tp.tv_usec + (double) tp.tv_sec * 1e6);
}
