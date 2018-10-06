#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include<stdlib.h>
#include<stdio.h>
int pinfo()
{

    int fd = open( "/proc/self/status", O_RDONLY );
    if ( fd < 0 )
	{perror("Error");
        return 1;}

        int sz,j,h;
        char *c = (char *) calloc(100, sizeof(char));
        sz = read(fd, c, 50);

        c[sz] = '\0';
        for( j =0 ;j<30;j++)
        {
                if(c[j]=='S' && c[j+1] =='t' && c[j+2] =='a' )
                { // printf("%c %c %c",c[j],c[j+1],c[j+2]);
                        h=j;
                        while(c[h]!='\n')
                        {
                                printf("%c",c[h]);
                                h++;
                        }

                }
        }
        printf("\n");

    close( fd );
        fd = open( "/proc/self/stat", O_RDONLY );
    if ( fd < 0 )
	{perror("Error");
        return 1;}

        sz = read(fd, c, 10);

        c[sz] = '\0';
        j=0;
        printf("pid ");
        while(c[j] !=' ')
        {
                printf("%c",c[j]);
                j++;
        }
        printf("\n");

        close( fd );
        fd = open( "/proc/self/statm", O_RDONLY );
        if ( fd < 0 )
        return 1;

        sz = read(fd, c, 10);

        c[sz] = '\0';
        j=0;
        while(c[j] !=' ')
        {
                printf("%c",c[j]);
                j++;
        }
        printf("{virtual memory}\n");
	
	char *res = realpath("/proc/self/exe", NULL);
    if (res) {
        printf("This source is at %s\n", res);
    } else {
        perror("realpath");
        exit(EXIT_FAILURE);
    }

    return 0;
}
