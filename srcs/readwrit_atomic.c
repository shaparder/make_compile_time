#include <stdio.h>
#include <zconf.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

#define WITR 640
#define RITR 2560

int write_mutex;
int read_mutex;
int z_mutex;

typedef struct primitive_sem
{
  int val;
  int* lock;
}prim_sem;

void lock_tts(int *lock);
void unlock_ts(int *lock);
int prim_sem_init(prim_sem **s, int start_val);
int prim_sem_destroy(prim_sem *sem);
int prim_sem_wait(prim_sem *sem);
int prim_sem_post(prim_sem *sem);

prim_sem* writesem;
prim_sem* readsem;

volatile int readcount = 0;
volatile int writecount = 0;
__thread int W_iter = 0;
__thread int R_iter = 0;

void *Writer(void *param) {

  int iter = *((int *) param);

  while(W_iter < iter)
  {
    lock_tts(&write_mutex);

    W_iter++;
    writecount++;
    if (writecount == 1) prim_sem_wait(readsem);

    unlock_ts(&write_mutex);

    prim_sem_wait(writesem);
    //écriture des données
    while(rand() > RAND_MAX/10000);
    prim_sem_post(writesem);

    lock_tts(&write_mutex);

    writecount--;
    if(writecount == 0) prim_sem_post(readsem);

    unlock_ts(&write_mutex);
  }

  free(param);
  return NULL;
}

void *Reader(void *param) {
  int iter = *((int *) param);

  while(R_iter < iter)
  {
    lock_tts(&z_mutex);  //un seul reader sur read_sem
    prim_sem_wait(readsem);
    lock_tts(&read_mutex);

    R_iter++;
    readcount+=1;
    if (readcount == 1) prim_sem_wait(writesem);

    unlock_ts(&read_mutex);

    prim_sem_post(readsem); //libération du prochain reader
    unlock_ts(&z_mutex);

    //lecture des données
    while(rand() > RAND_MAX/10000);
    lock_tts(&read_mutex);
    readcount-=1;
    if(readcount == 0) {
      prim_sem_post(writesem);
    }
    unlock_ts(&read_mutex);
  }
  free(param);
  return NULL;
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    perror("2 arguments required");
  }
  int n_write = strtol(argv[1], NULL, 10);
  int n_read = strtol(argv[2], NULL, 10);

  prim_sem_init(&writesem, 1);
  prim_sem_init(&readsem, 1);
  write_mutex = 0;
  read_mutex = 0;
  z_mutex = 0;

  pthread_t write_threads[n_write];
  pthread_t read_threads[n_read];
  int err;

  //création des threads writer
  for(int i=0; i < n_write; i++) {
    int *arg = (int *)malloc(sizeof(*arg));
    *arg = (i < n_write - 1) ? (WITR/n_write) : (WITR - (WITR/n_write)*i);
    err = pthread_create(& write_threads[i], NULL, Writer, arg);
    if (err != 0) {
      perror("pthread_create");
    }
  }

  //crétion des threads reader
  for(int i=0; i < n_read; i++) {
    int *arg = (int *)malloc(sizeof(*arg));
    *arg = (i < n_read - 1) ? (RITR/n_read) : (RITR - (RITR/n_read)*i);
    err=pthread_create(&read_threads[i], NULL, Reader, arg);
    if(err!=0) {
      perror("pthread_create");
    }
  }

  //join thread writer
  for(int i=0; i < n_write; i++) {
    err=pthread_join(write_threads[i], NULL);
    if(err!=0) {
      perror("pthread_create");
    }
  }

  //join thread reader
  for(int i=0; i < n_read; i++) {
    err=pthread_join(read_threads[i], NULL);
    if(err!=0) {
      perror("pthread_create");
    }
  }

  prim_sem_destroy(writesem);
  prim_sem_destroy(readsem);

  return 0;
}
