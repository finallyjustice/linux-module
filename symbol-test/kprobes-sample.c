#include <linux/module.h>

static int __init kprobes_register(void)
{
	unsigned int *my_max_loop = (void *)MY_MAX_LOOP;
	printk(KERN_ALERT "my_max_loop: %u\n", *my_max_loop);
	return 0;
}

static void kprobes_unregister(void)
{
	return;
}

module_init(kprobes_register);
module_exit(kprobes_unregister);
MODULE_LICENSE("GPL");
