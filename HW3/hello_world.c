/*@Author  : Om Raheja
 *@Date    : 15th Feb 2019
 *@Filename: hello_world.c
 *@Description : This source file contains a Hello World Kernel module
 * */

/* Standard C Library Headers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* @Function name: hello_init
 * @brief: The hello_init() function is called when the module is loaded into the kernel.
 * 	   init functions always return an 'int'
 * @return : 0 (if module is loaded successfully)
 * 	   : non-zero (otherwise)	
 * */

static int hello_init(void)
{
	printk(KERN_ALERT "Hello World! This is Om Raheja's first kernel module\n");
	return 0;
}

/* @Function_name: hello_exit
 * @brief: The hello_exit function is called when the module is removed from the kernel.
 * @return: void
 * */
static void hello_exit(void)
{
	printk(KERN_ALERT "Bye Bye World!\n");
}


/* @module_init is a macro and not a function. It is used to register any kernel module which
 * is supposed to be loaded into the kernel.The kernel invoked the hello_init() when the module
 * is loaded.
 *
 * @module_exit is used when the module is to be removed from the kernel.
 * */
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Om Raheja");
MODULE_DESCRIPTION("Description: My hello world module");

