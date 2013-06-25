#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int init_htest()
{
    printk("test.......\n");
    return 0;
}

static void exit_htest()
{
    printk("unload....\n");
}

module_init(init_htest);
module_exit(exit_htest);

MODULE_LICENSE("GPL");
