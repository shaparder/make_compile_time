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

//count value unique to each thread
__thread int count = 0;

//struct to hold mutex and thread related data
struct philo
{
  int id;
  int nphilo;
  int nchop;
  pthread_mutex_t *chop;
};

//check command line arguments correctness
void args_check(int argc, const char *argv[])
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
struct philo* new_philo(int id, int nphilo, int nchop, pthread_mutex_t *chop)
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
  pthread_mutex_t* chop = p->chop;

  //iteration loop, locking mutexes in ascending order
  while (count < ITER)
  {
    count++;
    if (left < right)
    {
      pthread_mutex_lock(&chop[left]);
      pthread_mutex_lock(&chop[right]);
    } else {
      pthread_mutex_lock(&chop[right]);
      pthread_mutex_lock(&chop[left]);
    }
    //thread action
    eat(id);
    //unlocking mutexes
    pthread_mutex_unlock(&chop[left]);
    pthread_mutex_unlock(&chop[right]);
  }
  //free struct philo
  free(param);
  return NULL;
}

int main(int argc, char const *argv[]) {

  //security check for args_check
  args_check(argc, argv);

  //get number of threads from argument
  int nphilo = atoi(argv[1]);
  //init mutexes and threads
  pthread_t phil_threads[nphilo];
  //avoid infinite lock if only 1 philosopher
  int nchop = (nphilo == 1) ? 2 : nphilo;
  pthread_mutex_t* chop = (pthread_mutex_t *)malloc(nchop * sizeof(pthread_mutex_t));

  //init mutexes
  for (int i = 0; i < nchop; i++) {
    pthread_mutex_init(&chop[i], NULL);
  }

  //create threads
  for (int i = 0; i < nphilo; i++) {
    //create struct to pass as argument for thread
    struct philo* p = new_philo(i, nphilo, nchop, chop);
    //create thread
    pthread_create(&phil_threads[i], NULL, Philosothread, p);
  }

  // join all threads
  void *ret = NULL;
  for (int i = 0; i < nphilo; i++) pthread_join(phil_threads[i], &ret);

  // free struct
  free(chop);

  return 0;
}
