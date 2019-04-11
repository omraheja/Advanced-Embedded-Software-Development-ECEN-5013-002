/* Course   	: Advanced Embedded Software Development
 * Semester 	: Spring 2019
 * Date     	: 26th January 2019
 * Filename 	: File_handle.c
 * Author   	: Om Raheja
 * Description 	: This code is written to perform basic file handling tasks like
 * 		  Reading/Writing a string, character in a file.
 * */

/*Standard C library Headers*/
#include<stdio.h>
#include<string.h>
#include<malloc.h>

/*Main function*/
int main()
{

	int length;  //to calculate the length of the string

	//Print to standard out an interesting string using printf
	printf("File handling program\n\r");

	/************************************************************/

	FILE *fpointer;    //pointer to a file
	int check;	   //check if fflush is successfull or not

	if(fpointer == NULL)
	{
		printf("File could not be found\n\r");
	}
	else
	{
		//fopen returns the address of the file being created
		fpointer = fopen("om.txt","w"); //create a file in read/write mode and open the file
	
		//Write a character to the file
		fputc('c',fpointer);
		//fputc('\n',fpointer);

		//close the file
		fclose(fpointer);

	/*************************************************************/

		//open the file in append mode
		fpointer= fopen("om.txt","a+");
	
		//Dynamically allocate an array of memory (allocating 50 bytes)
		char *array=(char *) malloc(50*sizeof(char));

		//Read an input string from the command line
		scanf("%[^\n]%*c",array);
		
		length = strlen(array);  //calc the length of the string named 'array'
		
		//Write the string to the file
		fputs(array,fpointer);
		//fputc('\n',fpointer);

		//Flush file output
		check=fflush(stdin);

		if(check == 0)
		{
			printf("fflush() is successful\n");
		}
		else
		{
			printf("fflush() is unsuccessfull\n");
		}
	
		//close the file
		fclose(fpointer);

	/**************************************************************/

		//open the file in reading mode
		fpointer = fopen("om.txt","r");

		//array of characters to display the contents in file
		char *msg = (char *)malloc(50*(sizeof(char)));

		//character variable
		char ch;
		ch = fgetc(fpointer);
		printf("%c\n",ch);
	
		//Read a string of characters
		while(! (feof(fpointer)))
		{
			fgets(msg,length,fpointer);
			printf("%s",msg);
		}

		printf("\n");

		//close the file
		fclose(fpointer);

		//free the memory allocated
		free(msg);
	}

	return 0;

}
