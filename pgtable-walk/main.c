/** File:		main.c
 ** Author:		Dongli Zhang
 ** Contact:	dongli.zhang0129@gmail.com
 **
 ** Copyright (C) Dongli Zhang 2013
 **
 ** This program is free software;  you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY;  without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 ** the GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program;  if not, write to the Free Software 
 ** Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/pgtable.h> 
#include <asm/page.h>

void pgtable_walk(unsigned long addr)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long page_mask;
	unsigned int level;

	phys_addr_t phys_addr;
	unsigned long offset;
	
	pgd = pgd_offset(current->mm, addr);
	printk(KERN_ALERT "pgd is : %lx\n", (unsigned long)pgd->pgd);
	printk(KERN_ALERT "pgd index: %lx\n", (unsigned long)pgd_index(addr));

	pud = pud_offset(pgd, addr);
	printk(KERN_ALERT "pud is : %lx\n", (unsigned long)pud->pud);
	printk(KERN_ALERT "pud index: %lx\n", (unsigned long)pud_index(addr));

	pmd = pmd_offset(pud, addr);
	printk(KERN_ALERT "pmd is : %lx\n", (unsigned long)pmd->pmd);
	printk(KERN_ALERT "pmd index: %lx\n", (unsigned long)pmd_index(addr));

	if(!pmd_large(*pmd))
	{
		pte = pte_offset_kernel(pmd, addr);
		printk(KERN_ALERT "pte is : %lx\n", (unsigned long)pte->pte);
		printk(KERN_ALERT "pmd index: %lx\n", (unsigned long)pte_index(addr));
		level = 2;
	}
	else
	{
		pte = (pte_t *)pmd;
		level = 1;
	}

	page_mask = page_level_mask(level);
	phys_addr = pte_pfn(*pte) << PAGE_SHIFT;
	offset    = addr & ~page_mask;
	printk("Final Phys Addr: %lx\n", (unsigned long)(phys_addr | offset));
}

static int __init multi_file_init(void)
{
	unsigned long addr; 
	void *mem;
	pte_t *pte;
	int level;

	mem = vmalloc(10);
	addr = (unsigned long)mem;
	//addr = 0xffffffff81a001a0;
	pte = lookup_address(addr, &level);

	printk(KERN_ALERT "Virt Addr: %lx\n", addr);
	printk(KERN_ALERT "Phys Addr: %lx\n", (unsigned long)slow_virt_to_phys((void *)addr));
	printk(KERN_ALERT "expected entry is : %lx, level: %u\n", (unsigned long)pte->pte, level);

	pgtable_walk(addr);
	
	printk(KERN_ALERT "init the module\n");
	return 0;
}

static void __exit multi_file_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(multi_file_init);
module_exit(multi_file_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dongli");
