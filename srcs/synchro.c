int test_set(volatile int *lock, int lock_val) {
  int *add_lock_val = &lock_val;
  asm volatile (
          "xhcgl %0, %1"
          :"+m" (*lock), "+q" (lock_val) //output
  );
  return lock_val;
}

int lock_ts(int *lock) {
  while (test_set(lock, 1)) {}
}

void unlock_ts(volatile int *lock) {
  test_set(lock, 0);
}
