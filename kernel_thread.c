
/***
The below code tests for 60 secs for running a loop continuously
In the code, we can execute it for 60 secs
Issues:
1. Code doesnt go to thread_cleanup and cleans the pointer
2. Segmentation Fault Detected - Solved the issue

Solution:
The Code Executes for desired duration of Time
We shd manaully remove the loaded kernel

***/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>  // for threads
#include <linux/time.h>   // for using jiffies 
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/workqueue.h>


/************ Variable Declaration *********/
static unsigned long puf_init_val = 0x0;
module_param(puf_init_val, uint, S_IRUGO);
static unsigned  long puf_delaysec =30;
module_param(puf_delaysec , uint, S_IRUGO);
static unsigned long puf_base_addr =0xa0000000;
module_param(puf_base_addr, uint, S_IRUGO);

unsigned int PUF_size=1024;                     //1024*4 byte//
unsigned int OMAP_EMIF2 =0x4d000010;
unsigned int OMAP_EMIF2_SHW =0x4d000014;
unsigned int OMAP_EMIF2_temp_polling =0x4d0000cc;


struct task_struct *rowhammer_thread;

/************Functions for Reading and Writing**************************/
/*
*   This function writes the value write_vale to the system address system_addr.
*/
void write_OMAP_system_address(unsigned int system_addr,unsigned int write_val){
  void *write_virtaddr;
  unsigned int written_value;

  write_virtaddr = ioremap(system_addr,sizeof(unsigned int));
  *((unsigned int*)write_virtaddr)=write_val;
  iounmap(write_virtaddr);
}

/*
*   This function reads the value from system address system_addr.
*/
void read_OMAP_system_address(unsigned int system_addr){
  void *read_virtaddr;

  read_virtaddr = ioremap(system_addr, sizeof(unsigned int));
  printk(KERN_INFO "PUF Read:0x%08x at 0x%08x\n",*((unsigned int*)read_virtaddr),system_addr);
  iounmap(read_virtaddr);
}

/*
*   This function disables the DRAM refresh of the external memory interface 2 (EMIF2). Note, that
*  if DRAM refresh of EMIF1 is disabled, it will not be possible to boot a linux kernel.
*/
void disable_refresh(void){
  void *read_virtaddr;     //read address for EMIF 2 register
  void *write_virtaddr;    //write address for EMIF 2 register

  write_virtaddr = ioremap(OMAP_EMIF2,sizeof(unsigned int));
  read_virtaddr = ioremap(OMAP_EMIF2, sizeof(unsigned int));
  *((unsigned int*)write_virtaddr)=0x80000000;
  printk(KERN_INFO "EMIF 2 REG Write 0x%08x at 0x%08x\n",*((unsigned int*)write_virtaddr),OMAP_EMIF2);
  printk(KERN_INFO "EMIF 2 REG Read 0x%08x at 0x%08x\n",*((unsigned int*)read_virtaddr),OMAP_EMIF2);
  iounmap(read_virtaddr);
  iounmap(write_virtaddr);

  write_virtaddr = ioremap(OMAP_EMIF2_SHW,sizeof(unsigned int));
  read_virtaddr = ioremap(OMAP_EMIF2_SHW, sizeof(unsigned int));
  *((unsigned int*)write_virtaddr)=0x80000000;
  printk(KERN_INFO "EMIF 2 REG Write 0x%08x at 0x%08x\n",*((unsigned int*)write_virtaddr),OMAP_EMIF2_SHW);
  printk(KERN_INFO "EMIF 2 REG Read 0x%08x at 0x%08x\n",*((unsigned int*)read_virtaddr),OMAP_EMIF2_SHW);
  iounmap(read_virtaddr);
  iounmap(write_virtaddr);

  write_virtaddr = ioremap( OMAP_EMIF2_temp_polling,sizeof(unsigned int));
  read_virtaddr = ioremap( OMAP_EMIF2_temp_polling, sizeof(unsigned int));
  *((unsigned int*)write_virtaddr)=0x08016893;
  printk(KERN_INFO "EMIF 2 REG Write 0x%08x at 0x%08x\n",*((unsigned int*)write_virtaddr),OMAP_EMIF2_temp_polling);
  printk(KERN_INFO "EMIF 2 REG Read 0x%08x at 0x%08x\n",*((unsigned int*)read_virtaddr), OMAP_EMIF2_temp_polling);
  printk(KERN_INFO "Temp polling alert disabled");
  printk(KERN_INFO "Refresh Disabled at EMIF2");
  iounmap(read_virtaddr);
  iounmap(write_virtaddr);
}

