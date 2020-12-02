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


pthread_mutex_t mutex_readcount;
pthread_mutex_t mutex_writecount;
pthread_mutex_t z; //un seul reader en attente

sem_t write_sem;
sem_t read_sem;

volatile int readcount = 0;
volatile int writecount = 0;
__thread int W_iter = 0;
__thread int R_iter = 0;

//writer thread function
void *Writer(void *param) {

  int iter = *((int *) param);

  while(W_iter < iter)
  {
    pthread_mutex_lock(&mutex_writecount);

    W_iter++;
    writecount++;
    if (writecount == 1) sem_wait(&read_sem);

    pthread_mutex_unlock(&mutex_writecount);

    sem_wait(&write_sem);
    //écriture des données
    while(rand() > RAND_MAX/10000);
    sem_post(&write_sem);

    pthread_mutex_lock(&mutex_writecount);

    writecount--;
    if(writecount == 0) sem_post(&read_sem);

    pthread_mutex_unlock(&mutex_writecount);
  }

  free(param);
  return NULL;
}

//reader thread function
void *Reader(void *param) {
  int iter = *((int *) param);

  while(R_iter < iter)
  {
    pthread_mutex_lock(&z);  //un seul reader sur read_sem
    sem_wait(&read_sem);
    pthread_mutex_lock(&mutex_readcount); //lock le mutex read

    R_iter++;
    readcount+=1;
    if (readcount == 1) sem_wait(&write_sem);

    pthread_mutex_unlock(&mutex_readcount); //unlock mutex read

    sem_post(&read_sem); //libération du prochain reader
    pthread_mutex_unlock(&z);

    //lecture des données
    while(rand() > RAND_MAX/10000);
    pthread_mutex_lock(&mutex_readcount);
    readcount-=1;
    if(readcount == 0) {
      sem_post(&write_sem);
    }
    pthread_mutex_unlock(&mutex_readcount);
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

  sem_init(&write_sem, 0, 1);
  sem_init(&read_sem, 0, 1);

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

  //exit semaphores
  sem_destroy(&write_sem);
  sem_destroy(&read_sem);

  return 0;
}
