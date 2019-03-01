/*  @Author		: Om Raheja
	@Date  		: 25th February 2019
	@Filename	: mythread.c
	@Course 	: Advanced Embedded Software Development Spring 2019
	@References	: [https://riptutorial.com/posix/example/16306/posix-timer-with-sigev-thread-notification] 
				: [posix_timer demo code from Professor Richard]
				: [https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/]
				: [http://devarea.com/linux-handling-signals-in-a-multithreaded-application/#.XHYOE4VlCoc]
*/

/* Standard C library Headers */
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>			/* for sleep() */
#include <string.h>			/* for memset() */
#include <signal.h>			/* for struct sigevent and SIGEV_THREAD */
#include <time.h>			/* for timer_create(), struct itimerspec,timer_t and CLOCK_REALTIME */


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
int flag = 0;
static timer_t timerid;


/* Mutex */
pthread_mutex_t lock;


/* Signal Handler */
void handler(int signo, siginfo_t *info, void *extra) 
{
	flag = 1;
}


/* Set the signal handler */
void set_sig_handler(void)
{
    struct sigaction action;
    action.sa_flags = SA_SIGINFO; 
    action.sa_sigaction = handler;
 
    if (sigaction(SIGUSR1, &action, NULL) == -1)
    { 
        perror("sigusr: sigaction");
        _exit(1);
    }
}


/* Child 2 Thread Handler */
void thread_handler(union sigval sv)
{
	/* Store start time of thread handler in start_handler variable */
	clock_t start_handler = clock();

	/* Store the info message */
    char *s = sv.sival_ptr;

    /* Create instance of struct timespec*/
	struct timespec thTimeSpec;

	/* Open /proc/stat file to extract information related to cpu utilization */
	FILE *file_open = popen("cat /proc/stat | grep 'cpu'","r");

	/* Lock with help of mutex */
	pthread_mutex_lock(&lock);

	/* Open file */
	thread.file_pointer = fopen("om.txt","a");

	/* Check for Null condition */
	if(thread.file_pointer == NULL)
	{
		printf("File could not be created\n");
	}
	else
	{
		fprintf(thread.file_pointer,"********************************THREAD 2*********************************\n");
		fprintf(thread.file_pointer,"Start Time Thread 2: %f seconds\n",((double)start_handler/CLOCKS_PER_SEC));
		fprintf(thread.file_pointer,"2nd Thread LINUX ID -> %ld	2nd Thread POSIX ID -> %ld\n",syscall(SYS_gettid),pthread_self());
		while(!feof(file_open))
		{	
			char cpu_util;
			cpu_util = fgetc(file_open);

			fprintf(thread.file_pointer,"%c",cpu_util);

		}
		pclose(file_open);
		fprintf(thread.file_pointer,"End Time Thread 2: %f seconds\n",((double)clock()/CLOCKS_PER_SEC));
		fclose(thread.file_pointer);
	}
	pthread_mutex_unlock(&lock);


    /* Will print "100 miliseconds elapsed." from stored sv data */
    puts(s);

}

/* Block signal on all threads except on the thread we want to run the signal in */
void mask_sig(void)
{
	sigset_t mask;
	sigemptyset(&mask); 
    sigaddset(&mask, SIGUSR1); 
    pthread_sigmask(SIG_BLOCK, &mask, NULL);    
}

/* Thread 1 */
void *first_child(void *argv)
{
	filename *first_child_filename = (filename *)argv;

	mask_sig();

	clock_t start;

	double cpu_time_used;

	start = clock();				

	/* character to read the file content */
	char ch;						

	/* character to store content in lower case */
	char lower_case_ch;				

	/* array to store the count of each alphabet */
	int char_count[256]={0};		

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
		while(!feof(read_file))
		{
			ch = fgetc(read_file);
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


/* Thread 2 */
void *second_child(void *argv)
{
	char info[] = "100 miliseconds elapsed";

	/* Store file name in a variable local to the thread */
	filename *second_child_filename = (filename *)argv;

	/* Create instances for struct sigevent, timespec and itimerspec */
	struct sigevent sev;
	struct timespec mainTimeSpec;	
	struct itimerspec trigger;
	
	/* Store timer count */
	clock_t start2;
	double cpu_time_used2;
	
	/* Record the start time of the thread 2 */
	start2 = clock();

	/* Linux Thread id */
	pid_t tid_2 = syscall(SYS_gettid);

	/* Set all `sev` and `trigger` memory to 0 */
    memset(&sev, 0, sizeof(struct sigevent));
    memset(&trigger, 0, sizeof(struct itimerspec));


    /* 
     * Set the notification method as SIGEV_THREAD:
     *
     * Upon timer expiration, `sigev_notify_function` (thread_handler()),
     * will be invoked as if it were the start function of a new thread.
     *
     */
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = &thread_handler;
    sev.sigev_value.sival_ptr = &info;

   /*
    * Create the timer. In this example, CLOCK_REALTIME is used as the
    * clock, meaning that we're using a system-wide real-time clock for 
    * this timer.
    */
      timer_create(CLOCK_REALTIME, &sev, &timerid);


    /* Timer expiration will occur withing 2 seconds after being armed
     * by timer_settime(). Then the interval timer will takeover 
     */
    trigger.it_value.tv_nsec = 100000000;

	/* Uncomment the following line to set the interval timer and
	 * and see the threadhandler() execute periodically.
	 */
    trigger.it_interval.tv_nsec = 100000000;

    /* Arm the timer. No flags are set and no old_value will be retrieved.
     */
    timer_settime(timerid, 0, &trigger, NULL);

    while(!flag);

    timer_delete(timerid);
}


int main(int argc,char *argv[])
{
	/* Variable to store the timer count */
	clock_t start_parent;
	double cpu_time_used_parent;

	start_parent = clock();

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

	/* Set signal handler */
	set_sig_handler();

	
	/* Create first child/Thread */
	pthread_create(&first_child_thread_id,NULL, first_child, (void *)&file_name);

	/* Create second child/Thread */
	pthread_create(&second_child_thread_id,NULL, second_child, (void *)&file_name);

	/* Apply mutex to make file handling thread safe */
	pthread_mutex_lock(&lock);

	/* open file */
	thread.file_pointer = fopen(file_name.file,"a");
	
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


	/* Block the calling thread until specified thread terminates */
	pthread_join(first_child_thread_id,NULL);
	pthread_join(second_child_thread_id,NULL);

	/* Destroy mutex */
	pthread_mutex_destroy(&lock);

	return 0;
}
