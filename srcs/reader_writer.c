#include <stdio.h>
#include <zconf.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>


pthread_mutex_t mutex_readcount;
pthread_mutex_t mutex_writecount;
pthread_mutex_t z; //un seul reader en attente

sem_t write_sem;
sem_t read_sem;

int readcount = 0;
int writecount = 0;
volatile int write_loop = 640;
volatile int read_loop = 2560;

void writer() {

    while(write_loop > 0) {

        pthread_mutex_lock(&mutex_writecount);
		write_loop-=1;
        writecount+=1;
        if (writecount == 1) {
            sem_wait(&read_sem);
        }
		pthread_mutex_unlock(&mutex_writecount);

		sem_wait(&write_sem);
		//écriture des données
		while(rand() > RAND_MAX/10000);
		sem_post(&write_sem);

		pthread_mutex_lock(&mutex_writecount);
		writecount-=1;
		if(writecount == 0) {
			sem_post(&read_sem);
		}
		pthread_mutex_unlock(&mutex_writecount);
    }
}

void reader() {

    while(read_loop > 0) {

        pthread_mutex_lock(&z);  //un seul reader sur read_sem
		sem_wait(&read_sem);

		pthread_mutex_lock(&mutex_readcount);
		read_loop-=1;
        readcount+=1;
        if (readcount == 1) {
            sem_wait(&write_sem);
        }
		pthread_mutex_unlock(&mutex_readcount);

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
}

int main(int argc, char *argv[]) {

	struct timeval temps_avant, temps_apres;
    gettimeofday (&temps_avant, NULL);

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
		err=pthread_create(&(write_threads[i]),NULL,(void *)&writer,NULL);
		if(err!=0) {
			perror("pthread_create");
		}
	}

	//crétion des threads reader
	for(int i=0; i < n_read; i++) {
		err=pthread_create(&(read_threads[i]),NULL,(void *)&reader,NULL);
		if(err!=0) {
			perror("pthread_create");
		}
	}

	//join thread writer
	for(int i=0; i < n_write; i++) {
		err=pthread_join(write_threads[i],NULL);
		if(err!=0) {
			perror("pthread_create");
		}
	}

	//join thread reader
	for(int i=0; i < n_read; i++) {
		err=pthread_join(read_threads[i],NULL);
		if(err!=0) {
			perror("pthread_create");
		}
	}

	sem_destroy(&write_sem);
	sem_destroy(&read_sem);

	gettimeofday (&temps_apres, NULL);
 	printf("temps en us: %ld us\n", ((temps_apres.tv_sec - temps_avant.tv_sec) * 1000000 + temps_apres.tv_usec) - temps_avant.tv_usec);
}
