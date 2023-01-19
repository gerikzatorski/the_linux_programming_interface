/* exercise_31_01.c */

/*********************************************************************
Implement a function, one_time_init(control, init), that performs the
equivalent of pthread_once(). The control argument should be a pointer
to a statically allocated structure containing a Boolean variable and
a mutex. The Boolean variable indicates whether the function init has
already been called, and the mutex controls access to that variable.
To keep the implementation simple, you can ignore possibilities such
as init() failing or being canceled when first called from a thread
(i.e., it is not necessary to devise a scheme whereby, if such an
event occurs, the next thread that calls one_time_init() reattempts
the call to init()).
*********************************************************************/

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

#define NUM_THREADS 1000

struct one_time_init_t
{
  bool been_called;
  pthread_mutex_t mtx;
};

int one_time_init(struct one_time_init_t *control, void (*init)(void))
{
  pthread_mutex_lock(&(control->mtx));
  if (!control->been_called)
    init();
  control->been_called = true;
  pthread_mutex_unlock(&(control->mtx));
}

void print_msg()
{
  printf("Hello from thread %lu!\n", pthread_self());
}

void *thread_func(void *arg)
{
  one_time_init(arg, print_msg);
}

int main(int argc, char *argv[])
{
  int i, s;
  pthread_t threads[NUM_THREADS];

  struct one_time_init_t c = {
    .been_called = false,
    .mtx = PTHREAD_MUTEX_INITIALIZER };

  for (i = 0; i < NUM_THREADS; i++) {
    s = pthread_create(&threads[i], NULL, thread_func, &c);
    if (s != 0)
      errExitEN(s, "pthread_create");
  }

  for (i = 0; i < NUM_THREADS; i++) {
    s = pthread_join(threads[i], NULL);
    if (s != 0)
      errExitEN(s, "pthread_join");
  }
  
  exit(EXIT_SUCCESS);
}

