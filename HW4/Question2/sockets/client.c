/* @Reference : https://www.youtube.com/watch?v=pFLQmnmDOo
*/


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

typedef struct{
	char *string;
	int string_length;
	int led_control_command;
}msg_t;

char* timestamp()
{
	time_t ltime; 		//calendar time
	ltime=time(NULL);	//get current cal time
	return (asctime(localtime(&ltime)));
}

/* File pointer */
FILE *fileptr;

/* Array of string for populating the structure */
char *array_for_client[]={"client0","client1","client2","client3","client4","client5","client6","client7","client8","client9"};
char *array_for_server[]={"server0","server1","server2","server3","server4","server5","server6","server7","server8","server9"};

int main(int argc,char *argv[])
{
	int sock;
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
	server.sin_port = htons(5000);
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
			
			//working->send(sock,array_for_client[i],20,0);
			send(sock,client.string,strlen(client.string),0);


			memset(msgbuffer,0,sizeof(msgbuffer));
			if(ret_val = recv(sock,msgbuffer,7,0) < 0)
			{
				perror("Reading stream message error");
			}
			else if(ret_val == 0)
			{
				printf("Ending Connection\n");
			}
			else
			{
				printf("Message : %s\n",msgbuffer);
			}

			// printf("Message Received\nRet_v	al = %d\n",ret_val);
			printf("Message Received\nRet_v	al = %s\n",msgbuffer);

			sleep(1);

		}

		// if(send(sock,,20,0) < 0)
		// {
		// 	perror("Send Failed");
		// 	exit(1);
		// }
	}

	close(sock);



	return 0;
}