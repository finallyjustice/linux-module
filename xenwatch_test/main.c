#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <xen/xen.h>
#include <xen/xenbus.h>

static void local_backend_changed(struct xenbus_watch *watch,
		const char **vec, unsigned int len)
{
	printk(KERN_ALERT "xenwatch changed: %s\n", vec[XS_WATCH_PATH]);
}

static struct xenbus_watch be_watch = {
	.node = "backend",
	.callback = local_backend_changed,
};

static int __init xenwatch_test_init(void)
{
	register_xenbus_watch(&be_watch);
	return 0;
}

static void __exit xenwatch_test_exit(void)
{
	register_xenbus_watch(&be_watch);
	printk(KERN_ALERT "exit the module\n");
}

module_init(xenwatch_test_init);
module_exit(xenwatch_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test xenwatch");
MODULE_AUTHOR("Dongli Zhang");
