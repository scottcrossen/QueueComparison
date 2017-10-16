// @Copyright 2017 Scott Leland Crossen

#include "linkedqueue.h"
#include <stdio.h>
#include <stdlib.h>

linked_queue_t* createLinkedQueue(unsigned int size) {
	linked_queue_t* this = malloc(sizeof(linked_queue_t));
  /*this->allocated = malloc(sizeof(queue_node_t) * size);
  while (this->allocated)*/
	return this;
}

void destroyLinkedQueue(linked_queue_t* this) {
  queue_node_t* current = this->head;
  while (current != NULL) {
    queue_node_t* next = current->next;
    free(current);
    current = next;
  }
  free(this);
  this = NULL;
}

void enqueueLinked(linked_queue_t* this, char data) {
  queue_node_t* new = malloc(sizeof(queue_node_t));
  new->value = data;
  if (this->tail == NULL) {
    this->head = new;
    this->tail = new;
  } else {
    this->tail->next = new;
    this->tail = this->tail->next;
  }
}

void dequeueLinked(linked_queue_t* this, char* data) {
  if(this->head == NULL) {
    *data = 0;
    return;
  }
  *data = this->head->value;
  queue_node_t* next = this->head->next;
  free(this->head);
  this->head = next;
}
