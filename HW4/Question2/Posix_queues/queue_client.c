/*  @Author		: Om Raheja
	@Date  		: 27th February 2019
	@Filename	: queue_client.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://www.softprayog.in/programming/interprocess-communication-using-posix-message-queues-in-linux
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>
#include <sys/time.h>

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

int main()
{
	mqd_t client_queue;
	msg_t send_info;
	msg_t receive_info;
	struct mq_attr queue_attr;
	queue_attr.mq_maxmsg = sizeOfQueue;
	queue_attr.mq_msgsize = sizeof(msg_t);

	client_queue = mq_open(queue_name, O_CREAT | O_RDWR , 0666, &queue_attr);

	if(client_queue == (mqd_t)-1)
	{
		perror("Failed to Open Queue");
		return 1;
	}


for(int i=0;i<10;i++)
{
	strcpy(send_info.string , array_for_server[i]);
	send_info.string_length =strlen(array_for_server[i]);

	int return_stat = mq_send(client_queue,(char *)&send_info,sizeof(send_info),0);

	if(return_stat == -1)
	{
		perror("Error in Sending message");
		return 1;
	}

	fptr = fopen("om.txt","a");
	fprintf(fptr,"Message Sent to Server!\n");
	fprintf(fptr,"Message :%s\n",send_info.string);
	fprintf(fptr,"Message Length : %d\n",send_info.string_length);
	fclose(fptr);
	sleep(1);
}



for(int i=0;i<10;i++)
{
	int return_status = mq_receive(client_queue,(char *)&receive_info,sizeof(receive_info),0);
	if(return_status == -1)
	{
		perror("Error in Receiving info");
		return 1;
	}

	fptr = fopen("om.txt","a");
	fprintf(fptr,"Message Received from Server!\n");
	fprintf(fptr,"Message : %s\n",receive_info.string);
	fprintf(fptr,"Message Length : %d\n",receive_info.string_length);
	fclose(fptr);
}

mq_close(client_queue);
mq_unlink(queue_name);

	return 0;
}
