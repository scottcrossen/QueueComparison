// @Copyright 2017 Scott Leland Crossen

#include "mmqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void resizeMM(mm_queue_t* this);

mm_queue_t* createMMQueue(unsigned int size) {
  mm_queue_t* this = malloc(sizeof(mm_queue_t));
  int page_size = getpagesize();
  int obj_size = sizeof(char);
  this->allocated = (size * obj_size / page_size + ((size * obj_size) % page_size != 0)) * page_size;
  this->size = size;
  if ((this->file_descriptor = shm_open("queue_region", O_RDWR|O_CREAT, 0600)) == -1) {
    printf("Error on obtaining anonymous file: %s\r\n", strerror(errno));
  }
  if (ftruncate(this->file_descriptor, this->allocated) != 0){
    printf("Could not set size of anonymous file: %s\r\n", strerror(errno));
  }
  if((this->buffer = (char *) mmap(NULL, 2 * this->allocated, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
    printf("Could not allocate virtual memory: %s\r\n", strerror(errno));
  }
  if(mmap(this->buffer, this->allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED){
    printf("Could not map first buffer into virtual memory: %s\r\n", strerror(errno));
  }
  if(mmap(this->buffer + this->allocated / obj_size, this->allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED){
    printf("Could not map second buffer into virtual memory: %s\r\n", strerror(errno));
  }
  this->head = 0;
  this->tail = 0;
  return this;
}

void destroyMMQueue(mm_queue_t* this) {
  int obj_size = sizeof(char);
  if(munmap(this->buffer, this->allocated) != 0){
    printf("Could not unmap buffer: %s\r\n", strerror(errno));
  }
  if(munmap(this->buffer + this->allocated / obj_size, this->allocated) != 0){
    printf("Could not unmap buffer: %s\r\n", strerror(errno));
  }
  if(close(this->file_descriptor) != 0){
    printf("Could not close anonymous file: %s\r\n", strerror(errno));
  }
}

void enqueueMM(mm_queue_t* this, char data) {
  if (this->tail - this->head >= this->size) {
    resizeMM(this);
  }
  *(this->buffer+(++this->tail % (2 * this->allocated))) = data;
}

void dequeueMM(mm_queue_t* this, char* data) {
  if (this->head >= this->tail) {
    *(data) = 0;
    return;
  }
  *(data) = *(this->buffer+(++this->head % (2 * this->allocated)));
}

void resizeMM(mm_queue_t* this) {
  int obj_size = sizeof(char);
  unsigned int new_size = this->size * 2 * obj_size;
  if (new_size >= this->allocated / sizeof(char)) {
    // Warning:  I haven't tested this segment of code.
    unsigned int new_allocated = this->allocated * 2;
    if (ftruncate(this->file_descriptor, new_allocated) != 0){
      printf("Could not resize anonymous file: %s\r\n", strerror(errno));
    }
    char* new_buffer;
    if((new_buffer = (char *) mmap(NULL, 2 * new_allocated, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
      printf("Could not reallocate virtual memory: %s\r\n", strerror(errno));
    }
    if(mmap(new_buffer, new_allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED) {
      printf("Could not map first buffer into virtual memory: %s\r\n", strerror(errno));
    }
    if(mmap(new_buffer + new_allocated / obj_size, new_allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED) {
      printf("Could not map second buffer into virtual memory: %s\r\n", strerror(errno));
    }
    unsigned int new_tail = new_size;
    unsigned int new_head = new_size;
    while (this->head < this->tail) {
      *(new_buffer + (++new_tail)) = *(this->buffer+(++this->head % (2 * this->allocated)));
    }
    if(munmap(this->buffer, this->allocated) != 0){
      printf("Could not unmap buffer: %s\r\n", strerror(errno));
    }
    if(munmap(this->buffer + this->allocated / obj_size, this->allocated) != 0){
      printf("Could not unmap buffer: %s\r\n", strerror(errno));
    }
    this->head = new_head;
    this->tail = new_tail;
  }
  this->size = new_size;
}
