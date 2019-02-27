/*  @Author		: Om Raheja
	@Date  		: 25th February 2019
	@Filename	: mythread.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: https://riptutorial.com/posix/example/16306/posix-timer-with-sigev-thread-notification 
				: posix_timer demo code from Professor Richard.
				: https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/ 
*/

/* Standard C library Headers */
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>			/* for sleep() */
#include <string.h>			/* for memset() */
#include <signal.h>			/* for struct sigevent and SIGEV_THREAD */
#include <time.h>			/* for timer_create(), struct itimerspec,timer_t and CLOCK_REALTIME */
#include <ctype.h>

typedef struct{
	FILE *file_pointer;
}thread_arguments;

typedef struct{
	char *file;
}filename;

/* Global instances of structure thread_arguments */
thread_arguments thread;

/* Global variables */
pthread_t first_child_thread_id;
pthread_t second_child_thread_id;

/* Mutex */
pthread_mutex_t lock;


//static timer_t timerid;
//struct itimerspec trigger;

/* First Child */
void *first_child(void *argv)
{
	filename *first_child_filename = (filename *)argv;


	clock_t start;

	double cpu_time_used;

	start = clock();

	char ch;						//character to read the file content

	char lower_case_ch;				//character to store content in lower case

	int char_count[256]={0};		//array to store the count of each alphabet

	/* take file name as argument */
	char *argument1 = (char *)argv;

	/* Linux Thread id */
	pid_t tid_1 = syscall(SYS_gettid);

	/* File pointer to read the gdb.txt file */
	FILE *read_file;

	/* Open gdb.txt in read mode */
	read_file = fopen("gdb.txt","r");

	/* Check for null condition */
	if(read_file == NULL)
	{
		printf("File not available\n");
	}
	else
	{
		while((ch=fgetc(read_file)) != EOF)
		{
			lower_case_ch = tolower(ch);

			if((lower_case_ch >= 97) && (lower_case_ch <= 122))
			{
				char_count[lower_case_ch]++;
			}
		}

		fclose(read_file);

	}

	/* Apply mutex to make file handling thread safe */
	pthread_mutex_lock(&lock);

	/* open file */
	thread.file_pointer = fopen(first_child_filename->file,"a");

	/* check for null pointer */
	if(thread.file_pointer == NULL)
	{
		printf("File could not be created\n");
	}
	else
	{
		fprintf(thread.file_pointer,"********************************THREAD 1*********************************\n");
		fprintf(thread.file_pointer,"Start Time Thread 1: %f seconds\n",((double)start/CLOCKS_PER_SEC));
		fprintf(thread.file_pointer,"1st Thread LINUX ID -> %d	1st Thread POSIX ID -> %ld\n",tid_1,pthread_self());
				for(int i=97;i<123;i++)
		{
			if(char_count[i]< 100)
			{
				fprintf(thread.file_pointer,"%c -> %d\n",i,char_count[i]);
			}
		}
		fprintf(thread.file_pointer,"End Time Thread 1: %f seconds\n",((double)clock()/CLOCKS_PER_SEC));
	
		fclose(thread.file_pointer);
	}
	pthread_mutex_unlock(&lock); 

}

void *second_child(void *argv)
{
	filename *second_child_filename = (filename *)argv;

	clock_t start2;

	double cpu_time_used2;

	start2 = clock();

	/* take file name as argument */
	char *argument2 = (char *)argv;

	/* Linux Thread id */
	pid_t tid_2 = syscall(SYS_gettid);

	/* Apply mutex to make file handling thread safe */
	pthread_mutex_lock(&lock);

	/* open file */
	thread.file_pointer = fopen(second_child_filename->file,"a");

	/* check for null pointer */
	if(thread.file_pointer == NULL)
	{
		printf("File could not be created\n");
	}
	else
	{
		fprintf(thread.file_pointer,"********************************THREAD 2*********************************\n");
		fprintf(thread.file_pointer,"Start Time Thread 2: %f seconds\n",((double)start2/CLOCKS_PER_SEC));
		fprintf(thread.file_pointer,"2nd Thread LINUX ID -> %d	2nd Thread POSIX ID -> %ld\n",tid_2,pthread_self());
		fprintf(thread.file_pointer,"End Time Thread 2: %f seconds\n",((double)clock()/CLOCKS_PER_SEC));
		fclose(thread.file_pointer);
	}
	pthread_mutex_unlock(&lock);


}

