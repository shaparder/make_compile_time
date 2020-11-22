#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>

//define buffer size and number of iterations prod-cons
#define NBUF 8
#define NITR 1024

typedef struct buffer
{
        int buf[NBUF];           /* shared var */
        int    in;               /* buf[in%BUFF_SIZE] is the first empty slot */
        int    out;              /* buf[out%BUFF_SIZE] is the first full slot */
        sem_t  full;             /* keep track of the number of full spots */
        sem_t  empty;            /* keep track of the number of empty spots */
        pthread_mutex_t mutex;   /* enforce mutual exclusion to shared data */
} buf_t;

buf_t buf;
volatile int P_iter;
volatile int C_iter;

//check if char[] is number
bool isNumber(char number[])
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
void args_check(int argc, char *argv[])
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
  int result = 0, low_num = INT_MIN, hi_num = INT_MAX;

  srand(time(NULL));
  result = (rand() % (hi_num - low_num)) + low_num;
  return result;
}

//print buffer
void print_bufs()
{
  printf("[");
  for(int i = 0; i < NBUF; i++)
  {
    if (buf.buf[i]) printf("%d", buf.buf[i]);
    else printf("X");
    if (i < 9) printf("|");
  }
  printf("]\n\n");
}

void *Producer(void *param)
{
  (void)param;

  while (P_iter < NITR)
  {
    sem_wait(&buf.empty);
    //if another thread uses the buffer, wait
    pthread_mutex_lock(&buf.mutex);
    //increment iter count
    P_iter++;
    //store item
    buf.buf[buf.in] = random_number();
    buf.in = (buf.in+1)%NBUF;
    //release the buffer
    pthread_mutex_unlock(&buf.mutex);
    //increment the number of full slots
    sem_post(&buf.full);
  }

  return NULL;
}

void *Consumer(void *param)
{
  (void)param;
  int item;

  while (C_iter < NITR)
  {
    sem_wait(&buf.full);
    pthread_mutex_lock(&buf.mutex);
    //increment iter
    C_iter++;
    //get line
    item = buf.buf[buf.out];
    buf.out = (buf.out+1)%NBUF;
    //release buffer
    pthread_mutex_unlock(&buf.mutex);
    sem_post(&buf.empty);
  }
  return NULL;
}


int main(int argc, const char* argv[])
{
  printf("Start of the program...\n");

  //security check for args
  args_check(argc, argv);

  //get threads numbers from command line
  int nprod = argv[1];
  int ncons = argv[2];

  //create threads array
  pthread_t prod_threads[nprod];
  pthread_t cons_threads[ncons];

  //init semaphore and mutex
  sem_init(&buf.full, 0, 0);
  sem_init(&buf.empty, 0, NBUF);
  pthread_mutex_init(&buf.mutex, NULL);

  //init iter values
  P_iter = 0;
  C_iter = 0;

  //create all threads
  for (int i = 0; i < nprod; i++) pthread_create(&prod_threads[i], NULL, Producer, NULL);
  for (int j = 0; j < ncons; j++) pthread_create(&cons_threads[j], NULL, Consumer, NULL);

  // join all threads
  void *ret = NULL;
  for (int i = 0; i < nprod; i++) pthread_join(prod_threads[i], &ret);
  for (int j = 0; j < ncons; j++) pthread_join(cons_threads[j], &ret);

  //exit semaphores
  sem_destroy(&buf.full);
  sem_destroy(&buf.empty);

  printf("Compteurs : P_iter=%d, C_iter=%d\n", P_iter, C_iter);

  printf("End of the program...\n");

  return 0;
}
