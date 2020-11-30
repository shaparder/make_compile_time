#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "semaphore.h"

int loop = 6400;


void seminit(int nb);
int getlock();
void semwait();
void sempost();

void *Test() {

	while(loop > 0 && getlock() == 0) {
		semwait();
		loop--;
		while(rand() > RAND_MAX/10000);
		sempost();
	}
}

int main(int argc, char const *argv[]) {
	seminit(1);
	int err;
	int n_threads = strtol(argv[1], NULL, 10);
	pthread_t thrds[n_threads];
	struct timeval tvalBefore, tvalAfter;

    gettimeofday (&tvalBefore, NULL);

	for (int i = 0; i < n_threads; i++) {
		err = pthread_create(&thrds[i], NULL, Test, NULL);
		if(err!=0) {
      		perror("pthread_create");
    	}
	}

	for (int i = 0; i < n_threads; i++) {
		err = pthread_join(thrds[i], NULL);
				if(err!=0) {
      		perror("pthread_join");
    	}
	}

	gettimeofday (&tvalAfter, NULL);
	printf("Time in microseconds: %ld ms\n",
		((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000L
		+tvalAfter.tv_usec) - tvalBefore.tv_usec
		);
	printf("\n%d\n", loop);

}
