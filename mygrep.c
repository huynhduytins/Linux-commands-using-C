#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

void red()
{
  printf("\033[1;31m");
}

void reset()
{
  printf("\033[0m");
}

void mygrep(char *c, char *argv, int len)
{
  int count = 0;
  int pos = 0;
  int i = 0;
  while (i < strlen(c))
  {
    if ((argv[count] == c[i] || (argv[count] == '-')) && c[i] != '\n')
    {
      count++;
    }
    else if (count != 0)
    {
      i = i - count;
      count = 0;
    }
    if (count == len)
    {
      pos = i - count + 1;
      break;
    }
    i++;
  }
  i = 0;
  while (i < strlen(c) && count == len)
  {
    if (i >= pos && i < pos + len)
    {
      red();
      printf("%c", c[i]);
    }
    else
    {
      reset();
      printf("%c", c[i]);
    }
    i++;
  }
}

void main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    char s[100] = "Usage: grep [OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for more information.\n";
    write(FD_STDOUT, s, 100);
  }
  else if (argc == 2)
  {
    int len = strlen(argv[1]);
    while (1)
    {
      char c[100] = "";
      if (read(FD_STDIN, &c, 100) != 0)
      {
        if (strstr(c, argv[1]) != NULL)
        {
          mygrep(c, argv[1], len);
        }
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    int len = strlen(argv[1]);
    for (int a = 2; a < argc; ++a)
    {
      char temp[200];
      FILE *fp;
      fp = fopen(argv[a], "r");
      if (fp)
      {
        reset();
        printf("%s:\n", argv[a]);
        while (!feof(fp))
        {
          fgets(temp, 1000, fp);
          if (strstr(temp, argv[1]) != NULL || strstr(argv[1], "-"))
          {
            mygrep(temp, argv[1], len);
          }
        }
        printf("\n");
        fclose(fp);
      }
      else
      {
        printf("grep: %s: No such file or directory\n", argv[a]);
      }
    }
  }
}