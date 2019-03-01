/* @Reference : https://www.youtube.com/watch?v=pFLQmnmDOo
*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>


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

	/* Accept */
	do{

		mysock = accept(sock, (struct sockaddr *) 0,0);
		if(mysock == -1)
		{
			perror("Accept Failed");
		}
		else
		{
			memset(buffer,0,sizeof(buffer));
			if(ret_val = recv(mysock,buffer,sizeof(buffer),0) < 0)
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

			printf("Message Received\nRet_v	al = %d\n",ret_val);

			close(mysock);

		}
	}while(1);
	
	return 0;

}