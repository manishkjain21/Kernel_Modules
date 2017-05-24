#include <linux/module.h>    	// included for all kernel modules
#include <linux/kernel.h>    	// included for KERN_INFO
#include <linux/init.h>      	// included for __init and __exit macros
#include <linux/sched.h>     	// included for jiffies
#include <linux/time.h>			// included for jiffies to seconds conversion, HZ = 100

//Issues in code during Runtime
// Stalled the CPU


static int __init timer_init(void)
{
	unsigned int start_t, end_60, end_120, end_5;    
	printk(KERN_INFO "Timer Module Init\n");
	start_t = jiffies;
	end_60 = start_t + 60*HZ;
	end_120 = start_t + 120*HZ;
	end_5 = start_t + 5*HZ;
	
	printk(KERN_INFO "Start Hammering the Rows\n");
	do{
		//Write the values to registers for hammering
		printk(KERN_INFO "Read the PUF Values\n");
	}while(!time_after_eq(start_t, end_5));		// Check for this condition
	
	//read the PUF value
	printk(KERN_INFO "Read the PUF Values\n");
	
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit timer_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(timer_init);
module_exit(timer_cleanup);

MODULE_LICENSE("CYSEC");
MODULE_AUTHOR("Manish Jain");
MODULE_DESCRIPTION("A Simple Timer module");
