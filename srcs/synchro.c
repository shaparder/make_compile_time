#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


int test_set(volatile int *lock, int lock_val)
{
  //int *add_lock_val = &lock_val;
  asm volatile(
      "xchgl %0, %1;"
      : "+m"(*lock), "+q"(lock_val) //output
  );
  return lock_val;
}

//lock until parameter is atomicly set
void lock_ts(volatile int *lock)
{
  while (test_set(lock, 1))
  {
  }
}

//unlock atomicly the parameter
void unlock_ts(volatile int *lock)
{
  test_set(lock, 0);
}

void lock_tts(volatile int *lock)
{
  while (test_set(lock, 1))
  {
    while (*lock)
    {
    }
  }
}

typedef struct sem {
  volatile int nb;
  int max;
  volatile int mutex;
} semaphore_t;

void seminit(semaphore_t* new_sem, volatile int nb) {
  if (nb < 0) {
    perror("arg cannot be negative");
  }
  new_sem->nb = nb;
  new_sem->mutex = 0;
  new_sem->max = nb;
}
void semwait(semaphore_t* sem) {
  if (sem->nb > 0) {
    sem->nb = sem->nb-1;
  }
  else {
    lock_ts(&(sem->mutex));
  }
}

void sempost(semaphore_t* sem) {
  if(sem->nb == 0) {
    unlock_ts(&(sem->mutex));
  }
  sem->nb = sem->nb+1;
}

