#include <stdlib.h>
#include <stdbool.h>

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

/*

struct sem
{
  volatile int count;
  volatile int lock;
};

//init sem at initial value
struct sem *seminit(int initial_value)
{
  struct sem *ret = (struct sem *)malloc(sizeof(struct sem));
  ret->count = initial_value;
  ret->lock = 0;

  return ret;
}

//wait until running state available
void semwait(struct sem *sem)
{
  if (sem->count > 0)
  {
    lock_ts(&sem->lock);
    sem->count = (sem->count) - 1;
    unlock_ts(&sem->lock);
  }
  else
  {
    semwait(sem);
  }
}

//increment available spot
void sempost(struct sem *sem)
{
  lock_ts(&sem->lock);
  sem->count = (sem->count) + 1;
  unlock_ts(&sem->lock);
}

//destroy sem
void semdestroy(struct sem *sem)
{
  free(sem);
}

*/

typedef struct primitive_sem
{
  int val;
  volatile int* lock;
}prim_sem;

//Initialize the sem structure and his lock
int prim_sem_init(prim_sem **s, int start_val)
{
  *s = (struct primitive_sem *)malloc(sizeof(struct primitive_sem));
  if (*s == NULL) return -1;
  (**s).lock = (int *)malloc(sizeof(int));
  *((**s).lock) = 0;
  (*s)->val = start_val;
  return 0;
}

//free the sem struct
int prim_sem_destroy(prim_sem *sem)
{
  free((int *)(sem->lock));
  free(sem);
  return 0;
}

//takes sem structure as an argument
//puts the threads in spinlock if the val is not >0 and decrements the val of sem once out of the spinlock
int prim_sem_wait(prim_sem *sem)
{
  bool cond = false;

  while (!cond)
  {
    lock_tts(sem->lock);

    if (sem->val > 0)
    {
      //int val = *(sem->lock);
      //printf("before name: %s val: %d and lock: %d\n", name, sem->val, *(sem->lock));
      //printf("%d\n", val);
      sem->val = sem->val - 1;
      //printf("after name: %s val: %d and lock: %d\n", name, sem->val, *(sem->lock));
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

//takes sem structure as an argument
//increments the val of sem
int prim_sem_post(prim_sem *sem)
{
  lock_tts(sem->lock);
  sem->val = sem->val + 1;
  unlock_ts(sem->lock);
  return 0;
}
