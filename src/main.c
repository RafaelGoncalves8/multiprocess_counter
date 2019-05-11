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
    return (n > 1); /* 0 0 1 */
  else if (n % 2 == 0 || n % 3 == 0)
    return 0;
  while (i * i <= n) /* Until i = sqrt(n). */
  {
    if (n % i == 0 || n % (i + 2) == 0) /* Divisible by i or i+2. */
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

  /* Create mmap vectors for each process. */
  for (int i = 0; i < N; i++)
    aux[i] = alloc_mmap(counter/N + 1);

  for (int i = 0; i < N; i++)
  {
    pid[i] = fork();

    /* Child process. */
    if (pid[i] == 0)
    {
      for (int j = 0; j < counter/N + 1; j++)
      {
        if ((i*(counter/N + 1) + j) < counter)
          n = is_prime(nums[i*(counter/N + 1) + j]);
        else
          n = 0; /* Empty slot. */
        aux[i][j] = n;
      }
      exit(0);
    }
  }

  /* Wait all processes to finish. */
  for (int i = 0; i < N; i++)
    waitpid(pid[i], NULL, 0);

  /* Sum boolean vectors into ans. */
  ans = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < counter/N + 1; j++)
      ans += aux[i][j];

  printf("%d\n", ans);

  return 0;
}
