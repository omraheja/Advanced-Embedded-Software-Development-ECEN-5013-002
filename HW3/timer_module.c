/*@Author  : Om Raheja
 *@Date    : 15th Feb 2019
 *@Filename: timer_module.c
 *@ References used: https://www.ibm.com/developerworks/library/l-timers-list/index.html
 * */

/* Standard C Library Headers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>

/* MODULE INFORMTION */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Om Raheja");
MODULE_DESCRIPTION("Timer kernel module fires up every 500msec and prints the user name on the screen");
MODULE_VERSION("1.0");

/* Declare timer_list instance */
struct timer_list my_timer;

/* Defining parameters */
static unsigned long timer_count_ms = 500;	/*default wake up time in msec*/
static char *name = "Om Raheja";			/*string to take name as input*/
static unsigned long counter;           /*keep count of the no of times timer has fired up*/


/*Passing command line arguments to a module*/
/* variable name = timer_count
 * type = long
 * permission = everyone can read; iser can also write
 * */
module_param(timer_count_ms,ulong,0644);

/*MODULE_PARM_DESC() is used to document the arguments that the module
 * can take.
 * */
MODULE_PARM_DESC(timer_count_ms,"Timer Count");

/*Passing command line arguments to a module*/
/* variable name = name
 * type = charp
 * permission = disable the sysfs entry
 * */
module_param(name,charp,0644);

/*MODULE_PARM_DESC() is used to document the arguments that the module
 * can take.
 * */
MODULE_PARM_DESC(name,"Name of user");


static void my_timer_callback(struct timer_list *data)
{
	counter++;   //increment the counter
	printk("Name: %s Count: %lu",name,counter);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(timer_count_ms));
}

static int timer_module_init(void)
{
	printk("Timer is being initialized.....\n");
	
	/* Setup_timer() initializes the timer
	 * The init_timer() is internally called by the setup_timer.
	 * */
	timer_setup(&my_timer,my_timer_callback,0);

	/* mod_timer Sets expiration time */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(timer_count_ms));
	
	return 0;
}

static void timer_module_exit(void)
{
	printk("Exiting Timer\n");
	del_timer(&my_timer);

}

module_init(timer_module_init);
module_exit(timer_module_exit);