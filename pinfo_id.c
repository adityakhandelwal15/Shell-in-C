#include<errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

int pinfo_id(char  *tp)
{
char *s;
s = strtok(tp," \t\n");
printf("%zu\n",strlen(s));
char  str[100],temp[100],temp1[100],temp2[100];
strcpy(str,"/proc/");
char buf[100];
strcat(str,s);
strcpy(temp,str);
strcpy(temp1,str);
strcpy(temp2,str);
strcat(temp2,"/exe");
char *res = realpath(temp2, NULL);
    if (res) {
        printf("This source is at %s\n", res);
    } else {
        perror("realpath");
	return 0;
       // exit(EXIT_FAILURE);
    }
strcat(str,"/status");

    int fd = open(str, O_RDONLY );
    if ( fd < 0 )
 {       printf("Error Number % d\n", errno);

        // print program detail "Success or failure"
        perror("Program");
}

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
strcat(temp,"/stat");
        fd = open( temp, O_RDONLY );
    if ( fd < 0 )
        return 1;

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

strcat(temp1,"/statm");
        fd = open(temp1, O_RDONLY );
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

    return 0;
}
