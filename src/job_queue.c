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
  if (job_queue->size == 0){
    free(job_queue);
  }
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  while(job_queue->size >= job_queue->capacity);
  void* vp = job_queue->start + (job_queue->back * sizeof(void*));
  vp = data;
  job_queue->back++;
  if(job_queue->back >= job_queue->capacity) {
    job_queue->back = 0;
  }
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  
  while (job_queue->size == 0){
  }
  if (job_queue->size == NULL)
  {
    return -1;
  }
  void* vp = job_queue->start + (job_queue->front * sizeof(void*));
  if((int)*vp <= 0 || *vp > 0)
  {
    data = &job_queue->start +(job_queue->front * sizeof(void*));
    vp = job_queue->start + (job_queue->back * sizeof(void*)); 
    vp = NULL; 
    job_queue->front++;
    if (job_queue->front >= job_queue->capacity) {
      job_queue->front = 0; 
    }
    job_queue->size--;  
    return 0;
  }
  else{
    return 1;
  }
}
