#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define ITER 6400

// prototyping lock functions
void lock_ts(int *lock);
void unlock_ts(int *lock);

//init lock variables
int lock = 0;
int c = 0;

//test and set function using lock and unlock
void *ts_thread(void *param){
  int iter = *((int *) param);
  int count = 0;

  //iterate the right amount of time for any thread
  while (count < iter)
  {
    lock_ts(&lock); //lock the lock
    count++;
    c++;
    while (rand() > RAND_MAX / 10000); //simulating time consumption
    unlock_ts(&lock); //unlock the lock
  }

  //printf("thread count=%d\n", count);
  //free parameter of thread
  free(param);
  return NULL;
}

int main(int argc, char const *argv[])
{
  if (argc != 2) {
    perror("1 argument required");
  }
  int n_threads = strtol(argv[1], NULL, 10);
  pthread_t thrds[n_threads];

  int err;
  for (int i = 0; i < n_threads; i++)
  {
    //allocate parameter that holds iteration number
    int *arg = (int *)malloc(sizeof(*arg));
    //compute number of iterationnfor each thread
    *arg = (i < n_threads - 1) ? (ITER/n_threads) : (ITER - (ITER/n_threads)*i);
    //create thread
    err = pthread_create(&thrds[i], NULL, ts_thread, arg);
    if (err != 0) perror("pthread_create");
  }

  //join all threads
  for (int i = 0; i < n_threads; i++)
  {
    err = pthread_join(thrds[i], NULL);
    if (err != 0) perror("pthread_join");
  }

  //printf("%d\n", c);

  return 0;
}
