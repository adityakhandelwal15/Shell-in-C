#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "ls.h"
#include "pinfo.h"
#include "pinfo_id.h"
#include "ls.c"
#include "pinfo.c"
#include "pinfo_id.c"
int pid,flag_of_execute_it,pid_list[1000],pid_no;
char *user;	
char host[30];
char root_address[100];
char cur_address[100];
char address[100];
int output_redi,input_redi,pipe_flag,output_redi_append;
void execute_it(char *args);
char *Background_names[100];
int Background_pids[100],no_background_process;
char process_status[100];
void handleZ();
void handleC();
int bg_mai_bheja_kya;
char * copy_for_echo;
pid_t cpid ;

int main()
{
	no_background_process =0;
	char *tem;
	user =  getenv("USER");
	gethostname(host,99);
	getcwd(root_address,sizeof(root_address));
	getcwd(cur_address,sizeof(cur_address));

	signal(SIGINT,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	char args[100];
	flag_of_execute_it =1;
	while(flag_of_execute_it==1)
	{
		input_redi=0;
		output_redi=0;
		output_redi_append =0;
		char *a[10];
		printf("\n");
		if (strlen(cur_address) < strlen(root_address))
		{
			printf("%s@%s %s $",user,host,address );
		}
		else
		{
			printf("%s@%s~/%s $",user,host,address );
		}
		fgets(args,99,stdin);
		
		tem = (char*) malloc(100*sizeof(char));
		tem= strtok(args,";");
		int j = 0;
		//command seperated by semicolon and stored in char array a
		while(tem!=NULL)
		{
			a[j]=malloc(50*sizeof(char));
			strcpy(a[j],tem);

			tem = strtok(NULL,";");
			j++;

		}


		//all the commands executed
		for (int i = 0; i < j ; ++i)
		{
			output_redi = 0;
			input_redi = 0;
			pipe_flag =0;
			output_redi_append=0;
			int f=0;
			for (int j = 0; j < strlen(a[i]); ++j)
			{
				if (a[i][j]=='>' && a[i][j+1]!='>' && f==0)
				{
					output_redi=1;
				}
				if (a[i][j]=='>' && a[i][j+1]=='>')
				{
					output_redi_append=1;
					f=1;
				}
				if (a[i][j]=='<')
				{
					input_redi =1;
				}
				if (a[i][j]=='|')
				{
					pipe_flag =1;
				}

			}
			//piping
			

			char *command = strtok(a[i],"|\n");
			char *b[10];
			int no_pipes=0;
			while(command!=NULL)
			{
				b[no_pipes]=malloc(50*sizeof(char));
				strcpy(b[no_pipes],command);

				command = strtok(NULL,"|\n");
				no_pipes++;

			}
			int actual_stdin = dup(0);
			int actual_stdout = dup(1);

			int sav_i = dup(0);
			int sav_o = dup(1);
			
			int pipes[2];

			pipe(pipes);

			for (int h = 0; h < no_pipes ; ++h)
			{
				dup2(sav_i, 0);
				close(sav_i);
				if (h==no_pipes-1)
				{
					//last command
					dup2(actual_stdout,1);
					close(actual_stdout);
				}
				else
				{
					dup2(pipes[1],1);
					close(pipes[1]);
				}


				//input-output redirection
				if(output_redi == 1 && input_redi ==0)
				{
					char * cmd = strtok(b[h],">");
					
					char *file_name = strtok(NULL," \n");
					int file_desc = open(file_name,O_TRUNC | O_WRONLY | O_CREAT , 0644 ); 
					if (file_desc < 0)
					{
						printf("Error\n");
					}
					int savo =dup(1);
		  			dup2(file_desc, 1) ;
		  			close(file_desc);
					execute_it(cmd);
					dup2(savo, 1);
					close(savo);

	  			}
	  			else if(output_redi_append == 1 && input_redi ==0)
				{
					char * cmd = strtok(b[h],">");
					char *file_name = strtok(NULL,"> \n");
					int file_desc = open(file_name,O_WRONLY | O_APPEND | O_CREAT , 0644); 
					if (file_desc < 0)
					{
						printf("Error\n");
					}
					int savo =dup(1);
		  			dup2(file_desc, 1) ;
		  			close(file_desc);
					execute_it(cmd);
					dup2(savo, 1);
					close(savo);

	  			}
	  			else if (input_redi == 1 && output_redi ==0 && output_redi_append ==0)
	  			{
	  				char * cmd = strtok(b[h],"<");
					
					char *file_name = strtok(NULL," \n");
					int file_desc = open(file_name,O_RDONLY); 
					if (file_desc < 0)
					{
						printf("Error %s\n",file_name);
					}
					int savo =dup(0);
		  			dup2(file_desc, 0) ;
		  			close(file_desc);
					execute_it(cmd);
					dup2(savo, 0);
					close(savo);
	  			}
	  			else if(input_redi==1 && output_redi == 1)
	  			{
	  				char * cmd = strtok(b[h],"<");
					
					char *file_name = strtok(NULL," \t>");
					int file_desc = open(file_name,O_RDONLY); 
					if (file_desc < 0)
					{
						printf("Error1\n");
					}
					int savi =dup(0);
		  			dup2(file_desc, 0) ;
		  			close(file_desc);
		  			char *file_name2 = strtok(NULL," ><;\n");
					int file_desc2 = open(file_name2,O_TRUNC | O_WRONLY | O_CREAT , 0644);
					if (file_desc2 < 0)
					{
						printf("Error2\n");
					}

					int savo =dup(1);
		  			dup2(file_desc2, 1) ;
		  			close(file_desc2);
					execute_it(cmd);


					dup2(savo, 1);
					close(savo);

		  			dup2(savi,0);
		  			close(savi);
	  			}
	  			else if(input_redi==1 && output_redi_append == 1)
	  			{
	  				char * cmd = strtok(b[h],"<");
					char *file_name = strtok(NULL," \t>");
					int file_desc = open(file_name,O_RDONLY); 
					if (file_desc < 0)
					{
						printf("Error1\n");
					}
					int savi =dup(0);
		  			dup2(file_desc, 0) ;
		  			close(file_desc);
		  			char *file_name2 = strtok(NULL," ><;\n");
					int file_desc2 = open(file_name2,O_WRONLY | O_APPEND | O_CREAT , 0644);
					if (file_desc2 < 0)
					{
						printf("Error2\n");
					}

					int savo =dup(1);
		  			dup2(file_desc2, 1) ;
		  			close(file_desc2);
					execute_it(cmd);


					dup2(savo, 1);
					close(savo);

		  			dup2(savi,0);
		  			close(savi);
	  			}



	  			else
	  			{
	  				execute_it(b[h]);

	  			}

	  			sav_i = pipes[0];
	  			pipe(pipes);

  			}
  			dup2(actual_stdin,0);
  			close(actual_stdin);
  			//End of for Loop
		}
	}


	return 0;
}

//execute command function

void execute_it(char *args){
	int status =1,c=0; 
	char * single_word;
	char cpy[200];
	char *temp;
	int flag_for_bg=0,i,f;		
	

	printf("\n");

	for (i = 0; i < pid_no; ++i)
	{
		if (pid_list[i]!=0)
		{
			if(waitpid(pid_list[i],&status,WNOHANG)!=0)
			{
				printf("Process Terminated %d\n",pid_list[i] );
				process_status[i]='T';
				pid_list[i] = 0;
			}
		}
	}

	
	flag_for_bg =0;
	strcpy(cpy,args);

	temp = strtok(cpy," \t\n");
	while(temp!=NULL)
	{
		temp = strtok(NULL," \t\n");

		if(temp==NULL)
		{
			break;
		}
		if (strcmp(temp,"&")==0)
		{

			printf("Background Process started\n");
			flag_for_bg = 1;
		}
	}
	single_word = strtok(args," \n\t");
	if(single_word == NULL)
	{
		return;
	}



	
	if(strcmp(single_word,"pwd") == 0)
	{
		int pid=1;
		if(pid == 1)
		{
			char cwd[244];
			getcwd(cwd, sizeof(cwd));
			printf("%s\n",cwd );
		}
		if(flag_for_bg == 1)
		{
			pid = fork();	
		}
		if(pid == 0)
		{
			printf("\nDone %d\n",getpid() );
		}
		
		
		
	}
	else if(strcmp(single_word,"quit") == 0)
	{
		exit(1);
	}
	else if(strcmp(single_word,"overkill") == 0)
	{
		for (int i = 0; i < no_background_process; ++i)
		{
			kill(Background_pids[i],9);
		}
	}
	else if(strcmp(single_word,"fg") == 0)
	{
		single_word = strtok(NULL," \t");
		int job_id = atoi(single_word);
		if (job_id>no_background_process)
		{
			printf("Please enter a valid Job ID!!\n");
			return;
		}
		pid_t pid = Background_pids[job_id-1];
		//while(wait(&status)!=pid);
		bg_mai_bheja_kya = 0;

		cpid = pid; 

		signal(SIGINT,handleC);
		signal(SIGTSTP,handleZ);

		kill(pid,SIGCONT);
		while(!(bg_mai_bheja_kya==1) && (waitpid(pid,&status,WNOHANG)!=pid));
		if (bg_mai_bheja_kya == 1)
		{
			kill(pid,SIGSTOP);
			setpgid(pid,pid);

			process_status[job_id-1]='S' ;
		}
		
	}
	else if(strcmp(single_word,"bg") == 0)
	{
		single_word = strtok(NULL," \t");
		int job_id = atoi(single_word);
		if (job_id>no_background_process)
		{
			printf("Please enter a valid Job ID bro!!\n");
			return;
		}
		process_status[job_id-1]='R';

		pid_t pid = Background_pids[job_id-1];
		printf("%d\n", pid);
		kill(pid,SIGCONT);
		
	}
	else if(strcmp(single_word,"kjob") == 0)
	{
		single_word = strtok(NULL," \t");
		int job_id = atoi(single_word);
		pid_t pid = Background_pids[job_id-1];
		single_word = strtok(NULL," \t");
		int signal = atoi(single_word);
		kill(pid,signal);
	}
	else if(strcmp(single_word,"jobs") == 0)
	{
		for (int i = 0; i < no_background_process; ++i)
		{
			char st = process_status[i];
			printf("[%d] ",i+1  );
			if (st == 'R')
			{
				printf("Running ");
			}
			else if( st == 'T')
			{
				printf("Terminated ");
			}
			else
			{
				printf("Stopped ");
			}
			printf("%s[%d]\n",Background_names[i],Background_pids[i] );
		}
	}
	else if(strcmp(single_word,"pinfo") == 0)
	{
		
		single_word = strtok(NULL," \t");
		if(single_word == NULL)
		{
			pinfo();
		}
		else
		{
		//printf(" yo %s \n",single_word);
			pinfo_id(single_word);
		}
	}
	else if(strcmp(single_word,"setenv") == 0)
	{
		
		single_word = strtok(NULL," \t");
		if(single_word == NULL)
		{
			printf("Error zero arguments given\n");;
		}
		else
		{
			char * val =strtok(NULL," \t\n");
			if(val != NULL)
			setenv(single_word,val,1);
			else
			setenv(single_word,"",1);


		}
	}
	else if(strcmp(single_word,"unsetenv") == 0)
	{
		
		single_word = strtok(NULL," \t");
		if(single_word == NULL)
		{
			printf("Error zero arguments given\n");;
		}
		else
		{
			//char * val =strtok(NULL," \t\n");
			unsetenv(single_word);
		}
	}
	else if(strcmp(single_word,"clock") == 0)
	{
			
			single_word = strtok(NULL," \t");
			single_word = strtok(NULL," \t");
			if(single_word == NULL)
			{printf("Options Required");
			return;
			}
			int alarm = atoi(single_word);
			single_word = strtok(NULL," \t");
			single_word = strtok(NULL," \t");
			if(single_word == NULL)
			{printf("Arguments required");
			return;}
			int n = atoi(single_word);
			clok(alarm,n);
	}
	else if(strcmp(single_word,"remindme") == 0)
	{
		
			single_word = strtok(NULL," \t");
			int alarm = atoi(single_word);
			single_word = strtok(NULL,"\n");
			pid = fork();
	//		signal(SIGCHLD,SIG_DFL);
			if (pid == 0)
			{
				printf("Background reminder set\n");
				sleep(alarm);
				printf("\n%s\n",single_word);
			}
	}

	else if(strcmp(single_word,"cd") == 0)
	{
		char * arg =  strtok(NULL," \n\t");
		if(arg==NULL)
		{	
			printf("Look for man page");
		}
		chdir(arg);
		getcwd(cur_address,sizeof(cur_address));
		int j=0;
		if (strlen(cur_address) == strlen(root_address))
		{
			address[0] =NULL ;
		}
		for (i = strlen(root_address)+1; i <strlen(cur_address) ; ++i)
		{
			address[j] = cur_address[i];
			j++;

		}
		address[j] = NULL;
		if (strlen(cur_address) < strlen(root_address))
		{
			strcpy(address,cur_address);
		}
		

	}
	else if(strcmp(single_word,"clock") == 0)
	{
		int t = 1;
		single_word  = strtok(NULL," \t"); 
		if(single_word !=NULL)
		{
			single_word = strtok(NULL," ");
			t = atoi (single_word);
		}
		else
		{
			printf("Syntax Error");
		}
		char buf[150];
	   time_t curtime;
	   struct tm *loc_time;
	 
	   curtime = time (NULL);
	 
	   loc_time = localtime (&curtime);
	 
	   printf("%s", asctime (loc_time));
	   int seconds = time(NULL);
	   int p_second = time (NULL);
	   int c=0;
	   while(c < 10)
	   {
	   		sleep(t);
	   		c++;
	   		printf("%s", asctime (loc_time));
	   }

	}
	else if(strcmp(single_word,"ls") == 0)
	{
		single_word  = strtok(NULL," \n"); 
		if(single_word !=NULL && strcmp(single_word,"-a")==0)
		{
			lsa();
			return;
		}
		else if (single_word== NULL)
		{
			ls ();
		}
		else if((single_word !=NULL) && (strcmp(single_word,"-al")==0 || strcmp(single_word,"-la")==0 || strcmp(single_word,"-l -a")==0
		|| strcmp(single_word,"-a -l")==0 ))
		{
			ls_al();
			return;
		}
		else if(single_word !=NULL && strcmp(single_word,"-l")==0)
		{
			ls_l();
			return;
		}
		else
		{
			printf("see the man page for options\n");
		}
	}

	else
	{


		char cmd[100];
		strcpy(cmd,single_word);
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
		
		if(flag_for_bg == 1)
		argv[i-1] = NULL;
		int j = 0;

		
		pid = fork();

		if (flag_for_bg == 0)
		{

			if (pid == 0)
			{
				//Child
				execvp(cmd,argv);
			}
			else
			{
				//Parent
				bg_mai_bheja_kya = 0;
				cpid =pid;
				signal(SIGTSTP,handleZ);
				signal(SIGINT,handleC);

				while(!(bg_mai_bheja_kya==1) && (waitpid(pid,&status,WNOHANG)!=pid));
				if (bg_mai_bheja_kya == 1)
				{
					printf("I m stopping pid %d\n",pid );
					kill(pid,SIGSTOP);

					setpgid(pid,pid);
					pid_list[pid_no++] = pid;
					process_status[no_background_process]='S' ;
					Background_pids[no_background_process]=pid;
					Background_names[no_background_process] = (char*)(malloc(100*sizeof(char)));
					strcpy(Background_names[no_background_process],cmd);
					no_background_process++;	
				}

			}
		}
		else
		{
		
				
			
			if (pid == 0)
			{
				execvp(cmd,argv);
			}
			else
			{
				setpgid(pid,pid);
				pid_list[pid_no++] = pid;
				process_status[no_background_process]='R' ;
				Background_pids[no_background_process]=pid;
				Background_names[no_background_process] = (char*)(malloc(100*sizeof(char)));
				strcpy(Background_names[no_background_process],cmd);
				no_background_process++;
			}
		}
	}

}
void handleZ()
{
		bg_mai_bheja_kya = 1;
		

}
void handleC()
{
	kill(cpid,9);
}
























