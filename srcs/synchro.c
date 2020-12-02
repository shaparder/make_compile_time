#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//xchgl fucntion
int test_set(int *lock, int lock_val)
{
  asm volatile(
      "xchgl %0, %1;"
      : "+m"(*lock), "+q"(lock_val)
  );
  return lock_val;
}

//lock until parameter is atomicly set
void lock_ts(int *lock)
{
  while (test_set(lock, 1)){}
}

//unlock atomicly the parameter
void unlock_ts(int *lock)
{
  test_set(lock, 0);
}

void lock_tts(int *lock)
{
  while (test_set(lock, 1))
  {
    while (*lock){}
  }
}


typedef struct semaphore
{
  int value;
  int* lock;
} sem_p;

//init sem struct and lock
int sem_p_init(sem_p **sem, int initial_value)
{
  *sem = (struct semaphore *)malloc(sizeof(struct semaphore));
  if (*sem == NULL) return -1;
  (**sem).lock = (int *)malloc(sizeof(int));
  *((**sem).lock) = 0;
  (*sem)->value = initial_value;
  return 0;
}

//free sem struct
int sem_p_destroy(sem_p *sem)
{
  free(sem);
  return 0;
}

//spinlock function semaphore
int sem_p_wait(sem_p *sem)
{
  bool cond = false;

  while (!cond)
  {
    lock_ts(sem->lock);

    if (sem->value > 0)
    {
      sem->value = sem->value - 1;
      cond = true;
      unlock_ts(sem->lock);
    }
    else
    {
      unlock_ts(sem->lock);
    }
  }
  return 0;
}

int sem_p_post(sem_p *sem)
{
  lock_ts(sem->lock);
  sem->value = sem->value + 1;
  unlock_ts(sem->lock);
  return 0;
}
