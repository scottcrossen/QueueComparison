// @Copyright 2017 Scott Leland Crossen

#include "linkedqueue.h"
#include <stdio.h>
#include <stdlib.h>

linked_list_queue* createQueue() {
	linked_list_queue* this = malloc(sizeof(linked_list_queue));
  this->size = 0;
	return this;
}

void destroyQueue(linked_list_queue* this) {
  queue_node* current = this->head;
  while (current != NULL) {
    queue_node* next = current->next;
    free(current);
    current = next;
  }
  free(this);
  this = NULL;
}

void enqueue(linked_list_queue* this, char data) {
  this->tail = malloc(sizeof(queue_node));
  this->tail->value = data;
  if (this->head == NULL) {
    this->head = this->tail;
  }
  this->size++;
}

void dequeue(linked_list_queue* this, char* data) {
  if(this->head == NULL) {
    *data = 0;
    return;
  }
  queue_node* to_pop = this->head;
  this->head = this->head->next;
  *data = to_pop->value;
  free(to_pop);
  to_pop = NULL;
  this->size--;
}
