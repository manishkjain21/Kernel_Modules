
/***
The below code tests for 60 secs for running a loop continuously
In the code, we can execute it for 60 secs
Issues:
1. Code doesnt go to thread_cleanup and cleans the pointer

***/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>  // for threads
#include <linux/time.h>   // for using jiffies 
#include <linux/timer.h>


static struct task_struct *thread1;

/*In the call to kthread_create we have passed the following arguments 

  thread_fn : Which is the function that will be run as the thread. 

  NULL: As we are not passing any data to the function we have kept this NULL. 

  name: The process will be named "thread1" in the list of processes . 
 
 */

int thread_fn() {

unsigned long j0,j1;
int delay = 60*HZ;
j0 = jiffies;
j1 = j0 + delay;

printk(KERN_INFO "In thread1");

while (time_before(jiffies, j1))
        schedule();

printk(KERN_INFO "Timer Completed");

return 0;
}



int thread_init (void) {
   
    char our_thread[8]="thread1";
    printk(KERN_INFO "in init");
    thread1 = kthread_create(thread_fn,NULL,our_thread);
    if((thread1))
        {
        printk(KERN_INFO "in if");
        wake_up_process(thread1);
        }

    return 0;
}

void thread_cleanup(void) {
 int ret;
 ret = kthread_stop(thread1);
 if(!ret)
  printk(KERN_INFO "Thread stopped");

}

MODULE_LICENSE("GPL");   
module_init(thread_init);
module_exit(thread_cleanup);



