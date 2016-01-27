#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

#include <xen/interface/io/netif.h>
#include <xen/interface/memory.h>
#include <xen/interface/grant_table.h>

union gnttab_shared_union {
        struct grant_entry_v1 *v1;
        void *addr;
};

static union gnttab_shared_union *gsu =
        (union gnttab_shared_union *)0xffffffff81e2bd38;

static int __init dump_gnttab_init(void)
{
        printk(KERN_ALERT "init the module\n");
        printk(KERN_ALERT "gsu.addr: 0x%016lx\n", gsu->addr);
        printk(KERN_ALERT "gsu.flags=%d, domid=%d, frame=%d\n",
                        gsu->v1[1].flags,
                        gsu->v1[1].domid,
                        gsu->v1[1].frame);
        return 0;
}

static void __exit dump_gnttab_exit(void)
{
        printk(KERN_ALERT "exit the module\n");
}

module_init(dump_gnttab_init);
module_exit(dump_gnttab_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Dump Grant Table");
MODULE_AUTHOR("Dongli Zhang");
