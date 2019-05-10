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

  ans = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  if ((int) ans == -1)
  {
    printf("Erro de alocacao.");
    exit(ERROR);
  }

  return ans;
}


int main() {
  pid_t pid[N];
  int n, counter = 0;
  char c;
  int *nums;
  int *aux;
  int *ans;

  nums = alloc_mmap(MAX);
  ans = alloc_mmap(1);
  aux = alloc_mmap(1);
  *aux = 0;

  while(c = getchar() != '\n')
  {
    if (c == ' ')
      c = getchar();
    nums[counter++] = atoi(c);
  }

  for (int i = 0; i < N; i++)
  {
    pid[i] = fork();

    if (pid[i] == 0)
    {
      while (*aux < counter)
      {
        *aux++;
        *ans += is_prime(nums[*aux]);
      }
    }
  }

  for (int i = 0; i < N; i++)
    waitpid(pid[i], NULL, 0);

  printf("%d\n", *ans);
  return 0;
}
