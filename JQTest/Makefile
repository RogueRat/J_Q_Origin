CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=gnu99 -g -pthread
DEPS = jqTest.h jqThreadTest.h job_queue.h
OBJ = jqTest.o jqThreadTest.o job_queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
Jqtest: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
