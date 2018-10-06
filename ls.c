#include <dirent.h> 
#include <stdio.h> 
#include<string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h> 
#include <grp.h>
#include <stdlib.h>
#include<fcntl.h>
#include<errno.h>

int lsa(void) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s ", dir->d_name);
    }
    closedir(d);
  }
  return(0);
}

int ls(){
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL ) {
      if(dir->d_name[0]=='.') continue;
      printf("%s ", dir->d_name);
    }
    closedir(d);
  }
  printf("\n");
  return(0);
}

int ls_al(void)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {

  struct stat buf;

  stat(dir->d_name,&buf);
  int size = buf.st_size;
  int nlink  = buf.st_nlink;
  struct passwd *pw = getpwuid(buf.st_uid);
  struct group *gr = getgrgid(buf.st_gid);
  struct tm lt; 
  time_t t = buf.st_mtime;
  localtime_r(&t, &lt);
  char timebuf[80];
  strftime(timebuf, sizeof(timebuf), "%c", &lt);
      printf( (S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
    printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
    printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
    printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
    printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
    printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
    printf( (buf.st_mode & S_IROTH) ? "r" : "-");
    printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
    printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
  printf("\t%s\t%s\t%d\t%ld\t%s\t%s\n",pw->pw_name,gr->gr_name,nlink,buf.st_size,timebuf,dir->d_name);


}
}

}
int ls_l(void)
{
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if(dir->d_name[0]=='.') continue;
  struct stat buf;

  stat(dir->d_name,&buf);
  int size = buf.st_size;
  int nlink  = buf.st_nlink;
  struct passwd *pw = getpwuid(buf.st_uid);
  struct group *gr = getgrgid(buf.st_gid);
  struct tm lt; 
  time_t t = buf.st_mtime;
  localtime_r(&t, &lt);
  char timebuf[80];
  strftime(timebuf, sizeof(timebuf), "%c", &lt);
      printf( (S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
    printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
    printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
    printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
    printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
    printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
    printf( (buf.st_mode & S_IROTH) ? "r" : "-");
    printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
    printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
  printf("\t%s\t%s\t%d\t%ld\t%s\t%s\n",pw->pw_name,gr->gr_name,nlink,buf.st_size,timebuf,dir->d_name);


}

    closedir(d);

}
}




int clok(int t,int n)
{
int c = 0;
while(c<n)
{c++;
//printf("i m in The loop");
int fd = open( "/proc/driver/rtc", O_RDONLY );
    if ( fd < 0 )
        return 1;

        int sz,j,h;
        char c[210];
        sz = read(fd, c, 200);
        char * temp = strtok(c,":");
        char * temp2 = strtok(NULL,"\n");
        char * temp3 = strtok(NULL,":");
        char * temp4 = strtok(NULL,"\n");
        printf("%s %s",temp2,temp4);
        printf("\n\n");
        sleep(t);
    close( fd );
}
}
