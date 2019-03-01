/*  @Author		: Om Raheja
	@Date  		: 27th February 2019
	@Filename	: mypipe.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://www.youtube.com/watch?v=7bNMkvcOKlc
*/

typedef struct{
	char *string[10];
	int string_length;
	int led_control_command;
}msg_t;


/* Array of string for populating the structure */
char *array_for_parent[]={"parent0","parent1","parent2","parent3","parent4","parent5","parent6","parent7","parent8","parent9"};
char *array_for_child[]={"child0","child1","child2","child3","child4","child5","child6","child7","child8","child9"};

/* Standard C library Headers */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main()
{
	/* To store pid number of new child */
	pid_t pid;

	/* Check return value of pipe() */
	int ret_val;

	char read_msg_parent[20];
	char read_msg_child[20];

	/* Create instance of msg_t */
	msg_t msgSend;

	/*  Parent->Child
		Parents Writes to Child
		Child Reads
	 */
	int mypipe1[2];

	/*  Child->Parent
		Child Writes to Parent
		Parent Reads
	 */
	int mypipe2[2];

	ret_val = pipe(mypipe1);

	if(ret_val == -1)
	{
		perror("Pipe Error");
		exit(1);
	}

	ret_val = pipe(mypipe2);

	if(ret_val == -1)
	{
		perror("Pipe Error");
		exit(1);
	}

	/* Create new child */
	pid = fork();


	/* Child Process */
	if(pid == 0)
	{

		printf("Reading from Parent\n");
		while((read(mypipe1[0],read_msg_child,sizeof(read_msg_child))))
		{
			printf("Message : %s\n",read_msg_child);
		}

		for(int i=0;i<10;i++)
		{	
			msgSend.string[i]= array_for_child[i];
			write(mypipe2[1],msgSend.string[i],sizeof(msgSend.string[i]));
			sleep(1);
		}

	}
	/* Parent Process */
	else
	{
		printf("Sending messages from parent to child\n");
		for(int i=0;i<10;i++)
		{	
			msgSend.string[i]= array_for_parent[i];
			write(mypipe1[1],msgSend.string[i],sizeof(msgSend.string[i]));
			sleep(1);
		}

		while((read(mypipe2[0],read_msg_parent,sizeof(read_msg_parent))))
		{
			printf("Message : %s\n",read_msg_parent);
		}


	}


}