#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"

int job_queue_init(struct job_queue *job_queue, int capacity) {
  struct job_queue* job_queue = (struct job_queue*)malloc(
    sizeof(job_queue)); // kan være at det er capacity der skal bruges :)
  job_queue -> size = capacity;
  job_queue -> front = 0;
  job_queue -> back = capacity - 1;
  
  return 1;  
}

int job_queue_destroy(struct job_queue *job_queue) {
  if (Empty(job_queue)){
    free(job_queue);
  }
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  assert(0);
}
