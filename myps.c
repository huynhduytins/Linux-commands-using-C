#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define FD_STDOUT 1

char *get_process_name_by_pid(int pid)
{
  char *name = (char *)calloc(1024, sizeof(char));
  if (name)
  {
    sprintf(name, "/proc/%d/cmdline", pid);
    FILE *f = fopen(name, "r");
    if (f)
    {
      size_t size;
      size = fread(name, sizeof(char), 1024, f);
      if (size > 0)
      {
        if ('\n' == name[size - 1])
          name[size - 1] = '\0';
      }
      fclose(f);
    }
  }
  return name;
}

int main(int argc, char *argv[])
{
  pid_t pid = getpid();
  pid_t pidP = getppid();
  char *name = get_process_name_by_pid(pid);
  char *nameParrent = get_process_name_by_pid(pidP);
  char cur[128];
  char prev[128];

  snprintf(prev, 128, "%d    %s\n", pidP, nameParrent);
  snprintf(cur, 128, "%d    %s\n", pid, name);

  write(FD_STDOUT, "PID       CMD\n", 15);
  write(FD_STDOUT, prev, strlen(prev));
  write(FD_STDOUT, cur, strlen(cur));
  return 0;
}