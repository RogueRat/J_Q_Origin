
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fts.h>
#include <err.h>

#include "jqTest.h"
#include "jqThreadTest.h"

int main() {

  FTS *ftsp;
  int fts_options = FTS_LOGICAL | FTS_NOCHDIR;
  char* tf = "TESTFOLDER";
  char* const* path = &tf;
  if ((ftsp = fts_open(path, fts_options, NULL)) == NULL) {
    err(1, "fts_open() failed");
    return -1;
  }
  FTSENT *p;
  p = fts_read(ftsp);
  printf("Path: %s\n", p->fts_path);
  p = fts_read(ftsp);
  printf("Path: %s\n", p->fts_path);
  FILE *f = fopen(p->fts_path, "r");
  char word[100];
  fread(word, sizeof(char*), 2, f);
  printf("%s", word);

  //pthread_t thread1;
  //pthread_create(&thread1, NULL, TestFunc1, NULL);
  //pthread_join(thread1, NULL);
  
  struct job_queue* jq2 = malloc(sizeof(struct job_queue));
  job_queue_init(jq2, 3);
  char* string = "Hello\n";
  //void* vp;// = (void*)string;
  void* vp;// = *vpp;
  //void** vpp;// = &vp;
  printf("First %s", string);
  job_queue_push(jq2, (void*)string);
  
  job_queue_pop(jq2, &vp);
  //printf("Pop2");
  printf("Last %s\n", (char*)vp);
  job_queue_destroy(jq2);
  free(jq2);
  
  printf("job_queue_destroy() works: %s\n", jqTestDestroy());
  printf("\n");
  printf("job_queue_push() works on a healthy job queue: %s\n", jqTestPushSuccess());
  printf("job_queue_push() increases the queue size: %s\n", jqTestPushSize());
  printf("job_queue_push() increases the back index: %s\n", jqTestPushBack());
  printf("job_queue_push() does not work when job queue is destroyed: %s\n", jqTestPushFail());
  printf("\n");
  printf("job_queue_pop() works on a healthy job queue: %s\n", jqTestPopSuccess());
  printf("job_queue_pop() decreases the queue size: %s\n", jqTestPopSize());
  printf("job_queue_pop() increases the front index: %s\n", jqTestPopFront());
  printf("job_queue_pop() does not work when job queue is destroyed: %s\n", jqTestPopFail());
  printf("\n");
  printf("Front and back both wrap with pushes and pops beyond capacity: %s\n", jqTestFrontBackWrap());
  printf("\n");
  printf("Same queue-instance is available from different threads and pop-block works: %s\n", threadTestBlock());
  return 0;
}


void* TestFunc1() {
  printf("\nI am a thread-test. I succeeded =)\n\n");
}

///////////////////////DESTROY TESTS///////////////////////////

char* jqTestDestroy() {
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  int result = job_queue_destroy(jq1);  
  
  free(jq1);
  
  if(result == 0) return "true =)";
  return "false =(";
}

///////////////////////PUSH TESTS///////////////////////////

char* jqTestPushSuccess() {
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  void* vp;
  int result = job_queue_push(jq1, vp);
  job_queue_pop(jq1, &vp);
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == 0) return "true =)";
  return "false =(";
}

char* jqTestPushSize(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  void* vp;
  job_queue_push(jq1, vp);
  int result = jq1->size;
  job_queue_pop(jq1, &vp);
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == 1) return "true =)";
  return "false =(";
}

char* jqTestPushBack(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  int initBack = jq1->back;
  void* vp;
  job_queue_push(jq1, vp);
  int result = jq1->back;
  job_queue_pop(jq1, &vp);
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result - initBack == 1) return "true =)";
  return "false =(";
}

char* jqTestPushFail() {
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  job_queue_destroy(jq1);
  void* vp;
  int result = job_queue_push(jq1, vp);
  free(jq1);
  
  if(result == -1) return "true =)";
  return "false =(";
}

///////////////////////POP TESTS///////////////////////////

char* jqTestPopSuccess(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  void* vp;
  job_queue_push(jq1, vp);
  int result = job_queue_pop(jq1, &vp);
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == 0) return "true =)";
  return "false =(";
}

char* jqTestPopSize(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  void* vp;
  job_queue_push(jq1, vp);
  int result = jq1->size;
  job_queue_pop(jq1, &vp);
  result -= jq1->size;
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result == 1) return "true =)";
  return "false =(";
}

char* jqTestPopFront(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  int initFront = jq1->front;
  void* vp;
  job_queue_push(jq1, vp);
  job_queue_pop(jq1, &vp);
  int result = jq1->front;
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(result - initFront == 1) return "true =)";
  return "false =(";
}

char* jqTestPopFail(){
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 3);
  
  job_queue_destroy(jq1);
  void* vp;
  int result = job_queue_pop(jq1, vp);
  
  free(jq1);
  
  if(result == -1) return "true =)";
  return "false =(";
}

///////////////////////COMBO TESTS///////////////////////////

char* jqTestFrontBackWrap() {
  struct job_queue* jq1 = malloc(sizeof(struct job_queue));
  job_queue_init(jq1, 2);
  
  void* vp;
  job_queue_push(jq1, vp); // Back turns to 1
  job_queue_pop(jq1, &vp); // Front turns to 1
  job_queue_push(jq1, vp); // Back turns to 0
  job_queue_pop(jq1, &vp); // Front turns to 0
  int resultA = jq1->front;
  int resultB = jq1->back;
  
  job_queue_destroy(jq1);
  free(jq1);
  
  if(resultA ==0 && resultB == 0) return "true =)";
  return "false =(";
}

