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
#include <linux/mmzone.h>
#include <linux/blkdev.h>
#include <linux/list.h>
#include <linux/swap.h>
#include <linux/cpumask.h>

#define PAGE_KB 4

struct list_head *all_bdevs = (struct list_head *)0xffffffff81c3f540;
struct address_space *my_swapper_spaces = (struct address_space *)0xffffffff81c3b440;

unsigned long getMemTotal(void)
{
	return totalram_pages*PAGE_KB;
}

unsigned long getMemFree(void)
{
	return (*((unsigned long *)vm_stat+NR_FREE_PAGES))*4;
}

unsigned long getBuffers(void)
{
	unsigned long tot = 0;
	struct block_device *bdev;
	list_for_each_entry(bdev, all_bdevs, bd_list)
	{
		tot += bdev->bd_inode->i_mapping->nrpages;
	}
	return tot*4;
}

unsigned long getCached(void)
{
	unsigned long cached;
	
	unsigned long file_pages;
	unsigned long swapcache_pages;
	unsigned long bufferram;
	int i;

	file_pages = *((unsigned long *)vm_stat+NR_FILE_PAGES);

	swapcache_pages = 0;
	for(i=0; i<MAX_SWAPFILES; i++)
		swapcache_pages += my_swapper_spaces[i].nrpages;

	bufferram = getBuffers();

	cached = file_pages*4-swapcache_pages*4-bufferram;
	return cached;
}

void memory_utilization(void)
{
	unsigned long mem_usage;
	unsigned long MemTotal = getMemTotal();
	unsigned long MemFree  = getMemFree();
	unsigned long Buffers  = getBuffers();
	unsigned long Cached   = getCached();

	// the equation to compute the Memory utilization
	mem_usage = (MemTotal-MemFree-Buffers-Cached)*100/MemTotal;
	printk(KERN_ALERT "MemTotal  : %lu kB\n", MemTotal);
	printk(KERN_ALERT "MemFree   : %lu kB\n", MemFree);
	printk(KERN_ALERT "Buffers   : %lu kB\n", Buffers);
	printk(KERN_ALERT "Cached    : %lu kB\n", Cached);
	printk(KERN_ALERT "Mem Usage : %lu/100\n", mem_usage);
}

// check DECLARE_BITMAP(name,bits)
// The length of cpu_possible_mask->bits is 1 -> 64 bit
void cpu_utilization(void)
{
	//printk(KERN_ALERT "cpu_possible_mask: 0x%016lx\n", *((unsigned long *)cpu_possible_mask->bits));
	//printk(KERN_ALERT "cpu_possible_mask: 0x%016lx\n", *((unsigned long *)cpu_online_mask->bits));
	//printk(KERN_ALERT "cpu_possible_mask: 0x%016lx\n", *((unsigned long *)cpu_present_mask->bits));
	unsigned long cpu_bit = *((unsigned long *)cpu_possible_mask->bits);
	unsigned long idx = cpu_bit;
	int c = 0;
	while(idx)
	{
		printk(KERN_ALERT "CPU %d is active\n", c);
		c++;
		idx = idx >> 1;
	}
	for_each_possible_cpu(c)
	{
		printk(KERN_ALERT "CPU %d is active\n", c);
	}
}

static int __init utilization_init(void)
{
	//memory_utilization();
	cpu_utilization();
	return 0;
}

static void __exit utilization_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(utilization_init);
module_exit(utilization_exit);
