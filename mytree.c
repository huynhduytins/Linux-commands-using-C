#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

#define FILE_INFO_MAX_LEN 1000
#define PATH_NAME_MAX_LEN 1000

void reset()
{
  printf("\033[0m");
}

void purple()
{
  printf("\033[0;35m");
}

void green()
{
  printf("\033[0;32m");
}

void filestat(char *pathname, struct stat *buf)
{
  if (stat(pathname, buf) != 0)
  {
    return;
  }
}

void printName(struct stat *buf, char *name)
{
  if (S_ISDIR(buf->st_mode))
  {
    purple();
    printf("%s\n", name);
    reset();
  }
  else if (buf->st_mode & S_IXOTH)
  {
    green();
    printf("%s\n", name);
    reset();
  }
  else
  {
    printf("%s\n", name);
  }
}

void printaTree(int livello, char *directory, int *filenum, int *dirnum, int hidden, char *root, bool last)
{
  struct dirent **dobjects;
  int numeroOggetti = scandir(directory, &dobjects, NULL, alphasort);
  if (numeroOggetti == -1)
  {
    perror("scandir");
  }
  char pathnuovo[1024];
  int i;
  for (i = 0; i < numeroOggetti; i++)
  {
    if ((strcmp(dobjects[i]->d_name, ".") == 0) || (strcmp(dobjects[i]->d_name, "..") == 0))
      continue;
    if (hidden == 0)
    {
      if (dobjects[i]->d_name[0] == '.')
      {
        continue;
      }
    }

    char t[100] = "";
    strcat(t, directory);
    strcat(t, "/");
    strcat(t, dobjects[i]->d_name);
    struct stat buf;
    filestat(t, &buf);

    if (i != numeroOggetti - 1)
    {
      if (strcmp(directory, root) != 0)
      {
        printf("│");
      }
      if (livello != 0)
      {
        if (livello > 1 && !last)
        {
          printf("%*s", (livello - 1) * 4 - 1, "");
          printf("│");
          printf("%*s", 4 - 1, "");
        }
        else
        {
          printf("%*s", (livello)*4 - 1, "");
        }
      }
      else
      {
        printf("%*s", livello, "");
      }
      printf("├── ");
      printName(&buf, dobjects[i]->d_name);
    }
    else
    {
      if (strcmp(directory, root) != 0)
      {
        printf("│");
      }
      if (livello != 0)
      {
        if (livello > 1 && !last)
        {
          printf("%*s", (livello - 1) * 4 - 1, "");
          printf("│");
          printf("%*s", 4 - 1, "");
        }
        else
        {
          printf("%*s", (livello)*4 - 1, "");
        }
      }
      else
      {
        printf("%*s", livello, "");
      }
      printf("└── ");
      printName(&buf, dobjects[i]->d_name);
    }
    if (dobjects[i]->d_type == DT_REG)
    {
      (*filenum)++;
    }
    if (dobjects[i]->d_type == DT_DIR)
    {
      (*dirnum)++;
      livello = livello + 1;
      snprintf(pathnuovo, sizeof pathnuovo, "%s%s%s", directory, "/", dobjects[i]->d_name);
      printaTree(livello, pathnuovo, filenum, dirnum, hidden, root, numeroOggetti - 1 == i);
      livello = livello - 1;
    }
  }
}

int main(int argc, char *argv[])
{
  char *target;
  char wd[] = ".";
  char pathname[PATH_NAME_MAX_LEN];
  struct stat buf;

  char msg[256] = "";
  strcat(msg, argv[1]);
  strcat(msg, " [error opening dir]\n\n");

  int countFiles = 0;
  int countDirs = 0;

  target = (argc == 1) ? wd : argv[1];

  filestat(target, &buf);

  if (S_ISDIR(buf.st_mode))
  {
    purple();
    printf("%s\n", target);
    reset();
    printaTree(0, target, &countFiles, &countDirs, 0, argv[1], false);
  }
  else if (buf.st_mode & S_IXOTH)
  {
    green();
    printf("%s ", argv[1]);
    reset();
    printf("[error opening dir]\n\n");
  }
  else
  {
    printf("%s [error opening dir]\n", argv[1]);
  }
  printf("\n%d directories, %d files\n", countDirs, countFiles);
  return 0;
}
