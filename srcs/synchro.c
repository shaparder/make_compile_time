#include <stdlib.h>
#include <stdio.h>

int test_set(volatile int *lock, int lock_val) {
  //int *add_lock_val = &lock_val;
  asm volatile (
          "xchgl %0, %1;"
          :"+m" (*lock), "+q" (lock_val) //output
  );
  return lock_val;
}

//lock until parameter is atomicly set
void lock_ts(volatile int *lock) {
  while (test_set(lock, 1)) {}
}

//unlock atomicly the parameter
void unlock_ts(volatile int *lock) {
  test_set(lock, 0);
}

void lock_tts(volatile int* lock) {
  while(test_set(lock, 1)) {
    while (*lock) {

    }
  }
}

struct sem
{
  int count;
  volatile int* lock;
};

//init sem at initial value
struct sem* seminit(int initial_value) {
  struct sem* ret = (struct sem*)malloc(sizeof(struct sem));
  ret->count = initial_value;
  ret->lock = (int *)malloc(sizeof(int));
  *ret->lock = 0;


  return ret;
}

//wait until running state available
void semwait(struct sem* sem) {
  if (sem->count > 0) {
    lock_ts(sem->lock);
    sem->count = (sem->count) - 1;
    unlock_ts(sem->lock);
  } else {
    semwait(sem);
  }
}

//increment available spot
void sempost(struct sem* sem) {
    lock_ts(sem->lock);
    sem->count = (sem->count) + 1;
    unlock_ts(sem->lock);
}

//destroy sem
void semdestroy(struct sem* sem) {
  free((void*)sem->lock);
  free(sem);
}
