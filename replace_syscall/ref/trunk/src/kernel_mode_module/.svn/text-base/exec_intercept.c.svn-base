#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/proc_fs.h>
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/namei.h>
 
#define MAX_TRY 1024;

MODULE_LICENSE ("GPL");

extern void *sys_call_table[];
asmlinkage long (*original_sys_open) (const char __user * filename, int flags, int mode);


/* open function */
asmlinkage int our_fake_open_function(const char __user *filename, int flags, int mode)
{
    int error;
    struct inode *inode,*inode_t;
	struct nameidata nd,nd_t;
    mm_segment_t fs;
	int i;

	error=user_path_walk(filename,&nd);

    if(!error)
    {
        inode=nd.dentry->d_inode;

		/* check if the file is banned for open */
		for(i=0;i<1;i++)
		{
			/* Modify kernel's limit for valid addresses */
			fs=get_fs( );
			set_fs(get_ds());
				
			/* Protect the file */
			error=user_path_walk("ceva.ttt", &nd_t);
			
			printk(KERN_ALERT "opennnn %s\n", filename);
		
			set_fs(fs);
			if(!error)
			{
                inode_t=nd_t.dentry->d_inode;

                if(inode==inode_t)
				{
					printk(KERN_ALERT "deny 1\n");
					//printk(KERN_ALERT "files opened %i\n", (int)current->user->files);
                    return -EACCES;
				}
			}
		}
	}
		
    return original_sys_open(filename,flags,mode);
}


/*
int new_exec(struct pt_regs regs)
{
        int error = 0;
        char *filename;
        
        lock_kernel();
        filename = getname((char*)regs.ebx);
        error =  PTR_ERR(filename);
        if (IS_ERR(error))
           	goto out;
        
        printk("Hi, the hook is still installed. ;-)\n");
	error = do_execve(filename, (char**)regs.ecx, (char**)regs.edx, &regs);
	putname(filename);
out:
	unlock_kernel();
	return error;
}
*/

static int my_init (void)
{
  	
        /* install hook */
        original_sys_open = sys_call_table[__NR_execve];
        sys_call_table[__NR_execve] = our_fake_open_function; 
        
        printk("Tzzz... (sleeping)\n");
        return 0;
}

static void my_exit (void)
{
   	sys_call_table[__NR_execve] = original_sys_open;
   	return;
}

module_init(my_init);
module_exit(my_exit);

