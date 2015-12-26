#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uio.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

static struct kprobe kp = {
	.symbol_name = "do_fork",
};

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	struct thread_info *thread = current_thread_info();

	printk(KERN_INFO "pre-handler thread info: flags = %x, \
		status = %d, cpu = %d, task->pid = %d\n",
		thread->flags, thread->status, thread->cpu, thread->task->pid);

	return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs,
						unsigned long flags)
{

	struct thread_info *thread = current_thread_info();

	printk(KERN_INFO "post-handler thread info: flags = %x, \
		status = %d, cpu = %d, task->pid = %d\n",
		thread->flags, thread->status, thread->cpu, thread->task->pid);
}

static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	printk(KERN_INFO "fault_handler: p->addr = 0x%p, trap #%dn",
	p->addr, trapnr);
	return 0;
}


static int __init kprobes_register(void)
{
	int ret;
	kp.pre_handler = handler_pre;
	kp.post_handler = handler_post;
	kp.fault_handler = handler_fault;

	ret = register_kprobe(&kp);
	if (ret < 0) {
		printk(KERN_INFO "register_kprobe failed, returned %d\n", ret);
		return ret;
	}

	printk(KERN_INFO "Planted kprobe at %p\n", kp.addr);
	return 0;
}

static void kprobes_unregister(void)
{
	unregister_kprobe(&kp);
	return;
}

module_init(kprobes_register);
module_exit(kprobes_unregister);
MODULE_LICENSE("GPL");
