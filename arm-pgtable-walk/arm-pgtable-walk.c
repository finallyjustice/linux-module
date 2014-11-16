#include <linux/module.h>  
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/system.h>

void va2pa(unsigned long addr)
{
	pgd_t * pgd;
	pmd_t * pmd;
	pte_t * pte;

	pgd = pgd_offset(current->mm, addr);
	pmd = pmd_offset(pgd, addr);
	pte = pte_offset_map(pmd, addr);
	
	printk(KERN_ALERT "*pgd=0x%08lx\n", pgd_val(*pgd));
	printk(KERN_ALERT "*pmd=0x%08lx\n", pmd_val(*pmd));
	printk(KERN_ALERT "*pte=0x%08lx\n", pte_val(*pte));
	printk(KERN_ALERT "*ppte=0x%08lx\n", pte_val(pte[-PTRS_PER_PTE]));
}

static int __init pgtable_init(void)
{
	unsigned long addr;
	int test;

	printk(KERN_ALERT "Test Paging!\n");

	//addr = (unsigned long)current;
	addr = (unsigned long) &test;
	printk(KERN_ALERT "current base: 0x%08x\n", (unsigned int)addr);
	printk(KERN_ALERT "expected pa : 0x%08x\n", (unsigned int)__pa(addr));

	va2pa(addr);

	return 0;
}

static void __exit pgtable_exit(void)
{
	printk(KERN_ALERT "Remove the module\n");
}

module_init(pgtable_init);  
module_exit(pgtable_exit); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dongli Zhang");
MODULE_DESCRIPTION("i.MX53 Linux Kernel Module address translation");

