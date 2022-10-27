
#include <string.h>
#include <stdbool.h>

#include "job_queue.h"

void* TestFunc1();

char* jqTestDestroy();

char* jqTestPushSuccess();
char* jqTestPushSize();
char* jqTestPushBack();
char* jqTestPushFail();

char* jqTestPopSuccess();
char* jqTestPopSize();
char* jqTestPopFront();
char* jqTestPopFail();

char* jqTestFrontBackWrap();
