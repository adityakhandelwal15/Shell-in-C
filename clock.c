#include <stdlib.h>
#include <stdio.h>
#include<fcntl.h>
#include<errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int clok(int t)
{
while(0 == 0)
{
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
