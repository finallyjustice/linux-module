#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <asm/pgtable_types.h>
#include <linux/syscalls.h>

//unsigned long **sys_call_table = (unsigned long **)0xffffffff81a001a0;
unsigned long **sys_call_table = (unsigned long **)0xffffffff81801360;
unsigned long *old_mkdir = NULL;

int make_rw(unsigned long addr)
{
	unsigned int level;
	pte_t *pte = lookup_address(addr, &level);
	if(pte->pte & ~_PAGE_RW)
		pte->pte |= _PAGE_RW;

	return 0;
}

int make_ro(unsigned long addr)
{
	unsigned int level;
	pte_t *pte = lookup_address(addr, &level);
	pte->pte &= ~_PAGE_RW;

	return 0;
}

asmlinkage long new_mkdir(const char __user *pathname, int mode)
{
	printk(KERN_ALERT "mkdir path name: %s\n", pathname);
	printk(KERN_ALERT "mkdir not working\n");
	return 0;
}

static int __init hook_init(void)
{
	printk(KERN_ALERT "Hook Init\n");
	old_mkdir = (unsigned long *)(sys_call_table[__NR_mkdir]);
	printk(KERN_ALERT "old mkdir is: %lx\n", (unsigned long)old_mkdir);

	make_rw((unsigned long)sys_call_table);
	sys_call_table[__NR_mkdir] = (unsigned long *)new_mkdir;
	make_ro((unsigned long)sys_call_table);

	return 0;
}

static void __exit hook_exit(void)
{
	make_rw((unsigned long)sys_call_table);
	sys_call_table[__NR_mkdir] = (unsigned long *)old_mkdir;
	make_ro((unsigned long)sys_call_table);
	printk(KERN_ALERT "Hook Exit\n");
}

module_init(hook_init);
module_exit(hook_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dongli");
