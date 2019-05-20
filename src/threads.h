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

struct queue *init_queue();
