#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"

int job_queue_init(struct job_queue *job_queue, int capacity) {
  job_queue->start = malloc(capacity * sizeof(void*));
  if(job_queue->start == NULL) return -1;
  
  job_queue->capacity = capacity;
  job_queue->size = 0;
  job_queue->front = 0;
  job_queue->back = 0;
  
  return 0;  
}

int job_queue_destroy(struct job_queue *job_queue) {
  while (job_queue->size > 0) {}//printf("destroy!\n");
  free(job_queue->start);						// The only allocation we made IN the queue was for start
  job_queue->start = NULL;						// Set start to NULL so that we can check if a queue function
									// was called on a destroyed queue
  if(job_queue->start == NULL) return 0;				
  return -1;
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  if(job_queue->start == NULL) return -1; 				// We set the start to NULL on destruction
  while(job_queue->size >= job_queue->capacity){} 			// Hold until there's space in the queue
  //void* vp = &job_queue->start + (job_queue->back * sizeof(void*));	// "Index into" current queue back
  //vp = data;
  job_queue->start[job_queue->back] = data;	
  //printf("Second %s", (char*)job_queue->start[job_queue->back]);	
  if(job_queue->start[job_queue->back] != data) return -2;		// Check that copy worked before updating the queue
  
  job_queue->size++;
  job_queue->back++;
  if(job_queue->back >= job_queue->capacity) {				// Make sure that the queue wraps
    job_queue->back = 0;
  }
  return 0;
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  if(job_queue->start == NULL) return -1; 
  while (job_queue->size <= 0){} 					// Block until queue has content
  //void* vp = job_queue->start + (job_queue->front * sizeof(void*));	// "Index into" current queue front
  //data = &vp;
  *data = job_queue->start[job_queue->front];
  void* vp = *data;
  //printf("Pop %s\n", (char*)vp);								
  //if(data != &job_queue->start[job_queue->front]) return -2;		// Check that the copy worked before we update the queue
  
  job_queue->front++;							
  if(job_queue->front >= job_queue->capacity) {				// Ensure that the queue wraps
    job_queue->front = 0;
  }
  
  job_queue->size--;							
  //printf("weird\n");
  return 0;
}
