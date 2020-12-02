#include <stdlib.h>
#include <stdio.h>

//test set function, xchgl values
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
  while (test_set(lock, 1)) {}
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
    while (*lock) {}
  }
}

//struct for any semaphore
typedef struct semaphore_primitive
{
  int value;
  int* lock;
} sem_p;

//init sem struct and lock
void sem_p_init(sem_p **sem, int initial_value)
{
  *sem = (struct semaphore_primitive *)malloc(sizeof(struct semaphore_primitive));
  (**sem).lock = (int *)malloc(sizeof(int));
  *((**sem).lock) = 0;
  (*sem)->value= initial_value;
  return ;
}

//free sem struct and lock
void sem_p_destroy(sem_p *sem)
{
  free(&(sem->lock));
  free(sem);
  return ;
}

//spinlock function
void sem_p_wait(sem_p *sem)
{
  int test = 0;

  while (!test)
  {
    lock_ts(sem->lock);
    if (sem->value > 0)
    {
      sem->value = sem->value - 1;
      test = 1;
      unlock_ts(sem->lock);
    } else {
      unlock_ts(sem->lock);
    }
  }
  return ;
}

//takes sem structure as an argument
//increments the val of sem
void prim_sem_post(sem_p *sem)
{
  lock_ts(sem->lock);
  sem->value = sem->value + 1;
  unlock_ts(sem->lock);
  return ;
}
