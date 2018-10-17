#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

//Variáveis globais são compartilhadas
int x = 30;
char ingressos[30];
pthread_mutex_t ExcMutua = PTHREAD_MUTEX_INITIALIZER;

void *emitirIngresso(void *ingresso)
{
  for (int ing = 0; ing < 30; ing++)
  {
    pthread_mutex_lock(&ExcMutua);
    ingressos[ing] = 'e';
    pthread_mutex_unlock(&ExcMutua);
    sleep(2);
    printf("Ingresso Emitido %d", ing);
    puts(" ");
  }

  pthread_exit(0); //termina o thread de chamada e retorna um valor via retval que (se o thread for joinable) está disponível para outro thread no mesmo processo que chama pthread_join (3).
}

void *RetirarIngresso(void *par)
{
  char *nome = par;
  int n;
  printf("\n%s iniciou sua execução!\n", nome);
  bool verificar_ingresso = false;
  sleep(1);
  for (;;)
  {
    for (int i = 0; i < 30; i++)
    {
      pthread_mutex_lock(&ExcMutua);
      if (ingressos[i] == 'e')
      {
        ingressos[i] = nome;
        printf("\n Comprou --> Comprador: %s - Poltrona: %d\n", nome, i);
        verificar_ingresso = true;
      }
      pthread_mutex_unlock(&ExcMutua);
    }
    if (verificar_ingresso != true)
    {
      printf("\n%s Ainda não foram emitidos novos ingressos, por favor aguarde\n", nome);
    }
    if (ingressos[29] != 'n' && ingressos[29] != 'e')
    {
      pthread_exit(0);
    }
    verificar_ingresso = false;
    sleep(1);
  }

  
  pthread_exit(0);
}
int main()
{
  pthread_t comprador_1, comprador_2, emissor;
  int n = 0;
  for (int zerar = 0; zerar < 30; zerar++)
  {
    ingressos[zerar] = 'n';
  }
  pthread_create(&emissor, 0, emitirIngresso, "");
  pthread_create(&comprador_1, 0, RetirarIngresso, "Thread1");
  pthread_create(&comprador_2, 0, RetirarIngresso, "Thread2");

  printf("Principal 1. x = %d - n = %d\n", x, n);
  /* Espera as threads finalizarem sua execução */
  pthread_join(emissor, 0);
  pthread_join(comprador_1, 0);
  pthread_join(comprador_2, 0);
  printf("\n Os ingressos estão esgotados");
  puts("Relação Poltrona / Comprador");
  for (int i = 0; i < 30; i++)
  {
    printf("Poltrona: %d - Comprador: %c\n", i, ingressos[i]);  puts(" ");
  }
}