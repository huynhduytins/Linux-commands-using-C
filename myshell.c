#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2
#define PATH_NAME_MAX_LEN 1000

void filestat(char *pathname, struct stat *buf)
{
  if (stat(pathname, buf) != 0)
  {
    perror("Error");
    _exit(errno);
  }
}

bool findCommand(char *command)
{
  char *target = "/usr/bin";
  char pathname[PATH_NAME_MAX_LEN];
  struct stat buf;

  filestat(target, &buf);
  DIR *dir = opendir(target);
  struct dirent *dirent = readdir(dir);
  while (dirent != 0)
  {
    sprintf(pathname, "%s/%s", target, dirent->d_name);
    filestat(pathname, &buf);
    if (strstr(dirent->d_name, command) != NULL && strlen(dirent->d_name) == strlen(command))
    {
      return true;
    }
    dirent = readdir(dir);
  }
  closedir(dir);
  return false;
}

int main(int argc, char *argv[])
{
  if (!findCommand(argv[1]))
  {
    char msg[128];
    snprintf(msg, 128, "error: %s not found in /usr/bin\n", argv[1]);
    write(FD_STDOUT, msg, strlen(msg));
    return -1;
  }

  char validCom[100] = "";
  for (int i = 1; i < argc; ++i)
  {
    strcat(validCom, argv[i]);
    strcat(validCom, " ");
  }

  int r;
  r = system(validCom);
  return r;
}