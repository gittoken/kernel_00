/*
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/times.h>
#include <linux/utsname.h>
#include <linux/param.h>
#include <linux/resource.h>
#include <linux/signal.h>
#include <linux/string.h>
#include <linux/ptrace.h>
#include <linux/stat.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <asm/segment.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/unistd.h>

#include <asm/uaccess.h>
#include <asm/string.h>
#include <linux/types.h>

extern void* sys_call_table[];

asmlinkage int (*original_execve)(const char* filename, char *const argv[], char *const envp[]);

int wrapped_execve(const char* filename, char *const argv[], char *const envp[])
{

  printk (KERN_ALERT "execve intercepted\n");
 
  return original_execve(filename, argv, envp);
}

int init_module(void)
{


  original_execve = sys_call_table[__NR_execve];
  sys_call_table[__NR_execve] = wrapped_execve;

  return 0;
}

void cleanup_module(void) {
  sys_call_table[__NR_execve] = original_execve;
} 

*/

#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/ptrace.h>
#include <linux/slab.h>
#include <linux/user.h>
#include <linux/utsname.h>
#include <linux/time.h>
#include <linux/major.h>
#include <linux/stat.h>
#include <linux/vt.h>
#include <linux/mman.h>
#include <linux/elfcore.h>
#include <linux/reboot.h>
#include <linux/tty.h>
#include <linux/console.h>

#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/pgtable.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/proc_fs.h>
#include <linux/ptrace.h>
#include <linux/fs.h>
#include <linux/namei.h>

#include <linux/slab.h>
#include <linux/file.h>
#include <linux/smp.h>
#include <asm/module.h> 
	
MODULE_LICENSE ("GPL");

extern void *sys_call_table[];
asmlinkage int (*original_exec)(struct pt_regs);


asmlinkage int our_sys_exec(struct pt_regs regs)
{ 
	struct dentry *dentry;
    struct nameidata nd;
    char *fname;
    int error = 0;

    fname=(char *) regs.ebx;

    printk(KERN_DEBUG "REMUS: execve - Process %s executing %s\n",current->comm,fname);

//        if (path_init(fname, LOOKUP_FOLLOW, &nd))
        error = user_path_walk(fname, &nd);
        if (!error) {
            dentry = nd.dentry;
            if ((dentry->d_inode))
			{
                printk(KERN_ERR "REMUS: Exec - process %s not authorized to exec file %s\n",current->comm,fname);
                printk(KERN_ERR "REMUS: Exec - process uid %d euid %d\n",current->uid,current->euid);
            }
            path_release(&nd);
        }

    fname=getname((char *) regs.ebx);
    error = PTR_ERR(fname);
    if (IS_ERR(fname))
        goto out;

    error = do_execve(fname, (char __user * __user *)regs.ecx, (char __user * __user *)regs.edx, &regs);
    if (error == 0) current->ptrace &= ~PT_DTRACE;
    putname(fname);
out:

    printk(KERN_DEBUG "REMUS: execve - return value = %d\n",error);

    return error; 
}

static int my_init (void)
{
  	
        // install hook 
        original_exec = sys_call_table[__NR_execve];
        sys_call_table[__NR_execve] = our_sys_exec; 
        
        printk("Module start...\n");
        return 0;
}

static void my_exit (void)
{
	
   	sys_call_table[__NR_execve] = original_exec;
	
	printk("Module ended...\n");
   	return;
}

module_init(my_init);
module_exit(my_exit);


