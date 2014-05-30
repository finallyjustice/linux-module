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
#include <linux/kernel_stat.h>

#include "memory.h"
#include "cpu.h"

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
	
	unsigned long user   = 0;
	unsigned long nice   = 0;
	unsigned long sys    = 0;
	unsigned long idle   = 0;
	unsigned long iowait = 0;
	unsigned long hirq   = 0;
	unsigned long sirq   = 0;
	unsigned long steal  = 0;
	unsigned long tot    = 0;

	int c = 0;
	while(idx)
	{
		user   += cpu_user_time(c);
		nice   += cpu_nice_time(c);
		sys    += cpu_sys_time(c);
		idle   += cpu_idle_time(c);
		iowait += cpu_iowait_time(c);
		hirq   += cpu_hirq_time(c);
		sirq   += cpu_sirq_time(c);
		steal  += cpu_steal_time(c);

		printk(KERN_ALERT "user: %lu\n", cpu_user_time(c));
		printk(KERN_ALERT "nice: %lu\n", cpu_nice_time(c));
		printk(KERN_ALERT "sys: %lu\n", cpu_sys_time(c));
		printk(KERN_ALERT "idle: %lu\n", cpu_idle_time(c));
		printk(KERN_ALERT "iowait: %lu\n", cpu_iowait_time(c));
		printk(KERN_ALERT "hi: %lu\n", cpu_hirq_time(c));
		printk(KERN_ALERT "si: %lu\n", cpu_sirq_time(c));
		printk(KERN_ALERT "st: %lu\n", cpu_steal_time(c));

		c++;
		idx = idx >> 1;
		// probably convert with cputime64_to_clock_t
	}

	tot = user+nice+sys+idle+iowait+hirq+sirq+steal;
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
