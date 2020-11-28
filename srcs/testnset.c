#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

int loop = 6400;
int locked = 0;
int count = 0;


void lock() {

	asm("enter:\n"
			"movl $1, %%eax;\n"    //on place 1 dans le registre eax
			"xchgl %%eax, %0;\n"   //on échange les valeurs de lock et eax
			"testl %%eax, %%eax;\n"//met le flag ZF = 1 si lock valait 0
			"jnz enter;\n"         //si ZF != 1, aors lock valait 1, et le trhread reste blocké
 						           //dans le cas contraire, lock valait 0, et le thread peut entrer en section critique
		: "=r" (locked)
		: "r" (locked)
		: "%eax"); 
}

void unlock() {

	asm("movl $0, %%ebx;\n"        //on met 0 dans eax
		"xchgl %%ebx, %0;\n"       //on échange la valeur de eax et lock
						           //lock vaut donc 0, ce qui indique que le thread a terminé sa section critique
		: "=r" (locked)
		: "r" (locked)
		: "%ebx"); 
}

void *TestnSet() {

	while(loop > 0 && locked == 0) {
		lock();
		loop--;
		for(int i = 0; i < 1000; i++);
		//while(rand() > RAND_MAX/10000);
		unlock();
	}
}


int main(int argc, char const *argv[]) {
	int err;
	int n_threads = strtol(argv[1], NULL, 10);
	pthread_t thrds[n_threads];
	struct timeval tvalBefore, tvalAfter; 

    gettimeofday (&tvalBefore, NULL);

	for (int i = 0; i < n_threads; i++) {
		err = pthread_create(&thrds[i], NULL, TestnSet, NULL);
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
