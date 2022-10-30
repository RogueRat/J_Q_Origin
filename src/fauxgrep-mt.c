// Setting _DEFAULT_SOURCE is necessary to activate visibility of
// certain header file contents on GNU/Linux systems.
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>

// err.h contains various nonstandard BSD extensions, but they are
// very handy.
#include <err.h>

#include <pthread.h>
#include <time.h>

#include "job_queue.h"


char const* needle; 

int fauxgrep_file(char const *needle_c, char const *path) {
  FILE *f = fopen(path, "r");
  
  if (f == NULL) {
    warn("failed to open %s", path);
    return -1;
  }

  
  char *line = NULL;
  size_t linelen = 0;
  int lineno = 1;
  
  while (getline(&line, &linelen, f) != -1) {  
    if (strstr(line, needle_c) != NULL) {
      printf("%s:%d: %s", path, lineno, line);
    }

    lineno++;
  }

  free(line);
  fclose(f);

  return 0;
}

void* worker (void* queue) {
  struct job_queue* the_queue = (struct job_queue*)queue; 
  void* info;
  char* needle_c = malloc(sizeof(char*)*strlen(needle));
  for(int c = 0; c < strlen(needle); c++) {
    needle_c[c] = needle[c];
  }
  while (job_queue_pop(the_queue, &info)==0) {
    fauxgrep_file((char const*)needle_c, (char const*)info);
  }
  free(needle_c);
}

int main(int argc, char * const *argv) { 
  if (argc < 2) {
    err(1, "usage: [-n INT] STRING paths...");
    exit(1);
  }

  time_t start_t;
  start_t = time(NULL);

  int num_threads = 1;
  needle = argv[1];
  char * const *paths = &argv[2];

  if (argc > 3 && strcmp(argv[1], "-n") == 0) {
    // Since atoi() simply returns zero on syntax errors, we cannot
    // distinguish between the user entering a zero, or some
    // non-numeric garbage.  In fact, we cannot even tell whether the
    // given option is suffixed by garbage, i.e. '123foo' returns
    // '123'.  A more robust solution would use strtol(), but its
    // interface is more complicated, so here we are.
    num_threads = atoi(argv[2]);

    if (num_threads < 1) {
      err(1, "invalid thread count: %s", argv[2]);
    }

    needle = argv[3];
    paths = &argv[4];

  } else {
    needle = argv[1];
    paths = &argv[2];
  }

  // Initialise the job queue and some worker threads here.
  struct job_queue* the_queue = malloc(sizeof(struct job_queue));
  assert((job_queue_init(the_queue, 500))==0);
  
  pthread_t tarray [num_threads]; 
  for (int i = 0; i < num_threads; i++) { 
    pthread_create(&tarray[i], NULL, worker, (void*)the_queue);
  }


  // FTS_LOGICAL = follow symbolic links
  // FTS_NOCHDIR = do not change the working directory of the process
  //
  // (These are not particularly important distinctions for our simple
  // uses.)
  int fts_options = FTS_LOGICAL | FTS_NOCHDIR;

  FTS *ftsp;
  if ((ftsp = fts_open(paths, fts_options, NULL)) == NULL) {
    err(1, "fts_open() failed");
    return -1;
  }

  FTSENT *p;
  while ((p = fts_read(ftsp)) != NULL) {
    switch (p->fts_info) {
    case FTS_D:
      break;
    case FTS_F:
      assert(job_queue_push(the_queue, (void*)strdup(p->fts_path))== 0); // Process the file p->fts_path, somehow
      break;
    default:
      break;
    }
  }
  
  fts_close(ftsp);

  //Shut down the job queue and the worker threads here.
  assert((job_queue_destroy(the_queue))==0);
  for (int i = 0; i < num_threads; i++) {
    pthread_join (tarray[i], NULL); 
  }
  free(the_queue); 

  double runtime = (double)(time(NULL) - start_t); 
  printf ("\nRuntime: %f\n", runtime);

  return 0;
}