void thread_handler(union sigval sv)
{
	char *s = sv.sival_ptr;
	struct timespec thTimeSpec;

	/* print out the posix clock to see that time has incremented
	 * by 2 seconds.
	 */
	clock_gettime(CLOCK_REALTIME, &thTimeSpec);
	printf("Clock_getttime: %ld - ",thTimeSpec.tv_sec);

    /* Will print "2 seconds elapsed." from stored sv data */
   puts(s);
}





int main(int argc,char *argv[])
{
//	char info[]="100ms elapsed";
	clock_t start_parent;

	double cpu_time_used_parent;

	start_parent = clock();

	struct sigevent sev;
	struct timespec mainTimeSpec;

	/* structure instance for passing filename */
	filename file_name;

	/* assign file name to the member of the structure */
	file_name.file = argv[1];

	/* Linux Thread id */
	pid_t tid_parent = syscall(SYS_gettid);

	/* Print the command entered in the command line */
	printf("argv[1]: %s\n",argv[1]);

	if(pthread_mutex_init(&lock,NULL) != 0)
	{
		printf("Mutex has failed!\n");
		return 1;
	}

	
	/* Create first child */
	pthread_create(&first_child_thread_id,NULL, first_child, (void *)&file_name);

	/* Create second child */
	pthread_create(&second_child_thread_id,NULL, second_child, (void *)&file_name);


	/* Apply mutex to make file handling thread safe */
	pthread_mutex_lock(&lock);

	/* open file */
	thread.file_pointer = fopen(argv[1],"a");

	/* check for null pointer */
	if(thread.file_pointer == NULL)
	{
		printf("File could not be created\n");
	}
	else
	{
		fprintf(thread.file_pointer,"******************************PARENT THREAD******************************\n");
		fprintf(thread.file_pointer,"Start Time Parent Thread: %f seconds\n",((double)start_parent/CLOCKS_PER_SEC));
		fprintf(thread.file_pointer,"Parent Thread LINUX ID -> %d	Parent Thread POSIX ID -> %ld\n",tid_parent,pthread_self());
		fprintf(thread.file_pointer,"End Time Parent Thread: %f seconds\n",((double)clock()/CLOCKS_PER_SEC));
		fclose(thread.file_pointer);
	}
	pthread_mutex_unlock(&lock);




	/* Setting all 'sev' and 'trigger' memory to 0 */
//	memset(&sev,0,sizeof(struct sigevent));
//	memset(&trigger,0,sizeof(struct itimerspec));

	/* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
//    sev.sigev_notify = SIGEV_THREAD;				
//    sev.sigev_notify_function = &thread_handler;
//    sev.sigev_value.sival_ptr = &info;

     /* Create the timer. In this example, CLOCK_REALTIME is used as the
      * clock, meaning that we're using a system-wide real-time clock for
      * this timer.
      */
//    timer_create(CLOCK_REALTIME, &sev, &timerid);

    /* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
//    trigger.it_value.tv_sec = 0;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */
//     trigger.it_interval.tv_nsec = 100000000;

    /* Arm the timer. No flags are set and no old_value will be retrieved.
     */
//    timer_settime(timerid, 0, &trigger, NULL);

//	clock_gettime(CLOCK_REALTIME, &mainTimeSpec);
//	printf("Clock_getttime: %ld (initial) waiting for 2 seconds\n",mainTimeSpec.tv_sec);

        
	/* Delete (destroy) the timer */
//    timer_delete(timerid);











	/* Block the calling thread until specified thread terminates */
	pthread_join(first_child_thread_id,NULL);
	pthread_join(second_child_thread_id,NULL);

	/* Destroy mutex */
	pthread_mutex_destroy(&lock);

	return 0;
}