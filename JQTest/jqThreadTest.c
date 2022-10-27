
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"
#include "jqThreadTest.h"


void TestTest() {
  printf("Testing\n");
}


char* threadTestBlock() {
  
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 5);
  
  pthread_t thread1;
  pthread_create(&thread1, NULL, popBlockThreadFunc, (void*)jq1);
  void* vp;
  job_queue_push(jq1, vp);
  
  while(jq1->size > 0){}
  sleep(3);
  printf("with \"If\", the following test is valid:\n");
  job_queue_push(jq1, vp);
  pthread_join(thread1, NULL);
  
  job_queue_destroy(jq1);
  
  free(jq1);
  
  return "true =)\n";
}

void* popBlockThreadFunc(void* jqp){
  struct job_queue* jq1 = jqp;
  void* vP;
  while(jq1->size <= 0) {}
  job_queue_pop(jq1, &vP);
  printf("If this sentence starts ");
  job_queue_pop(jq1, &vP);
}

/*
char* jqTest(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == -1) return "true =)";
  return "false =(";
}
char* jqTest(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == -1) return "true =)";
  return "false =(";
}

char* jqTest(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == -1) return "true =)";
  return "false =(";
}
char* jqTest(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == -1) return "true =)";
  return "false =(";
}
*/
