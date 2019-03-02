#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHSIZE 100


int main(int argc,char *argv[])
{
	int shmid;
	key_t key;
	char *shm;
	char *str;

	key=1234;
	shmid = shmget(key,SHSIZE,IPC_CREAT | 0666);
	




	return 0;
}