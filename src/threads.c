#include "threads.h"
#include <errno.h>

#define NB_THREADS 5
pthread_t[NB_THREADS] threads;
pthread_mutex_t[NB_THREADS] workingThreads;
struct queue *threadPool_queue = NULL;
int destroy = 0;
void *threadPool(void *arg) {
  pthread_mutex_t lock;
  printf("spawned thread %d\n", *((int*)arg));
  while (!destroy) {
    if (pthread_mutex_trylock(lock)) {
      struct toExec* elt = queue_pop(threadPool_queue);
      pthread_mutex_unlock(lock);
      if (elt != NULL) {
        elt->fun(elt->args);
      }
    }
  }
  return NULL;
}

struct queue *queue_init() {
  struct queue *res = calloc(1, sizeof(struct res));
  if (res == NULL)
    return NULL;
  pthread_mutex_init(&(res->lock), NULL);
  return res;
}

struct toExec *queue_pop(struct queue *q) {
  if (q->head == NULL)
    return NULL;
  struct linkedList *list = q->head;
  q->head = list->next;
  if (q->head == NULL)
    q->tail = NULL;
  q->size--;
  struct toExec *res = list->elt;
  free(list);
  return res;
}

/*  pop here                  insert here
 *  head                      tail
 *   e  ->  e ->   e->  e->   e -> NULL
 **/
void queue_push(struct queue *q, struct toExec *e, int alreadyLocked) {
  if (!alreadyLocked)
    pthread_mutex_lock(q->lock);

  struct linkedList* l = calloc(1, sizeof(struct linkedList));
  if (l == NULL)
    return;

  l->elt = e;
  if (q->size == 0) {
    q->head = l;
    q->tail = l;
  } else {
    q->tail->next = l;
    q->tail = l;
  }
  q->size++;

  if (!alreadyLocked)
    pthread_mutex_unlock(q->lock);
}

void threadPool_destroy() {
  while (queue_pop(threadPool_queue)); //elements are supposed to be free in main
  pthread_mutex_destroy(threadPool_queue->lock);
  free(threadPool_queue);
  for (size_t k = 0; k < NB_THREADS; ++k) {
    if (threads[k])
      pthread_join(threads[k], NULL);
  }
}

void threadPool_init() {
  if (threadPool_queue == NULL)
    threadPool_queue = queue_init();
  for (size_t k = 0; k < NB_THREADS; ++k) {
    int errorCode;
    if ((errorCode = pthread_create(threads + k, NULL, threadPool, threads + k))) {
      if (errno == EAGAIN) {
        for (; k < NB_THREADS; ++k)
          threads[k] = 0;
      }
    }
  }
  atexit(threadPool_destroy);
}




