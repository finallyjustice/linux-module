#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uio.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

long jdo_fork(unsigned long clone_flags, unsigned long stack_start,
		struct pt_regs *regs, unsigned long stack_size,
		int __user *parent_tidptr, int __user *child_tidptr)
{
	printk(KERN_ALERT "jprob: clone_flags\n");
	/* Always end with a call to jprobe_return() */
	jprobe_return();
	/* NOTREACHED */
	return 0;
}

static struct jprobe my_jprobe = {
	.entry = (kprobe_opcode_t *)jdo_fork
};

static int __init kprobes_register(void)
{
	int ret;
	my_jprobe.kp.addr = (kprobe_opcode_t *)kallsyms_lookup_name("do_fork");
	if (!my_jprobe.kp.addr) {
		printk(KERN_ALERT "Coundn't find %s to plant jprobe\n", "do_fork");
		return -1;
	}

	if ((ret=register_jprobe(&my_jprobe)) < 0) {
		printk(KERN_ALERT "register_jprobe failed, returned %d\n", ret);
		return -1;
	}

	printk(KERN_ALERT "Planted jprobe at %p, handler addr %p\n",
			my_jprobe.kp.addr, my_jprobe.entry);

	return 0;
}

static void kprobes_unregister(void)
{
	unregister_jprobe(&my_jprobe);
	return;
}

module_init(kprobes_register);
module_exit(kprobes_unregister);
MODULE_LICENSE("GPL");
