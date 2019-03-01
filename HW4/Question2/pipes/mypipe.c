/*  @Author		: Om Raheja
	@Date  		: 27th February 2019
	@Filename	: mypipe.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://www.youtube.com/watch?v=7bNMkvcOKlc
*/

/* Standard C library Headers */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <stdbool.h>

/* Led States */
bool LED_ON;
bool LED_OFF;

typedef struct{
	char *string;
	int string_length;
	int led_control_command;
}msg_t;


/* File pointer */
FILE *fileptr;


/* Array of string for populating the structure */
char *array[]={"Message0","Message1","Message2","Message3","Message4"};
char *array_for_parent[]={"parent0","parent1","parent2","parent3","parent4","parent5","parent6","parent7","parent8","parent9"};
char *array_for_child[]={"child0","child1","child2","child3","child4","child5","child6","child7","child8","child9"};

int main()
{
	int pipe1[2]; //parent to write, child to read
	int pipe2[2]; //child to write, parent to read
	int returnstatus1; //check pipe()
	int returnstatus2; //check pipe()
	pid_t pid;
	
	char readmsg[20];
	char buffer[20];

	int readmsg1;
	int buffer1;

	msg_t payload;
	msg_t info;

	returnstatus1 = pipe(pipe1);
	printf("returnstatus1->%d\n",returnstatus1);

	if(returnstatus1 == -1)
	{
		printf("Pipe 1 could not be created\n");
		return 1;
	}


	returnstatus2 = pipe(pipe2);
	printf("returnstatus2->%d\n",returnstatus2);
	if(returnstatus2 == -1)
	{
		printf("Pipe 2 could not be created\n");
		return 1;
	}


	pid=fork();
	printf("pid--> %d\n",pid);

	if(pid<0)
	{
		printf("Fork Failed\n");
		return 1;
	}
	else if (pid == 0)			//child process
	{
		printf("Writing from child to parent\n");
		/* Writing to Parent (string) */
		for(int i=0;i<10;i++)
		{
			payload.string = array_for_parent[i];
			write(pipe2[1],payload.string,sizeof(readmsg));
		}

		/* Writing from child to parent (integer) */
		for(int i=0;i<5;i++)
		{
			payload.string_length=strlen(array_for_parent[i]);
			write(pipe2[1],&payload.string_length,sizeof(int));
		}

		printf("Reading from Child\n");
		/* Reading from Parent */	
		for(int i=0;i<10;i++)
		{
			memset(buffer,0,sizeof(buffer));
			read(pipe1[0],&buffer,sizeof(buffer));
			printf("Message received from Parent: %s\n",buffer);
		}
		for(int i=0;i<5;i++)
		{
			//memset(buffer1,0,sizeof(buffer1));
			read(pipe1[0],&buffer1,sizeof(int));
			printf("Integer Reveiced from parent : %d\n",buffer1);
		}
		
	}
	else			//parent process
	{
		printf("In Parent Process\n");
		/* Reading from Child */
		for(int i=0;i<10;i++)
		{
			memset(readmsg,0,sizeof(readmsg));
			read(pipe2[0],&readmsg,sizeof(readmsg));
			printf("Message Received from child: %s\n",readmsg);

		}
		for(int i=0;i<5;i++)
		{
//			memset(readmsg1,0,sizeof(readmsg1));
			read(pipe2[0],&readmsg1,sizeof(int));
			printf("Integer Received from child: %d\n",readmsg1);

		}


		printf("Writing to Child\n");
		/* Writing to Child (String) */
		for(int i=0;i<10;i++)
		{
			info.string = array_for_child[i];
			write(pipe1[1],info.string,sizeof(buffer));
		}
		/* Writing to Child (Integer) */
		for(int i=0;i<5;i++)
		{
			info.string_length=strlen(array_for_child[i]);
			write(pipe1[1],&info.string_length,sizeof(int));
		}


	}
}