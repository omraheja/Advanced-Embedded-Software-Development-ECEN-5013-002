/*@Author  : Om Raheja
 *@Date    : 15th Feb 2019
 *@Filename: sort_list.c
 *@References: https://github.com/torvalds/linux/blob/master/include/linux/list.h
 * */

/* Standard C Library Headers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/sort.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/slab.h>

#define ARRAY_LENGTH 50

/*Declare array of animal name*/
static char *animals[]={"alligator","cat","dog","duck","bison","fox","mole","cat","dog","reindeer","mole","cow","cow","alligator","horse","horse","snake","elephant","chimpanzee","gorrila","shark","frog",
						"zebra","wolf","hippo","tiger","tiger","lion","lion","fox","eagle","baboon","monkey","snail","giraffe","giraffe","mouse","mice","emu","fish","dove","chipmunk","catfish","python",
						"waxbill","dove","chipmunk","catfish","python","waxbill"}; 

/* Structure Animal information */
struct animal_info{
	char *animal_name;
	int animal_count;
	struct list_head animal_address;
};

/* Structure for new linked list */
struct animal_info_new{
	char *animal_name_new;
	int animal_count_new;
	struct list_head animal_address_new;
};

//Global instances
struct animal_info animal_list;
struct animal_info *ptr;

struct animal_info_new animal_list_new;
struct animal_info_new *ptr_new;


//Global Variables
int node_count;			//node count
int node_count_new;		//new node count

char *animal_type="None";
module_param(animal_type,charp,0644);
MODULE_PARM_DESC(animal_type,"Animal Name");

int count_greater_than=0;
module_param(count_greater_than, int ,0644);
MODULE_PARM_DESC(count_greater_than,"Count");


//Comapre function
int compare(const void *element_1, const void *element_2)
{
	return strcmp(*(const char **)element_1,*(const char **)element_2);
}


void sort_array(void)
{

	int i;										//counter variable
	int j;										//counter variable					
	//int node_count;								//node count
	int element_size = sizeof(animals[0]);		//Size of each element
	int total_memory;							//total memory allocated for nodes	
	
	/* Sort the array list */
	sort(animals,ARRAY_LENGTH,element_size,compare,NULL);
	
	/* Print the sorted array */	
	for(i=0;i<ARRAY_LENGTH;i++)
	{
		printk("array[%d]= %s\n",i,*(animals+i));
	}
	
/*****************************************************************/

	INIT_LIST_HEAD(&(animal_list.animal_address));

	for(j=0;j<(ARRAY_LENGTH - 1);j++)
	{
		if(j==0)
		{
			/* dynamic memory allocation */
			ptr=(struct animal_info *) (kmalloc(sizeof(struct animal_info),GFP_KERNEL));	

			/* Copy the string */	
			ptr->animal_name = *(animals+j);
			
			/* Increment the animal count */
			ptr->animal_count=1;

			/* Increment the node count */
			node_count=1;

			/* Add node at tail */
			list_add_tail(&(ptr->animal_address),&(animal_list.animal_address));

		}

		/* If the two strings being compared are same (check strcmp return value) */
		if(!(strcmp(*(animals+j),*(animals+j+1))))
		{
			(ptr->animal_count)++;  						//increment the animal count
		}
		
		/* If the two strings being compared are different */
		if((strcmp(*(animals+j),*(animals+j+1)) != 0))
		{

			/* dynamic memory allocation */
			ptr=(struct animal_info *) (kmalloc(sizeof(struct animal_info),GFP_KERNEL));
			
			/* Increment node count*/
			node_count++;


			
			/* Check if memory allocation is successful or not */
			if(ptr == NULL)
			{
				printk("KMALLOC FAILED\n");
			}
			
			/* Assign animal name */
			ptr->animal_name = *(animals+j+1);
			
			/* Increment animal count*/
			(ptr->animal_count)=1;
			
			/* list_add_tail -adds a new entry 
			 * @new : new entry to be added
			 * @head : list head to add it before
			*/
			list_add_tail(&(ptr->animal_address),&(animal_list.animal_address));

		}
	}


/* Traverse the linked list */
list_for_each_entry(ptr,&(animal_list.animal_address),animal_address)
{
	printk("Animal-> %s  Count-> %d\n",(ptr->animal_name),(ptr->animal_count));
}

printk("Total Nodes-> %d\n",node_count);
printk("Total Memory Allocated for nodes-> %d",((node_count)*(sizeof(animal_list))));

}

