#include <stdio.h>

volatile int locked = 0;
int semcount;

void lock()
{
  asm("enter:\n"
      "movl $1, %%eax;\n"     //on place 1 dans le registre eax
      "xchgl %%eax, %0;\n"    //on échange les valeurs de lock et eax
      "testl %%eax, %%eax;\n" //met le flag ZF = 1 si lock valait 0
      "jnz enter;\n"          //si ZF != 1, aors lock valait 1, et le trhread reste blocké dans le cas contraire, lock valait 0, et le thread peut entrer en section critique
      : "=m"(locked)
      : "m"(locked)
      : "%eax");
}

//unlock operation using asm
void unlock()
{
  asm("movl $0, %%ebx;\n"  //on met 0 dans eax
      "xchgl %%ebx, %0;\n" //on échange la valeur de eax et lock vaut donc 0, ce qui indique que le thread a terminé sa section critique
      : "=m"(locked)
      : "m"(locked)
      : "%ebx");
}


int getlock(){
  return locked;
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
    lock();
    semcount--;
  } else {
    semwait();
  }
}

void sempost() {
  unlock();
  semcount++;
}
