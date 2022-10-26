#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define FD_STDOUT 1

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    write(FD_STDOUT, "\n", 1);
  }
  else
  {
    for (int i = 1; i < argc; i++)
    {
      write(FD_STDOUT, argv[i], strlen(argv[i]));
      write(FD_STDOUT, " ", 1);
    }
    write(FD_STDOUT, "\n", 1);
  }
  return 0;
}