/*
 * Author: Adrian Airinei	
 * user/group restriction done 
		- check current->uid or current->gid
 * allow/deny read access -> intercept open_syscall and mark it as not readable. When intercept read...deny it.
 * allow/deny write access -> analogously with allow/deny read access
 * allow/deny execute - TO BE DONE. We can use open syscall for this.
 * maximum disk space - should be done using quotas....quite difficult
 * maximum memory used - don't know
 * maximum opened files - current->user->files in open_syscall
 * maximum stack size - don't know	
 * maximum CPU - don't know
 * maximum number of processes - current->user->processes, but where should we intercept when a new procces is being created? R: in fork
 * maximum number of files - don't know
 * process .....
 */

 
// status - > exec interception doesn't work yet 
 

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
 
#define MY_SYSCALL_NO 			SIGUSR2

MODULE_LICENSE ("GPL");

extern void *sys_call_table[];

/* suppose forbiden files are in a vector of chars */
char **forbidenOpenFiles;
char **forbidenWriteFiles;

// forbiden uids
int *forbidenUIDs;
char *buf_loc;

// denied mode:   0-read, 1-write
int deniedMode = 1;

asmlinkage long (*original_sys_open) (const char __user * filename, int flags, int mode);
asmlinkage long (*original_sys_write) (int fd, const char * buf, size_t nr);
asmlinkage long (*original_sys_func) (struct pt_regs);

int maxOpenedFiles = 2;

int deniedWriteFd;

/* open function */
asmlinkage int our_fake_open_function(const char __user *filename, int flags, int mode)
{
    int error;
    struct nameidata nd,nd_t;
    struct inode *inode,*inode_t;
    mm_segment_t fs;

	int i;
		
	//printk(KERN_ALERT "open %s\n", filename);
	
    error=user_path_walk(filename,&nd);

    if(!error)
    {
        inode=nd.dentry->d_inode;

		/* check if the file is banned for open */
		for(i=0;i<3;i++)
		{
			/* Modify kernel's limit for valid addresses */
			fs=get_fs( );
			set_fs(get_ds());
				
			/* Protect the file */
			error=user_path_walk(forbidenOpenFiles[i], &nd_t);
		
			set_fs(fs);
			if(!error)
			{
                inode_t=nd_t.dentry->d_inode;
				
				//printk(KERN_ALERT "open %s\n", filename);
				
                if(inode==inode_t)
				{
					printk(KERN_ALERT "deny 1\n");
					//printk(KERN_ALERT "files opened %i\n", (int)current->user->files);
                    return -EACCES;
				}
			}
		}
		
		/* check if the file is banned for write */
		for(i=0;i<1;i++)
		{
			/* Modify kernel's limit for valid addresses */
			fs=get_fs( );
			set_fs(get_ds());
				
			/* Protect the file */
			error=user_path_walk(forbidenWriteFiles[i], &nd_t);

			set_fs(fs);
			if(!error)
			{
                inode_t=nd_t.dentry->d_inode;

                if(inode==inode_t)
				{
					// add the fd to the write denied 
					deniedWriteFd = original_sys_open(filename,flags,mode);
					printk(KERN_ALERT "deny 2\n");
					return deniedWriteFd;
				}
			}
		}
		
		
    }
	
	// check if the current uid is banned to open this file
	for(i=0;i<3;i++)
		if(current->uid == forbidenUIDs[i])
		{
			printk(KERN_ALERT "deny 3\n");
			return -EACCES;
		}
		
	// check the gid in the same manner
		
	
	// check no of already opened files
	//printk(KERN_ALERT "files opened %d\n", current->user->files);
		
    return original_sys_open(filename,flags,mode);
}
 
/* write function */
asmlinkage int our_fake_write_function(int fd, const char * buf, size_t nr)
{
	//printk(KERN_ALERT "write to fd %d",fd);
	
	if(fd == deniedWriteFd)
		return -EACCES;
	
	return original_sys_write(fd,buf,nr);
}

//asmlinkage long my_syscall(int cmd, int syscall, int pid)
asmlinkage long my_syscall(char * buffer)
{ 
	
	printk(KERN_ALERT "SIGUSR2 called!\n");
	//printk(KERN_ALERT "Buffer read : %s\n",buffer);
	
	forbidenOpenFiles[0] = "/tmp/ttt.txt";
	
	// TODO - fetch info from userspace
	return 0;
}

static int __init my_init (void)
{
	int i;
	
	printk(KERN_ALERT "Security policy module started...\n");

	/* modify the handler for the OPEN sys_call */
	original_sys_open  = (void *)xchg(&sys_call_table[__NR_open], our_fake_open_function);
	original_sys_write = (void *)xchg(&sys_call_table[__NR_write], our_fake_write_function);
	original_sys_func  = (void *)xchg(&sys_call_table[MY_SYSCALL_NO], my_syscall);
	
	// initialize forbiden files - for testing
	forbidenOpenFiles = (char **)kmalloc(3*sizeof(char*), GFP_ATOMIC);
	for(i=0;i<3;i++)
		forbidenOpenFiles[i] = (char*)kmalloc(20*sizeof(char), GFP_ATOMIC);
		
	forbidenOpenFiles[0] = "/tmp/test1.txt";
	forbidenOpenFiles[1] = "/tmp/test.txt";
	forbidenOpenFiles[2] = "/tmp/test2.txt";
		
	forbidenWriteFiles = (char **)kmalloc(1*sizeof(char*), GFP_ATOMIC);
	for(i=0;i<1;i++)
		forbidenWriteFiles[i] = (char*)kmalloc(20*sizeof(char), GFP_ATOMIC);
		
	forbidenWriteFiles[0] = "/tmp/testw.txt";
	
	// initialize forbiden uids - for testing
	forbidenUIDs = (int *)kmalloc(3*sizeof(int), GFP_ATOMIC);
	buf_loc = (char*)kmalloc(10*sizeof(char), GFP_ATOMIC);
	
	forbidenUIDs[0] = 333;
	forbidenUIDs[1] = 1000;
	forbidenUIDs[2] = 10;
	
	// denied mode
	//deniedMode = 1;
	
	
    return 0;
}
        
static void __exit my_exit (void)
{
	// restore the original functions
	sys_call_table[__NR_open] = original_sys_open;
	sys_call_table[__NR_write] = original_sys_write;
	sys_call_table[MY_SYSCALL_NO] = original_sys_func;
	
	printk(KERN_ALERT "Security policy module stopped.\n");
}
        
module_init(my_init);
module_exit(my_exit);
