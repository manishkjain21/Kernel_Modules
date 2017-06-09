
/***
The below code tests for 60 secs for running a loop continuously
In the code, we can execute it for 60 secs

It initialises the PUF Memory location


***/


/***********Header Files************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>  // for threads
#include <linux/time.h>   // for using jiffies 
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

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

//static struct delayed_work PUF_work;   -- Used by earlier method for scheduling the task

// Below are variables for thread creation
len = sizeof(struct task_struct);
struct task_struct *thread1;


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
static unsigned int PUF_read_query(struct work_struct *work)
{
  unsigned int addr,puf_read_loop,puf_read_vale;
  puf_read_vale=0;
  addr=0xa0000000;

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
*   This function writes the initialization value to the PUF memory range.
*/
static void PUF_write_query(void){
  unsigned int addr;
  unsigned int puf_write_loop;
  puf_write_loop=0;

  addr=0xa0000000;
  for(puf_write_loop=0;puf_write_loop<PUF_size;puf_write_loop++){
    write_OMAP_system_address(addr,puf_init_val);
    addr=addr+4;
  }
}



void get_puf(unsigned int base_address_puf){
	//PUF code begin 

	unsigned int puf_init_value=0x0;			//PUF Init Value
	unsigned int hammer_number=0;	                //Number of hammers
	unsigned int measurment_loop=0;	                //Loop variable for measurements
	unsigned int no_of_measurements_per_sampledecay=20; //number of sample per sample decay
	unsigned int currentdecay=0;                      //current decay in running loop
	unsigned int hammer_flag=0x1;                     //Hammer Flag.. Hammer Yes or No
	unsigned int no_PUF_rows=32;	                //No of Rows for PUF > 1Row has 1024 words total size:4KB:::
	unsigned int no_hammer_rows=1;	                //No of Rows for Hammer > 1Row has 1024 words total size:4KB::: e.g No of Hammer rows is 8
	unsigned int Sample_delay=60*1000;                     //Measurement sample decay(ms)
	unsigned int total_delay=120*1000;                      //Total decay time(ms)
	unsigned long current_timer_value=0;              //current Timer value in msec,Reference to get timer value from this point
	unsigned int hammer_init_value=0x0;      	//Hammer Rows init Value
	unsigned long relative_decay_time=0x0;             //Decay time relative to starting of application
	int puf_row_select=0;
	int puf_init_select=0;
	int RH_init_select=0;
	unsigned long pair_or_alternate_flag=0x0;	// 0x1: ALT (DSRH), 0x0: PRH (SSRH)

	while(puf_init_select<3){ //begin multiple test with multiple ending loop

		//Set rows
		switch(puf_row_select){
	        case 0: no_hammer_rows=1;no_PUF_rows=1; break;
	        case 1: no_hammer_rows=8;no_PUF_rows=8; break;
	        case 2: no_hammer_rows=32;no_PUF_rows=32; break;
	        default:
	        	no_hammer_rows=1;
    	    		no_PUF_rows=1;
        		puf_row_select=0;
	        	RH_init_select++;
        		break;
    		}

	    	//Set Hammer row IV
		switch(RH_init_select){
			case 0: hammer_init_value=0x0; break;
			case 1: hammer_init_value=0x55555555; break;
			case 2: hammer_init_value=0xaaaaaaaa; break;
			case 3: hammer_init_value=0xffffffff; break;
			default:
				hammer_init_value=0x0;
				RH_init_select=0;
				puf_init_select++;
				break;
    		}

    		//Set PUF row IV
		switch(puf_init_select){
        	case 0: puf_init_value=0x0; break;
        	case 1: puf_init_value=0xaaaaaaaa; break;
	        case 2: puf_init_value=0xffffffff; break;
        	default:puf_init_value=0x0; break;
    		}
	
    		printf("[i] Starting the Rowhammer PUF for PandaBoard\n");

		printf("Number of PUF rows: %d PUF init Value :%x Rowhammer rows init Value:%x\n",no_PUF_rows,puf_init_value,hammer_init_value);
		

		// Iterating the individual measurements
		for(measurment_loop=0;measurment_loop<no_of_measurements_per_sampledecay;measurment_loop++){
			printf("[i] Start measurement: %d\n",measurment_loop);

			// Iterating the invidivual decay times
			for(currentdecay=Sample_delay;currentdecay<=total_delay;currentdecay+=Sample_delay){
				printf("[i] Start decaytime: %d\n",currentdecay/1000);
				refresh_disable();
				Init_puf_and_hammer_rows(base_address_puf,no_PUF_rows,puf_init_value,no_PUF_rows,hammer_init_value,pair_or_alternate_flag);
				current_timer_value=get_timer(0);
				printf("[i]\tTimer elapsed since its reset %lu sec\n",current_timer_value/1000);
				printf("[i] Decay: %d sec\n",currentdecay/1000);
				relative_decay_time=currentdecay+current_timer_value;
				printf("[i]\tRelative Decay: %lu msec\n",relative_decay_time);
				current_timer_value=get_timer(0);

				//  Begin actual hammer procedure
				while(get_timer(0)<relative_decay_time){
					//	Check timer overflow and update relative time
					if(get_timer(0)>=0 && get_timer(0)<=10){
						relative_decay_time=currentdecay-(894784-current_timer_value);
						printf("[i]\tUpdated relative Decay: %lu msec\n",relative_decay_time);
					}

					// Hammering will be done
					if(hammer_flag==1){
						hammering_rows(base_address_puf,no_hammer_rows,pair_or_alternate_flag);
						hammer_number++;
					}
				}

				printf("[i] Total hammer attempts per row: %d\n",hammer_number);
				get_temperature();
				refresh_enable();
				//printf("[i] Starting PUF read-out\n");
				Read_puf(base_address_puf,no_PUF_rows,pair_or_alternate_flag);
				//printf("[i] PUF reading end\n");

				printf("[i] Finished PUF query for decaytime: %d\n",currentdecay);
				hammer_number=0;
			}
			printf("End measurement:%d\n",measurment_loop);
		}
		puf_row_select++;
	}
}//End get_puf function

/*
The below function takes care of timing so as to run the program accordingly
*/


/*In the call to kthread_create we have passed the following arguments 

  thread_fn : Which is the function that will be run as the thread. 

  NULL: As we are not passing any data to the function we have kept this NULL. 

  name: The process will be named "thread1" in the list of processes . 
 
 */

int thread_fn() {

  unsigned long j0,j1;
  int delay = puf_delaysec*HZ;
  j0 = jiffies;
  j1 = j0 + delay;

  printk(KERN_INFO "Pandaboard PUF Kernel Module\n");
  PUF_write_query();
  disable_refresh();

  while (time_before(jiffies, j1)){ 

        schedule();
  }
  printk(KERN_INFO "Timer Completed");
 
return 0;
}


int thread_init (void) {
   
    char our_thread[8]="thread1";
    
    printk(KERN_INFO "in init");
    thread1 = kthread_create(thread_fn,puf_delaysec,our_thread);
    if((thread1))
        {
        printk(KERN_INFO "in if");
        wake_up_process(thread1);
        }
	
    
    return 0;
}

void thread_cleanup(void) {

  kfree(thread1);  
 
  printk(KERN_INFO "Module Removed");
 
 return;
}

MODULE_LICENSE("GPL");   
module_init(thread_init);
module_exit(thread_cleanup);

MODULE_AUTHOR("PANDABOARD Rowhammer-BASED DRAM PUF");
MODULE_DESCRIPTION("Pandaboard Rowhammer-based DRAM PUF kernel module for changing DRAM refresh rate and reading/writing tp PUF memory locations");

