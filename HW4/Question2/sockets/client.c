/*  @Author		: Om Raheja
	@Date  		: 27th February 2019
	@Filename	: client.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://www.youtube.com/watch?v=pFLQmnmDOo
				: https://gist.github.com/sevko/d23646ba07c77c15fde9
*/

/*Standard C Library Headers*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
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
int flag=0;

int main(int argc,char *argv[])
{
		set_sig_handler();

	//int sock;
	int ret_val;
	struct sockaddr_in server;
	char msgbuffer[1024];

	msg_t client;

	sock = socket(AF_INET, SOCK_STREAM, 0); //create socket


	if(sock < 0)
	{
		perror("Socket Failed");
		exit(1);
	}


	server.sin_family = AF_INET;
	server.sin_port = htons(9000);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");


	if(connect(sock,(struct sockaddr *)&server,sizeof(server)) < 0)
	{
		perror("Connection Failed");
		close(sock);
		exit(1);
	}
	

	while(1)
	{

		for(int i=0;i<10;i++)
		{
			client.string = array_for_client[i];
			int a =send(sock,client.string,strlen(client.string),0);
			fileptr=fopen("om.txt","a");
			fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
			fprintf(fileptr,"Message Length = %d\n",a);
			fclose(fileptr);


			memset(msgbuffer,0,sizeof(msgbuffer));
			if((ret_val = recv(sock,msgbuffer,7,0) < 0) && (flag == 0))
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
				fprintf(fileptr,"Message : %s\n",msgbuffer);
				fclose(fileptr);
			}

			fileptr=fopen("om.txt","a");
			fprintf(fileptr,"Timestamp :%ld\n",getMicrotime());
			fprintf(fileptr,"Message in Client = %s\n",msgbuffer);
			fclose(fileptr);
			sleep(1);

		}
	}
	close(sock);
	return 0;
}

/* Signal Handler */
void handler(int signo, siginfo_t *info, void *extra) 
{
	fileptr=fopen("om.txt","a");
	fprintf(fileptr,"Timestamp: %ld\n",getMicrotime());
	fprintf(fileptr,"EXITING PROGRAM\n");
	fclose(fileptr);
	close(sock);
	exit(0);
}