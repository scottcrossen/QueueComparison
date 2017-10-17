// @Copyright 2017 Scott Leland Crossen

#include "mmqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

mm_queue_t* createMMQueue(unsigned int req_size) {
  mm_queue_t* this = malloc(sizeof(mm_queue_t));
  int page_size = getpagesize();
  this->allocated = (req_size * sizeof(char) / page_size + ((req_size * sizeof(char)) % page_size != 0)) * page_size;
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
    printf("Could not map buffer into virtual memory: %s\r\n", strerror(errno));
  }
  if(mmap(this->buffer + this->allocated, this->allocated, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, this->file_descriptor, 0) == MAP_FAILED){
    printf("Could not map buffer into virtual memory: %s\r\n", strerror(errno));
  }
  this->size = this->allocated / sizeof(char);
  this->head = 0;
  this->tail = 0;
  return this;
}

void destroyMMQueue(mm_queue_t* this) {
  if(munmap(this->buffer + this->allocated, this->allocated) != 0){
    printf("Could not unmap buffer: %s\r\n", strerror(errno));
  }
  if(munmap(this->buffer, this->allocated) != 0){
    printf("Could not unmap buffer: %s\r\n", strerror(errno));
  }
  if(close(this->file_descriptor) != 0){
    printf("Could not close anonymous file: %s\r\n", strerror(errno));
  }
}

void enqueueMM(mm_queue_t* this, char data) {
  if (this->tail - this->head >= this->size) {
    printf("Warning: Queue is not dynamic. Insert ignored.\r\n");
    return;
  }
  *(this->buffer+(++this->tail % (2 * this->size))) = data;
}

void dequeueMM(mm_queue_t* this, char* data) {
  if (this->head >= this->tail) {
    *(data) = 0;
    return;
  }
  *(data) = *(this->buffer+(++this->head % (2 * this->size)));
}
