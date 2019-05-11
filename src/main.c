#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define N 4
#define MAX 100
#define ERROR -1

int is_prime(int n)
{
  int i = 5;
  if (n <= 3)
    return (n > 1);
  else if (n % 2 == 0 || n % 3 == 0)
    return 0;
  while (i * i <= n)
  {
    if (n & i == 0 || n % (i + 2) == 0)
      return 0;
    i += 6;
  }
  return 1;
}

int * alloc_mmap(int n)
{
  int * ans;

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  ans = (int*) mmap(NULL, sizeof(int)*n, protection, visibility, 0, 0);

  if ((int) ans == -1)
  {
    printf("Erro de alocacao.");
    exit(ERROR);
  }

  return ans;
}


int main() {
  pid_t pid[N];
  int *aux[N];
  int *k;
  int nums[MAX];
  int n, ans, counter = 0;
  char c;

  do {
    scanf("%d", &nums[counter++]);
    /* printf("%d ", nums[counter - 1]); */
  }
  while((c = getchar()) != '\n');

  k = alloc_mmap(1);

  for (int i = 0; i < N; i++)
  {
    pid[i] = fork();
    aux[i] = alloc_mmap(counter/N + 1);

    if (pid[i] == 0)
    {
      for (int j = 0; j < counter/N + 1; j++)
      {
        if ((i*(counter/N + 1) + j) < counter)
          n = is_prime(nums[i*(counter/N + 1) + j]);
        else
          n = 0;
        aux[i][j] = n;
        (*k) += n;
        /* printf("%d(%d) ", nums[i*(counter/N + 1) + j], aux[i][j]); */
      }
      /* printf("\n"); */
      exit(0);
    }
  }

  for (int i = 0; i < N; i++)
    waitpid(pid[i], NULL, 0);

  ans = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < counter/N + 1; j++)
    {
      ans += aux[i][j];
      /* printf("%d ", aux[i][j]); */
    }
  /*  */
  /* printf("\n"); */

  printf("%d\n", (*k));

  return 0;
}