/* Function to create the 2nd linked list */
void new_linked_list(void)
{
	int i;					//counter variable
	
	INIT_LIST_HEAD(&(animal_list_new.animal_address_new));

	list_for_each_entry(ptr,&(animal_list.animal_address),animal_address)
	{
		//If no arguments passed in command line
		if((strcmp(animal_type,"None")==0) &&  (count_greater_than == 0))
		{
			printk("Animal-> %s  Count-> %d\n",(ptr->animal_name),(ptr->animal_count));
			node_count_new = node_count;
		}

		//If both name and count are passed in command line
		else if(((strcmp(ptr->animal_name,animal_type))==0) && ((ptr->animal_count) >= count_greater_than))
		{
			/* dynamic memory allocation */
			ptr_new=(struct animal_info_new *) (kmalloc(sizeof(struct animal_info_new),GFP_KERNEL));
			/* assign same name to new structre member as first linked list */
			ptr_new->animal_name_new = ptr->animal_name;
			/* assign same count to new structre member as first linked list */
			ptr_new->animal_count_new = ptr->animal_count;

			list_add_tail(&(ptr_new->animal_address_new),&(animal_list_new.animal_address_new));

			node_count_new++;

			printk("Animal-> %s  Count-> %d\n",(ptr_new->animal_name_new),(ptr_new->animal_count_new));
			
		}
		//Only name is given in command line
		else if(((strcmp(ptr->animal_name,animal_type))==0))
		{
			/* dynamic memory allocation */
			ptr_new=(struct animal_info_new *) (kmalloc(sizeof(struct animal_info_new),GFP_KERNEL));
			/* assign same name to new structre member as first linked list */
			ptr_new->animal_name_new = ptr->animal_name;
			/* assign same count to new structre member as first linked list */
			ptr_new->animal_count_new = ptr->animal_count;

			list_add_tail(&(ptr_new->animal_address_new),&(animal_list_new.animal_address_new));

			node_count_new++;
			
			printk("Animal-> %s  Count-> %d\n",(ptr_new->animal_name_new),(ptr_new->animal_count_new));
		}
		//Only count_greater_than is given in command line
		else if(((ptr->animal_count) >= count_greater_than) && (count_greater_than >0))
		{
			/* dynamic memory allocation */
			ptr_new=(struct animal_info_new *) (kmalloc(sizeof(struct animal_info_new),GFP_KERNEL));
			/* assign same name to new structre member as first linked list */
			ptr_new->animal_name_new = ptr->animal_name;
			/* assign same count to new structre member as first linked list */
			ptr_new->animal_count_new = ptr->animal_count;

			list_add_tail(&(ptr_new->animal_address_new),&(animal_list_new.animal_address_new));

			node_count_new++;
			
			printk("Animal-> %s  Count-> %d\n",(ptr_new->animal_name_new),(ptr_new->animal_count_new));
		}
	}
	printk("Nodes after filtering = %d\n",node_count_new);
}

int destroy_list(void)
{
	list_for_each_entry(ptr,&(animal_list.animal_address),animal_address)
	{
		kfree(ptr);
	}

	return ((node_count)* (sizeof(animal_list)));
}

int destroy_list_new(void)
{

	list_for_each_entry(ptr_new,&(animal_list_new.animal_address_new),animal_address_new)
	{
		kfree(ptr_new);
	}
	return ((node_count_new)* (sizeof(animal_list_new)));

}


//Init function
static int sort_list_init(void)
{
	printk("Entering the kernel module\n");

	sort_array();

	new_linked_list();

	return 0;
}

//Exit Function
static void sort_list_exit(void)
{
	int size_freed;
	int size_freed_new;
	
	size_freed = destroy_list();
	printk("Size freed => %d\n",size_freed);

	size_freed_new = destroy_list_new();
	printk("Size freed of filtered list => %d\n",size_freed_new);

	printk("Exiting the module\n");
}

module_init(sort_list_init);
module_exit(sort_list_exit);


/* MODULE INFORMTION */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Om Raheja");
MODULE_DESCRIPTION("Sort the array of strings (animal names) and create a linked list keeping count of each animal");
MODULE_VERSION("1.0");