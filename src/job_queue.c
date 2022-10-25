#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"

int job_queue_init(struct job_queue *job_queue, int capacity) {
  job_queue->capacity = capacity;
  job_queue->size = 0;
  job_queue->start = (void*)malloc(
    capacity * sizeof(void*));
  job_queue->front = 0;
  job_queue->back = (capacity - 1) * sizeof(void*);
  
  return 1;  
}

int job_queue_destroy(struct job_queue *job_queue) {
  if (Empty(job_queue)){
    free(job_queue);
  }
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  while(job_queue->size >= job_queue->capacity);
  job_queue->start + (job_queue->back * sizeof(void*)) = data;
  job_queue->back++;
  if(job_queue->back >= job_queue->capacity) {
    job_queue->back = 0;
  }
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  /*if (Empty(job_queue)){
  }
  else if(job_queue -> index findes)
  {
    remove element
  }
  else if (job_queue_destroy(job_queue))
  {
    return -1;
  }
  else{
    return 1;
  }*/
  
  
}
