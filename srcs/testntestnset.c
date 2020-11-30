#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define ITER 6400

__thread int count = 0;

void lock();
void unlock();
extern int locked;

//test and test and set function using lock and unlock
void *TestTestSet(void *param){
  int iter = (int) param;

  //iterate the right amount of time for any thread and only if locked isn't set
  while (count < iter && locked == 0)
  {
    lock();
    count++;
    while (rand() > RAND_MAX / 10000);
    unlock();
  }

  printf("thread count=%d\n", count);
  free(param);
  return ;
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
    int *arg = (int *)malloc(sizeof(*arg));
    *arg = (i < n_threads - 1) ? (ITER/n_threads) : (ITER - (ITER/n_threads)*i);
    err = pthread_create(&thrds[i], NULL, TestTestSet, arg);
    if (err != 0) perror("pthread_create");
  }

  for (int i = 0; i < n_threads; i++)
  {
    err = pthread_join(thrds[i], NULL);
    if (err != 0) perror("pthread_join");
  }

  return 0;
}
