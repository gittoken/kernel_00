// base modules inclusions 
#include <linux/init.h>
#include <linux/module.h>

// for getpid() 
#include <linux/unistd.h>

#include <linux/syscalls.h>
#include <asm/unistd.h>
//#include <linux/utsname.h>


MODULE_LICENSE("GPL"); 

static void **sys_call_table;

int (*original_clone)(int (*proc)(void *), void *sp, int flags, void *data);

int our_fake_clone_function(int (*proc)(void *), void *sp, int flags, void *data)
{
    
  // loging the syscall
  printk(KERN_ALERT "[edu]   clone syscall intercepted from %d\n", current->pid);
  
  return original_clone(proc, sp, flags, data);
}

static int my_init (void)
{
      
    /* install hook */
    original_clone = sys_call_table[__NR_clone];
    sys_call_table[__NR_clone] = our_fake_clone_function; 
        
    printk("Security policy module started\n");
    return 0;
}

static void my_exit (void)
{
   	sys_call_table[__NR_clone] = original_clone;
	
	printk("Security policy module removed\n");
	
   	return;
}

module_init(my_init);
module_exit(my_exit);
