// @Copyright 2017 Scott Leland Crossen

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "linkedqueue.h"

int main(int argc, char **argv) {
  printf("\n%s", "****************");
  printf("\n%s", "Program Starting");
  linked_list_queue* queue = createQueue();
  printf("\n%s", "Queue created");
  enqueue(queue,'a');
  char a;
  dequeue(queue,&a);
  printf("\n%c",a);
  enqueue(queue,'b');
  dequeue(queue,&a);
  printf("\n%c",a);
  dequeue(queue,&a);
  printf("\n%c",a);
  destroyQueue(queue);
  printf("\n%s", "Program Finished");
  printf("\n%s", "****************");
}


double When() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}
