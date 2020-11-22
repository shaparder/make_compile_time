
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

bool args_check(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("illegal number of arguments\n");
    return false;
  }

  if (!isNumber(argv[1]))
  {
    printf("illegal number of producers\n");
    return false;
  }

  if (!isNumber(argv[2]))
  {
    printf("illegal number of consumers\n");
    return false;
  }

  return true;
}

int main(int argc, const char* argv[])
{
  printf("start");

  if (!args_check(argc, argv))
  {
    printf("usage: ./prodcons N_PRODUCERS N_CONSUMERS\n");
    exit(1);
  }

  int nprod = argv[1];
  int ncons = argv[2];

  pthread_t prod_threads[nprod];
  pthread_t cons_threads[ncons];


}
