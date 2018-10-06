#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
  char * inp;
  fgets(inp,99,stdin);
  char *command = strtok(inp,"|\n");
  char *b[10];
  int no_pipes=0;
  while(command!=NULL)
  {
    b[no_pipes]=malloc(50*sizeof(char));
    strcpy(b[no_pipes],command);
    command = strtok(NULL,"|\n");
    no_pipes++;

  }
  
  printf("aditya\n");
  int pipes[10];
  pipe(pipes);
  pid_t pid = fork();

  if(pid==0)
  {
    printf("Child process\n");
    dup2(pipes[0],0);
    close(pipes[1]);
    int i=0;
    char *cmd,*single_word;
    cmd = strtok(b[1]," \t;\n");
    strcpy(single_word,b[1]);
    char *argv[50];
    for (i = 0; i < 50; ++i)
    {
      argv[i] = NULL;
    }
    i =0 ;
    while(single_word != NULL)
    {
      argv[i] = single_word;

      single_word =  strtok(NULL," ,\n,\t");
      i++;
    }
    execvp(cmd,argv);
  }
  else
  {
    printf("Parent\n");
    dup2(pipes[1],1);
    close(pipes[0]);
    char *cmd,*single_word;
    int i=0;
    cmd = strtok(b[0]," \t;\n");
    strcpy(single_word,b[0]);

    char *argv[50];
    for (i = 0; i < 50; ++i)
    {
      argv[i] = NULL;
    }
    i = 0 ;
    while(single_word != NULL)
    {
      argv[i] = single_word;

      single_word =  strtok(NULL," ,\n,\t");
      i++;
    }
    execvp(cmd,argv);

  }
  return 0;
}