/*
*   This function enables the DRAM refresh of the external memory interface 2 (EMIF2).
*/
void enable_refresh(void){
  void *read_virtaddr;     //read address for EMIF 2 register
  void *write_virtaddr;    //write address for EMIF 2 register

  write_virtaddr = ioremap(OMAP_EMIF2,sizeof(unsigned int));
  read_virtaddr = ioremap(OMAP_EMIF2, sizeof(unsigned int));
  *((unsigned int*)write_virtaddr)=0x00000618;
  printk(KERN_INFO "EMIF 2 REG Write 0x%08x at 0x%08x\n",*((unsigned int*)write_virtaddr),OMAP_EMIF2);
  printk(KERN_INFO "EMIF 2 REG Read 0x%08x at 0x%08x\n",*((unsigned int*)read_virtaddr),OMAP_EMIF2);
  iounmap(read_virtaddr);
  iounmap(write_virtaddr);

  write_virtaddr = ioremap(OMAP_EMIF2_SHW,sizeof(unsigned int));
  read_virtaddr = ioremap(OMAP_EMIF2_SHW, sizeof(unsigned int));
  *((unsigned int*)write_virtaddr)=0x00000618;
  printk(KERN_INFO "EMIF 2 REG Write 0x%08x at 0x%08x\n",*((unsigned int*)write_virtaddr),OMAP_EMIF2_SHW);
  printk(KERN_INFO "EMIF 2 REG Read 0x%08x at 0x%08x\n",*((unsigned int*)read_virtaddr),OMAP_EMIF2_SHW);
  iounmap(read_virtaddr);
  iounmap(write_virtaddr);

  write_virtaddr = ioremap( OMAP_EMIF2_temp_polling,sizeof(unsigned int));
  read_virtaddr = ioremap( OMAP_EMIF2_temp_polling, sizeof(unsigned int));
  *((unsigned int*)write_virtaddr)=0x58016893;
  printk(KERN_INFO "EMIF 2 REG Write 0x%08x at 0x%08x\n",*((unsigned int*)write_virtaddr),OMAP_EMIF2_temp_polling);
  printk(KERN_INFO "EMIF 2 REG Read 0x%08x at 0x%08x\n",*((unsigned int*)read_virtaddr), OMAP_EMIF2_temp_polling);
  printk(KERN_INFO "Temp polling alert enabled");
  printk(KERN_INFO "Refresh enabled at EMIF2");
  iounmap(read_virtaddr);
  iounmap(write_virtaddr);
}

/*
*   This function reads the contents of the PUF memory range.
*/
static unsigned int PUF_read_query()
{
  unsigned int addr,puf_read_loop,puf_read_vale;
  puf_read_vale=0;
  //addr=0xa0000000;
  addr = puf_base_addr;	
  printk(KERN_INFO "PUF Query START.\n");
  for(puf_read_loop=0;puf_read_loop<PUF_size;puf_read_loop++){
    read_OMAP_system_address(addr);
    addr=addr+4;
  }

  printk(KERN_INFO "PUF Query END.\n");
  enable_refresh();
  return puf_read_vale;
}
/*
*   This function reads the contents of the PUF memory range for hammering.
*/
static void PUF_read_hammer()
{
  unsigned int addr,puf_read_loop,puf_read_vale;
  void *read_virtaddr;
  unsigned long x;
  addr = puf_base_addr;	
  
  for(puf_read_loop=0;puf_read_loop<PUF_size;puf_read_loop++){
     read_virtaddr = ioremap(addr, sizeof(unsigned int));
  	 x = *((unsigned int*)read_virtaddr);
     iounmap(read_virtaddr);
     addr=addr+8;
  }
  
  return;
}

/*
*   This function writes the initialization value to the PUF memory range.
*/
static void PUF_write_query(void){
  unsigned int addr;
  unsigned int puf_write_loop;
  puf_write_loop=0;

  addr=puf_base_addr;
  for(puf_write_loop=0;puf_write_loop<PUF_size;puf_write_loop++){
    write_OMAP_system_address(addr,puf_init_val);
    addr=addr+4;
  }
}


//thread1 = (struct task_struct ) ;

/*In the call to kthread_create we have passed the following arguments 

  thread_fn : Which is the function that will be run as the thread. 

  NULL: As we are not passing any data to the function we have kept this NULL. 

  name: The process will be named "thread1" in the list of processes . 
 
 */

int thread_fn() {

unsigned long j0,j1, rowhammer_count=0;
int delay = puf_delaysec*HZ, read_complete;
j0 = jiffies;
j1 = j0 + delay;

printk(KERN_INFO "Rowhammer Thread");
PUF_write_query();
disable_refresh();
while (time_before(jiffies, j1)){ 
	//Rowhammer Code here
	PUF_read_hammer();
        //schedule();
	rowhammer_count++;
}
printk(KERN_INFO "The Number of times memory was accessed %lu \n", rowhammer_count);
read_complete = PUF_read_query();
if(read_complete == 0)
	printk(KERN_INFO "Read Completed Successfully");

return 0;
}

int thread_init (void) {
   
    char our_thread[18]="rowhammer_thread";
    
    printk(KERN_INFO "Pandaboard PUF Kernel Module\n");
  
    rowhammer_thread = kthread_create(thread_fn,puf_delaysec,our_thread);
    if((rowhammer_thread))
        {
        printk(KERN_INFO "in if");
        wake_up_process(rowhammer_thread);
        }

    return 0;
}

void thread_cleanup(void) {
  int ret;
  //ret = kthread_stop(rowhammer_thread);
 
  //if(!ret)
  printk(KERN_INFO "Thread stopped");
  //else 
  //	    printk(KERN_INFO "%d", ret);

  //kfree(rowhammer_thread);
  //printk(KERN_INFO "Pointer Deleted");
  printk(KERN_INFO "Module Removed");
 
 return;
}

MODULE_LICENSE("GPL");   
module_init(thread_init);
module_exit(thread_cleanup);

MODULE_AUTHOR("PANDABOARD Rowhammer-BASED DRAM PUF");
MODULE_DESCRIPTION("Pandaboard Rowhammer-based DRAM PUF kernel module for changing DRAM refresh rate and reading/writing tp PUF memory locations");

