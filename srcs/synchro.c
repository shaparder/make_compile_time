int test_set(volatile int *lock, int lock_val) {
  //int *add_lock_val = &lock_val;
  asm volatile (
          "xchgl %0, %1;"
          :"+m" (*lock), "+q" (lock_val) //output
  );
  return lock_val;
}

//lock until paramameter is atomicly set
void lock_ts(int *lock) {
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

void seminit(volatile int* sem, int initial_value) {
  sem = (volatile int*)malloc(sizeof(volatile int));
  *sem = initial_value;
}

void semwait(volatile int* sem) {
  if (*sem > 0) {
    *sem--;
  } else {
    semwait(sem);
  }
}

void sempost(volatile int* sem) {
  *sem++;
}

void semdestroy(volatile int* sem) {
  free(sem);
}
