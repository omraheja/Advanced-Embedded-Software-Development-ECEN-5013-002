/*  @Author		: Om Raheja
	@Date  		: 27th February 2019
	@Filename	: queue_server.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://www.softprayog.in/programming/interprocess-communication-using-posix-message-queues-in-linux
				: https://gist.github.com/sevko/d23646ba07c77c15fde9
*/

/*
 * server.c: Server program
 *           to demonstrate interprocess commnuication
 *           with POSIX message queues
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>


#define queue_name			"/om_queue"
#define sizeOfQueue			10

FILE *fptr;

typedef struct{
	char string[20];
	int string_length;
	int led_control_command;
}msg_t;

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

/* Array of string for populating the structure */
char *array_for_client[]={"client0","client1","client2","client3","client4","client5","client6","client7","client8","client9"};
char *array_for_server[]={"server0","server1","server2","server3","server4","server5","server6","server7","server8","server9"};

void set_sig_handler(void);
void handler(int signo, siginfo_t *info, void *extra);

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

/* Signal Handler */
void handler(int signo, siginfo_t *info, void *extra) 
{
	fptr=fopen("om.txt","a");
	fprintf(fptr,"Timestamp: %ld\n",getMicrotime());
	fprintf(fptr,"EXITING PROGRAM\n");
	fclose(fptr);
	exit(0);
}


int main()
{
	set_sig_handler();
	char *ptr;
	mqd_t server_queue;
	struct mq_attr queue_attr;
	queue_attr.mq_maxmsg = sizeOfQueue;
	queue_attr.mq_msgsize = sizeof(msg_t);

	server_queue = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &queue_attr);
	if(server_queue == -1)
	{
		perror("Failed to Open Queue");
		return 1;
	}

	msg_t receive_info;
	ptr = (char *)&receive_info;

	for(int i=0;i<10;i++)
	{
		int return_stat;
		return_stat = mq_receive(server_queue,(char *)&receive_info,sizeof(receive_info),0);
		if(return_stat == -1)
		{
			perror("Error in Receiving");
			return 1;
		}
		fptr = fopen("om.txt","a");
		fprintf(fptr,"Timestamp < %ld >\n",getMicrotime());
		fprintf(fptr,"Message Received from Client\n");
		fprintf(fptr,"Message : %s\n",receive_info.string);
		fprintf(fptr,"Message Length : %d\n",receive_info.string_length);
		fclose(fptr);
	}

	msg_t send_info;

	for(int i=0;i<10;i++)
	{
		strcpy(send_info.string , array_for_client[i]);
		send_info.string_length= strlen(array_for_client[i]);

		int return_stat;
		return_stat = mq_send(server_queue,(char *)&send_info,sizeof(send_info),0);
		if(return_stat == -1)
		{
			perror("Sending Failed");
			return 1;
		}
			fptr = fopen("om.txt","a");
			fprintf(fptr,"Timestamp < %ld >\n",getMicrotime());
			fprintf(fptr,"Message Sent to Client!\n");
			fprintf(fptr,"Message:%s\n",send_info.string);
			fprintf(fptr,"Message Length :%d\n",send_info.string_length);
			fclose(fptr);
			sleep(1);
	}

	// printf("Message Sent!\n");
	// printf("Message:%s\n",send_info.string);

	mq_close(server_queue);
	mq_unlink(queue_name);

	return 0;
}
