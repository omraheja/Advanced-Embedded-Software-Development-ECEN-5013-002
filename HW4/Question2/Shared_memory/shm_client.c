/*  @Author		: Om Raheja
	@Date  		: 27th February 2019
	@Filename	: shm_client.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@Reference  : https://gist.github.com/sevko/d23646ba07c77c15fde9

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 	(4096)


const char *mem_name = "/shared_memory";
const char *sem_cli	= "sem_cli";
const char *sem_ser	= "sem_ser";


/* Function Prototypes */
void handler(int signo, siginfo_t *info, void *extra);
void set_sig_handler(void);

typedef struct{
	char string[20];
	int string_length;
	int led_control_command;
}msg_t;


/* Array of string for populating the structure */
char *array_for_client[]={"client0","client1","client2","client3","client4","client5","client6","client7","client8","client9"};
char *array_for_server[]={"server0","server1","server2","server3","server4","server5","server6","server7","server8","server9"};


long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

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

	FILE *fptr;


int main()
{
	set_sig_handler();


	sem_t *semaphore1,*semaphore2;


	msg_t send_info;
	msg_t receive_info={0};

	void *ptr;

	char *pointer = (char *)&receive_info;
	
	/* Create/ open shared memory objects */
	int shm_fd = shm_open(mem_name, O_CREAT | O_RDWR , 0666);

	/* check file descriptor status */
	if(shm_fd < 0)
	{
		perror("Shared Memory Inaccessable\n");
		return 1;
	}

	/* Open semaphore */
	semaphore1 = sem_open (sem_cli,0);
	
	/* Check return value of semaphore */
	if(semaphore1 == NULL)
	{
		perror("SEMAPHORE NOT CREATED");
		return 1;
	}


	/* Open semaphore */
	semaphore2 = sem_open (sem_ser,0);
	
	/* Check return value of semaphore */
	if(semaphore2 == NULL)
	{
		perror("SEMAPHORE NOT CREATED");
		return 1;
	}

	int trunc = ftruncate(shm_fd,SIZE);

	if(trunc == -1)
	{
		perror("FILE TRUNCATION FAILED");
		return 1;
	}

	ptr = mmap(0,SIZE,(PROT_READ | PROT_WRITE), MAP_SHARED,shm_fd,0);

	for(int i=0;i<10;i++)
	{
		strcpy(send_info.string,array_for_server[i]);
		send_info.string_length = strlen(array_for_server[i]);
		fptr = fopen("om.txt","a");
		fprintf(fptr,"Message Sent to Server: %s\n",send_info.string);
		fclose(fptr);
		sem_post(semaphore1);
		sleep(1);
		
		sem_wait(semaphore2);
		fopen("om.txt","a");
		fprintf(fptr,"Message Received!\nMessage: %s",receive_info.string);
		fclose(fptr);
	}

	shm_unlink(mem_name);


	shm_unlink(sem_cli);
	sem_destroy(semaphore1);

	shm_unlink(sem_ser);
	sem_destroy(semaphore2);

	return 0;
}


/* Signal Handler */
void handler(int signo, siginfo_t *info, void *extra) 
{
	/* Check if signal is SIGINT */
	if(signo == 2)
	{
		fptr=fopen("om.txt","a");
		fprintf(fptr,"EXITING PROGRAM\n");
		fclose(fptr);
		exit(0);

		

	}
}