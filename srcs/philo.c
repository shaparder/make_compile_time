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

__thread int count = 0;

struct philo
{
  int id;
  int nphilo;
  int nchop;
  pthread_mutex_t *chop;
};

bool isNumber(const char number[])
{
    int i = 0;
    if (number[0] == '-') i = 1;
    for (; number[i] != 0; i++)
    {
        if (!isdigit(number[i]))
            return false;
    }
    return true;
}

//check command line arguments
void args_check(int argc, const char *argv[])
{
  if (argc != 2 || !isNumber(argv[1]))
  {
    printf("wrong arguments\n");
    printf("usage: ./philo N_PHILOSOPHERS\n");
    exit(1);
  }
  return;
}

struct philo* new_philo(int id, int nphilo, int nchop, pthread_mutex_t *chop)
{
  struct philo* ret = (struct philo*)malloc(sizeof(struct philo));
  ret->id = id;
  ret->chop = chop;
  ret->nphilo = nphilo;
  ret->nchop = nchop;

  return ret;
}

void eat(int id)
{
  //printf("philo %d is eating\n", id);
  (void)id;
  return ;
}

void *Philosothread(void *param)
{
  struct philo* p = (struct philo *) param;
  int id = p->id;
  int left = id;
  int right = (left + 1) % p->nchop;
  pthread_mutex_t* chop = p->chop;

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
    eat(id);
    pthread_mutex_unlock(&chop[left]);
    pthread_mutex_unlock(&chop[right]);
  }
  free(param);
  return NULL;
}

int main(int argc, char const *argv[]) {

  //security check for args_check
  args_check(argc, argv);

  int nphilo = atoi(argv[1]);
  pthread_t phil_threads[nphilo];
  int nchop = (nphilo == 1) ? 2 : nphilo;
  pthread_mutex_t* chop = (pthread_mutex_t *)malloc(nchop * sizeof(pthread_mutex_t));

  // init mutexes
  for (int i = 0; i < nchop; i++) {
    pthread_mutex_init(&chop[i], NULL);
  }

  // create threads
  for (int i = 0; i < nphilo; i++) {
    struct philo* p = new_philo(i, nphilo, nchop, chop);
    pthread_create(&phil_threads[i], NULL, Philosothread, p);
  }

  // join all threads
  void *ret = NULL;
  for (int i = 0; i < nphilo; i++) pthread_join(phil_threads[i], &ret);

  // free struct
  free(chop);

  return 0;
}
