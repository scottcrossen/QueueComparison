// @Copyright 2017 Scott Leland Crossen

#include "linkedqueue.h"
#include <stdio.h>
#include <stdlib.h>

void resizeLinked(linked_queue_t* this);

linked_queue_t* createLinkedQueue(unsigned int size) {
	linked_queue_t* this = malloc(sizeof(linked_queue_t));
  if (size > 0) {
    this->allocated = malloc(sizeof(queue_node_t));
    this->tail=this->allocated;
    unsigned int iter = 0;
    while (++iter < size) {
      this->allocated->next = malloc(sizeof(queue_node_t));
      this->allocated = this->allocated->next;
    }
  }
	return this;
}

void destroyLinkedQueue(linked_queue_t* this) {
  queue_node_t* current = (this->head != NULL) ? this->head : (this->tail != NULL) ? this->tail : NULL;
  while (current != NULL) {
    queue_node_t* next = current->next;
    free(current);
    current = next;
  }
  free(this);
  this = NULL;
}

void enqueueLinked(linked_queue_t* this, char data) {
  if (this->allocated == NULL) {
    resizeLinked(this);
    this->head = this->allocated;
    this->tail = this->allocated;
  } else if (this->head == NULL) {
    this->head = this->tail;
  } else if (this->tail == this->allocated) {
    resizeLinked(this);
    this->tail = this->allocated;
  } else {
    this->tail = this->tail->next;
  }
  this->tail->value = data;
}

void dequeueLinked(linked_queue_t* this, char* data) {
  if(this->head == NULL) {
    *data = 0;
    return;
  }
  *data = this->head->value;
  this->allocated->next = this->head;
  this->head = (this->head == this->tail) ? NULL : this->head->next;
  this->allocated->next = NULL;
}

void resizeLinked(linked_queue_t* this) {
  queue_node_t* new = malloc(sizeof(queue_node_t));
  if (this->allocated == NULL) {
    this->allocated = new;
  } else {
    this->allocated->next = new;
    this->allocated = this->allocated->next;
  }
}
