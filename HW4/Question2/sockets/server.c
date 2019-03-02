/*  @Author		: Om Raheja
	@Date  		: 27th February 2019
	@Filename	: server.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://www.youtube.com/watch?v=pFLQmnmDOo
				: https://gist.github.com/sevko/d23646ba07c77c15fde9
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

typedef struct{
	char *string;
	int string_length;
	int led_control_command;
}msg_t;

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

/* File pointer */
FILE *fileptr;

/* Array of string for populating the structure */
char *array_for_client[]={"client0","client1","client2","client3","client4","client5","client6","client7","client8","client9"};
char *array_for_server[]={"server0","server1","server2","server3","server4","server5","server6","server7","server8","server9"};
void handler(int signo, siginfo_t *info, void *extra);
void set_sig_handler(void);



/* Set the signal handler */
void set_sig_handler(void)
{
    struct sigaction action;
    action.sa_flags = SA_SIGINFO; 
    action.sa_sigaction = handler;
 
    if (sigaction(SIGINT, &action, NULL) == -1)
    { 
        perror("sigusr: sigaction");
        _exit(1);
    }
}

int sock;
int mysock;
int flag=0;

int main(int argc,char *argv[])
{
	/* Variables */
	//int sock;      			//get return val of socket function
	set_sig_handler();
	struct sockaddr_in server;
	//int mysock;				//hold connection
	char buffer[1024];		//buffer to hold data.
	int ret_val;	
	int len;
	char filereader[30];

	msg_t srvr;

	/* Create TCP/IP socket */
	sock = socket(AF_INET,SOCK_STREAM,0);



	/**/
	if(sock < 0)
	{
		perror("Failed to create Socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(9000);


	/* Call Bind */
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)))
	{
		perror("Bind Failed");
		exit(1);
	}

	/* Listen to network */
	listen(sock,5);

	mysock = accept(sock, (struct sockaddr *) 0,0);

	/* Accept */
	for(int i=0;i<10;i++)
	{
		//mysock = accept(sock, (struct sockaddr *) 0,0);
		if(mysock == -1)
		{
			perror("Accept Failed");
		}
		else
		{
			memset(buffer,0,sizeof(buffer));
			if((ret_val = recv(mysock,buffer,7,0) < 0) && (flag == 0))
			{
				perror("Reading stream message error");
			}
			else if(ret_val == 0)
			{
				//printf("Ending Connection\n");
			}
			else
			{
				fileptr=fopen("om.txt","a");
				fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
				fprintf(fileptr,"Message : %s\n",buffer);
				fclose(fileptr);
			}

			fileptr=fopen("om.txt","a");
			fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
			fprintf(fileptr,"Message in Server = %s\n",buffer);
			fclose(fileptr);


			srvr.string = array_for_server[i];
			int n = send(mysock,srvr.string,strlen(srvr.string),0);
			fileptr=fopen("om.txt","a");
			fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
			fprintf(fileptr,"Message Length = %d\n", n);
			fclose(fileptr);
		}
	}

	return 0;
}

/* Signal Handler */
void handler(int signo, siginfo_t *info, void *extra) 
{
	flag=1;
	fileptr=fopen("om.txt","a");
	fprintf(fileptr,"Timestamp: %ld\n",getMicrotime());
	fprintf(fileptr,"EXITING PROGRAM\n");
	fclose(fileptr);
		close(sock);
	close(mysock);
	exit(0);
}
