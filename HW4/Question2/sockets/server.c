/* @Reference : https://www.youtube.com/watch?v=pFLQmnmDOo
*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
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
	/* Variables */
	int sock;      			//get return val of socket function
	struct sockaddr_in server;
	int mysock;				//hold connection
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
	server.sin_port = htons(5000);


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
			if(ret_val = recv(mysock,buffer,7,0) < 0)
			{
				perror("Reading stream message error");
			}
			else if(ret_val == 0)
			{
				printf("Ending Connection\n");
			}
			else
			{
				printf("Message : %s\n",buffer);
			}

			// printf("Message Received\nRet_v	al = %d\n",ret_val);
			printf("Message Received\nRet_v	al = %s\n",buffer);


			srvr.string = array_for_server[i];

			printf("%s \n", srvr.string);
			//working->send(sock,array_for_client[i],20,0);
			int n = send(mysock,srvr.string,strlen(srvr.string),0);

			printf("%d\n", n);





		}
	}

	close(mysock);

	
	return 0;

}