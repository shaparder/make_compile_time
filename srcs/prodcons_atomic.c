#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

//define buffer size and number of iterations prod-cons
#define NBUF 8
#define NITR 1024

// struct to hold semaphores
typedef struct semaphore
{
  int value;
  int* lock;
} sem_p;

typedef struct buffer
{
        int buf[NBUF];           /* shared var */
        int    in;               /* buf[in%BUFF_SIZE] is the first empty slot */
        int    out;              /* buf[out%BUFF_SIZE] is the first full slot */
        sem_p* full;          /* keep track of the number of full spots */
        sem_p* empty;         /* keep track of the number of empty spots */
        int mutex;      /* enforce mutual exclusion to shared data */
} buf_t;

buf_t buf;

void lock_tts(int *lock);
void unlock_ts(int *lock);
int sem_p_init(sem_p **s, int initial_value);
int sem_p_destroy(sem_p *sem);
int sem_p_wait(sem_p *sem);
int sem_p_post(sem_p *sem);

__thread int P_iter = 0;
__thread int C_iter = 0;


//check if char[] is number
bool isNumber(const char number[])
{
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
        i = 1;
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i]))
            return false;
    }
    return true;
}

//check command line arguments
void args_check(int argc, const char *argv[])
{
  if (argc != 3 || !isNumber(argv[1]) || !isNumber(argv[2]))
  {
    printf("wrong arguments\n");
    printf("usage: ./prodcons N_PRODUCERS N_CONSUMERS\n");
    exit(1);
  }
  return ;
}

int random_number()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);

  srand(tp.tv_usec + getpid());
  int result = rand();
  return result;
}

void *Producer(void *param)
{
  int iter = *((int *) param);
  int to_store;

  while (P_iter < iter)
  {
    while(rand() > RAND_MAX/10000);
    //increment iter count
    P_iter++;
    //compute random number
    to_store = random_number();
    //wait for free spot in buffer
    sem_p_wait(buf.empty);
    //if another thread uses the buffer, wait
    lock_tts(&buf.mutex);
    //store item
    buf.buf[buf.in] = to_store;
    buf.in = (buf.in+1)%NBUF;
    //release the buffer
    unlock_ts(&buf.mutex);
    //increment the number of full slots
    sem_p_post(buf.full);
  }
  free(param);
  //printf("P_iter=%d\n", P_iter);
  return NULL;
}

void *Consumer(void *param)
{
  int iter = *((int *) param);
  int item;

  while (C_iter < iter)
  {
    while(rand() > RAND_MAX/10000);

    sem_p_wait(buf.full);
    lock_tts(&buf.mutex);
    //increment iter
    C_iter++;
    //get line
    item = buf.buf[buf.out];
    buf.out = (buf.out+1)%NBUF;
    //release buffer
    unlock_ts(&buf.mutex);
    sem_p_post(buf.empty);
    //use the variable
    //printf("%d\n", item);
    (void)item;
  }
  free(param);
  //printf("C_iter=%d\n", C_iter);
  return NULL;
}


int main(int argc, const char* argv[])
{

  //security check for args
  args_check(argc, argv);

  //get threads numbers from command line
  int nprod = atoi(argv[1]);
  int ncons = atoi(argv[2]);

  //create threads array
  pthread_t prod_threads[nprod];
  pthread_t cons_threads[ncons];

  //init semaphore and mutex
  sem_p_init(&buf.full, 0);
  sem_p_init(&buf.empty, NBUF);
  buf.mutex = 0;

  //init iter values
  P_iter = 0;
  C_iter = 0;

  //create all threads, handle number of iterations by computing it as arg
  for (int i = 0; i < nprod; i++)
  {
    int *arg = (int *)malloc(sizeof(*arg));
    *arg = (i < nprod - 1) ? (NITR/nprod) : (NITR - (NITR/nprod)*i);
    pthread_create(&prod_threads[i], NULL, Producer, arg);
  }
  for (int j = 0; j < ncons; j++)
  {
    int *arg = (int *)malloc(sizeof(*arg));
    *arg = (j < ncons - 1) ? (NITR/ncons) : (NITR - (NITR/ncons)*j);
    pthread_create(&cons_threads[j], NULL, Consumer, arg);
  }

  // join all threads
  void *ret = NULL;
  for (int i = 0; i < nprod; i++) pthread_join(prod_threads[i], &ret);
  for (int j = 0; j < ncons; j++) pthread_join(cons_threads[j], &ret);

  //exit semaphores
  sem_p_destroy(buf.full);
  sem_p_destroy(buf.empty);

  return 0;
}
