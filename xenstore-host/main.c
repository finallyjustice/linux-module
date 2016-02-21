#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>

#include <linux/mm.h>
#include <linux/cpu.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <asm/tlb.h>

#include <xen/xen.h>
#include <xen/xenbus.h>

#include <xen/interface/io/netif.h>
#include <xen/interface/memory.h>
#include <xen/interface/grant_table.h>

static int __init xenstore_host_init(void)
{
    struct xenbus_transaction xbt;
    int err;
    char *macstr1;
    char macstr2[18];
    char *bridgestr;

    macstr1 = xenbus_read(XBT_NIL, "backend/vif/1/0", "mac", NULL);
    printk(KERN_ALERT "macstr1: %s\n", macstr1);

    if(xenbus_scanf(XBT_NIL, "backend/vif/1/0", "mac", "%s", macstr2) < 0)
        printk(KERN_ALERT "Failed to scanf\n");
    else
        printk(KERN_ALERT "macstr2: %s\n", macstr2);

    bridgestr = xenbus_read(XBT_NIL, "backend/vif/1/0", "bridge", NULL);
    printk(KERN_ALERT "bridgestr: %s\n", bridgestr);

    err = xenbus_transaction_start(&xbt);
    if (err)
        printk(KERN_ALERT "Failed to start transaction\n");

    err = xenbus_write(xbt, "backend/vif/1/0", "host-feature", "test");
    if (err)
        printk(KERN_ALERT "Failed to write\n");

    err = xenbus_write(xbt, "backend/vif/1/0", "host-printf", "ok");
    if (err)
        printk(KERN_ALERT "Failed to printf\n");

    err = xenbus_transaction_end(xbt, 0);
    if (err)
        printk(KERN_ALERT "Do again\n");

    return 0;
}

static void __exit xenstore_host_exit(void)
{
    printk(KERN_ALERT "exit the module\n");
}

module_init(xenstore_host_init);
module_exit(xenstore_host_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Xenstore Read/Write in host");
MODULE_AUTHOR("Dongli Zhang");
