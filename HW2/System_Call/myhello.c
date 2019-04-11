/*@Author : Om Raheja
 *@Date   : 10th Februray 2019 
 *@Course : Advanced Embedded Software Development
 *@Filename :myhello.c (User space Program)
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

/* Define the maximum size of the array */
#define MAX_SIZE 256

/* Main Function */
int main(void)
{
        int array_length;			//length of the array
        srand(time(NULL));                      //initialize random generation function
        array_length = (rand()) % MAX_SIZE;     //Generate length of the array using random number generator

        //Allocate memory for the unsorted array
        int *array = (int *)malloc(array_length * sizeof(int));

	//Allocate memory for sorted array
        int dst[array_length];

        //Fill the array
        for(int i=0;i<array_length;i++)
        {
                *(array + i) = (rand()) % MAX_SIZE;
        }

	int return_status;			//to check return status from system call

	//Calculate time elapsed in system call
	clock_t ts = clock();

	//System call
        return_status = syscall(398, array, dst, array_length);
	
	//Calculate time elapsed in system call
	ts = clock()-ts;

	double(cpu_time_used)=((double)(ts))/CLOCKS_PER_SEC;

	printf("\nTime spent in executing syscall is %f seconds\n",cpu_time_used);

	//Check errors associated with system call
	//Check for Success
	if(return_status == 0)
	{
		printf("System Call Successful\n");
		printf("The array after sorting is :\n");
		for(int i=0;i<array_length;i++)
        	{
                	printf("%d ",*(dst+i));
        	}
	}
	//Check for invalid pointer and array length
	else if(return_status == -EINVAL) 
	{
		printf("CHECK FOR SOURCE/DESTINATION POINTER\nCHECK FOR ARRAY LENGTH\n");
	}
	//Check for invalid pointer in kernel
	else if(return_status == -ENOMEM )
	{
		printf("INVALID KERNEL_POINTER\n");
	}
	//Check for proper copying data from user<->kernel space
	else if(return_status == -EFAULT)
	{
		printf("COPY FROM USER<->KERNEL SPACE FAILED\n");
	}
	//Check for any other undefined error
	else
	{
		printf("UNDEFINED ERROR");
	}

        return 0;
}

