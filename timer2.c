#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
 
MODULE_LICENSE("GPL");
 
static struct timer_list my_timer;
 
void my_timer_callback( unsigned long data )
{
  printk( "my_timer_callback called (%ld).\n", jiffies );
}
 
int init_module( void )
{
  int ret;
 
  printk("Timer module installing\n");
 
  // my_timer.function, my_timer.data
  // my_timer_callback function called when timer expires
  setup_timer( &my_timer, my_timer_callback, 0 );	
  //add_timer(&my_timer);		//To activate the timer
  printk( "Starting timer to fire in 2 s (%ld)\n", jiffies );
  ret = mod_timer( &my_timer, jiffies + msecs_to_jiffies(2000) );	//Function to modify the already running time
  if (ret) printk("Error in mod_timer\n");

  printk("This is the test for In between Time Modules");
  printk("We can run functions in between program delays");			
 
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
