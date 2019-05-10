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
  int nums[MAX];
  int n, ans, counter = 0;
  char c;

  do {
    scanf("%d", &nums[counter++]);
  }
  while((c = getchar()) != '\n');

  /* for (int i = 0; i < counter; i++) */
  /*   printf("%d ", nums[i]); */

  for (int i = 0; i < N; i++)
  {
    pid[i] = fork();
    aux[i] = alloc_mmap(counter/N);

    for (int j = 0; j < counter/N; j++)
      *(aux[i] + j) = nums[(j+1)*i];

    if (pid[i] == 0)
    {
      for (int j = 0; j < counter/N; j++)
      {
        n = is_prime(aux[i][j]);
        printf("%d(%d) ", aux[i][j], n);
        *(aux[i] + j) = n;
      }
      printf("\n");
      exit(0);
    }
  }

  for (int i = 0; i < N; i++)
    waitpid(pid[i], NULL, 0);

  ans = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < counter/N; j++)
    {
      ans += *(aux[i] + j);
      printf("%d ", *(aux[i] + j));
    }

  printf("%d\n", ans);
  return 0;
}
