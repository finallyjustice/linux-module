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

static int __init xenstore_guest_init(void)
{
	struct xenbus_transaction xbt;
	int err;
	char *macstr1;
    char macstr2[20];

	/* we can also use full path here or the path of corresponding otherned
	 * backend: /local/domain/0/backend/vif/5/0/feature-gso-tcpv4
	 */
    macstr1 = xenbus_read(XBT_NIL, "device/vif/0", "mac", NULL);
    printk(KERN_ALERT "mc1: %s\n", macstr1);
    
	if (xenbus_scanf(XBT_NIL, "device/vif/0", "mac", "%s", macstr2) < 0)
		printk(KERN_ALERT "Failed to scanf\n");
    else
		printk(KERN_ALERT "mc2: %s\n", macstr2);

    err = xenbus_transaction_start(&xbt);
    if (err)
		printk(KERN_ALERT "Failed to start transaction\n");

	err = xenbus_write(xbt, "device/vif/0", "feature-gso-tcpv6", "0");
	if (err)
		printk(KERN_ALERT "Failed to write\n");

	err = xenbus_write(xbt, "device/vif/0", "new-write-feature", "test");
	if (err)
		printk(KERN_ALERT "Failed to write\n");

	err = xenbus_printf(xbt, "device/vif/0", "new-printf-feature", "%s", "ok");
	if (err)
		printk(KERN_ALERT "Failed to printf\n");
	
	err = xenbus_transaction_end(xbt, 0);
    if (err == -EAGAIN)
		printk(KERN_ALERT "Do again\n");

	return 0;
}

static void __exit xenstore_guest_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(xenstore_guest_init);
module_exit(xenstore_guest_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Xenstore Read/Write in guest");
MODULE_AUTHOR("Dongli Zhang");
