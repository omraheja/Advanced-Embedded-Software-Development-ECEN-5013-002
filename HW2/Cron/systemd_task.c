/*@Author   : Om Raheja
 *@Date     : 10th Februray 2019 
 *@Course   : Advanced Embedded Software Development
 *@Filename : systemd_task.c 
 *@Reference: https://stackoverflow.com/questions/5141960/get-the-current-time-in-c ; (Author: hexinpeter) 
 * */

/* Standard C library Headers */
#include <stdio.h>
#include <time.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <stdint.h>
#include <sys/time.h>

#define MAX_SIZE 256

/*@Reference for format_time() function :
 * https://stackoverflow.com/questions/5141960/get-the-current-time-in-c ; (Author: hexinpeter) 
 */

// Store the formatted string of time in the output
void format_time(char *output)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    sprintf(output, "[%d/%d/%d %d:%d:%d]",timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}


//Function to display the final sorted array
int my_main(void)
{
        int array_length;
        srand(time(NULL));                      //initialize random generation function
        array_length = (rand()) % MAX_SIZE;     //Generate length of the array using random number generator

        //Allocate memory for the unsorted array
        int array[array_length];

        //Allocate memory for sorted array
        int dst[array_length];

        //Fill the array
        for(int i=0;i<array_length;i++)
        {
                *(array + i) = (rand()) % MAX_SIZE;
        }

        int return_status;

        //system call
        return_status = syscall(398, array, dst, array_length);

        //Check errors associated with system call
        if(return_status == 0)
        {
                printf("System Call Successful\n");
                
		printf("\nThe array after sorting is :\n");
                
		//Print the sorted array
		for(int i=0;i<array_length;i++)
                {
                        printf("%d ",*(dst+i));
                }
        }
	else
        {
                printf("Error\n");
        }

        return 0;
}


int main()
{
	char *time_date = (char *)malloc(50*(sizeof(char)));
	
	//Print the current process ID
	printf("Current Process ID -> %d\n",getpid());
	
	//Print the Current User ID
	printf("Current User ID -> %d \n",getuid());

	//Function call to get time in HH:MM:SS
	format_time(time_date);

	//Print the current date and time
	printf("Current date and time ->%s\n",time_date);

	//Print the output of the system call
	printf("Output of System call -> \n");
	
	//Function call to display the final output (ie. sorted array)
	my_main();

}
