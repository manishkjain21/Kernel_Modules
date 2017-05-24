#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/delay.h>
 
MODULE_LICENSE("GPL");
 
static struct timer_list my_timer;
short count = 0;
bool timer_flag=0; 
void my_timer_callback( unsigned long data )
{
	
  printk( "my_timer_callback called (%ld).\n", jiffies );
  count++;
  if (count <= 3) mod_timer( &my_timer, jiffies + msecs_to_jiffies(1000) );


  	  		
}
 
int init_module( void )
{
  int ret;
  unsigned int end_120, end_60, start_t;
  printk("Timer module installing\n");
 
  // my_timer.function, my_timer.data
  // my_timer_callback function called when timer expires
  setup_timer( &my_timer, my_timer_callback, 0 );	
  //add_timer(&my_timer);		//To activate the timer
  printk( "Starting timer to fire in 2 s (%ld)\n", jiffies );
  start_t = jiffies;	
  ret = mod_timer( &my_timer, jiffies + msecs_to_jiffies(1000) );	//Function to modify the already running time
  if (ret) printk("Error in mod_timer\n");
  end_120 = start_t + 120*HZ;
  end_60 = start_t + 60*HZ;
  printk("This is the test for In between Time Modules\n");
  			
  
  while(jiffies <= end_60)
  { 
	printk("Hello This is a Test");
	mdelay(5000);

  }
  printk("We can run functions in between program delays\n");	
  return 0;
}
 
void cleanup_module( void )
{
  int ret;
  // delete the expired timer synchronously so as to avoid race conditions
  ret = del_timer_sync( &my_timer );
  if (ret) printk("The timer is still in use...\n");
 
  printk("Timer module uninstalling\n");
 
  return;
}

//module_init(init_module);
//module_exit(cleanup_module);

MODULE_AUTHOR("Manish Jain");
MODULE_DESCRIPTION("A Simple Timer2 module");
