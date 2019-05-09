/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>

#define MAX 4

int main() {
  pid_t pid[MAX];
  int n, count = 0, ans = 0;
  char c;

  while(c = getchar() != '\n')
  {
    if (c == ' ')
      c = getchar();
    n = atoi(c);
    if (count < 4)
    {
      pid[count]= fork();
    } else {
      waitpid(pid[count % MAX], NULL, 0);
      pid[count % MAX] = fork();
    }
    if (pid[count % MAX] == 0)
    {
      is_prime(n);
      exit(0);
    }
    count++;
  }

  for (int i = 0; i < MAX; i++)
    waitpid(pid[i], NULL, 0);

  printf("%d\n", ans);
  return 0;
}
