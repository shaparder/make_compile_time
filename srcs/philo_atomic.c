#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define ITER 1000000

//struct to hold mutex and thread related data
struct philo
{
  int id;
  int nphilo;
  int nchop;
  int *chop;
};

//prototyping primitive lock functions
void lock_tts(int *lock);
void unlock_ts(int *lock);

//check command line arguments
void args_check(int argc)
{
  if (argc != 2)
  {
    printf("wrong arguments\n");
    printf("usage: ./philo N_PHILOSOPHERS\n");
    exit(1);
  }
  return;
}

//struct constructor, allocate struct pointer
struct philo* new_philo(int id, int nphilo, int nchop, int *chop)
{
  struct philo* ret = (struct philo*)malloc(sizeof(struct philo));
  ret->id = id;
  ret->chop = chop;
  ret->nphilo = nphilo;
  ret->nchop = nchop;

  return ret;
}

//philospher's action
void eat(int id)
{
  //printf("philo %d is eating\n", id);
  (void)id;
  return ;
}

//thread function for philosophers
void *Philosothread(void *param)
{
  struct philo* p = (struct philo *) param;
  int id = p->id;
  int left = id;
  int right = (left + 1) % p->nchop;
  int* chop = p->chop;

  //iteration loop, locking mutexes in ascending order
  int count = 0;
  while (count < ITER)
  {
    count++;
    if (left < right)
    {
      lock_tts(&chop[left]);
      lock_tts(&chop[right]);
    } else {
      lock_tts(&chop[right]);
      lock_tts(&chop[left]);
    }
    eat(id);
    unlock_ts(&chop[left]);
    unlock_ts(&chop[right]);
  }
  //free struct
  free(param);
  return NULL;
}

int main(int argc, char const *argv[]) {

  //security check for args_check
  args_check(argc);

  int nphilo = atoi(argv[1]);
  pthread_t phil_threads[nphilo];
  int nchop = (nphilo == 1) ? 2 : nphilo;
  int* chop = (int *)malloc(nchop * sizeof(int));

  // init mutexes
  for (int i = 0; i < nchop; i++) {
    chop[i] = 0;
  }

  // create threads
  for (int i = 0; i < nphilo; i++) {
    //create struct to hold arguments for each thread
    struct philo* p = new_philo(i, nphilo, nchop, chop);
    pthread_create(&phil_threads[i], NULL, Philosothread, p);
  }

  // join all threads
  void *ret = NULL;
  for (int i = 0; i < nphilo; i++) pthread_join(phil_threads[i], &ret);

  // free struct
  free((int*)chop);

  return 0;
}
