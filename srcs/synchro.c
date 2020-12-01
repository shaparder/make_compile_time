int test_set(volatile int *lock, int lock_val) {
  //int *add_lock_val = &lock_val;
  asm volatile (
          "xchgl %0, %1;"
          :"+m" (*lock), "+q" (lock_val) //output
  );
  return lock_val;
}

void lock_ts(int *lock) {
  while (test_set(lock, 1)) {}
}

void unlock_ts(volatile int *lock) {
  test_set(lock, 0);
}

void lock_tts(volatile int* lock) {
  while(test_set(lock, 1)) {
    while (*lock) {

    }
  }
}

void seminit(int nb) {
  if (nb < 1)
  {
    perror("Argument must be greater than 0");
  }
  semcount = nb;
}

void semwait() {
  if (semcount > 0) {
    lock_ts();
    semcount--;
  } else {
    semwait();
  }
}

void sempost() {
  unlock_ts();
  semcount++;
}
