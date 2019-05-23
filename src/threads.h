#ifndef THREADS_H_
#define THREADS_H_

#include<stdlib.h>
#include<pthread,h>



struct toExec {
  void (*fun) (void *args);
  void *args;
};

struct linkedList {
  struct toExec *elt;
  struct linkedList *next;
};

struct queue {
  pthread_mutex_t lock;
  size_t size;
  struct linkedList *head;
  struct linkedList *tail;
};

struct queue *queue_init();
struct toExec *queue_pop(struct queue *q);
size_t queue_lenght(struct queue *q); //used to test not Empty statement
void queue_push(struct queue *q, struct toExec *e, int alreadyLocked); //allows to push ultiple items in a row

void threadPool_init(size_t nbThreads);
void threadPool_destroy();
extern struct queue *threadPool_queue;

#endif
