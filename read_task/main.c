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

/* run dmesg to see the list */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

void traverse_tasks(void)
{
	struct task_struct *pt;
	struct list_head *current_head;
	unsigned int cr3 = 0x0;
	int count = 0;

	current_head = &(current->tasks);
	list_for_each_entry(pt, current_head, tasks)
	{
		count++;
		if(pt->mm)
			cr3 = (unsigned long)(pt->mm->pgd->pgd);
		printk(KERN_ALERT "[process %d]: %s - %d - 0x%016x\n",
				count, pt->comm, pt->pid, cr3);
	}
}

static int __init read_task_init(void)
{
	printk(KERN_ALERT "init the module\n");
	traverse_tasks();
	return 0;
}

static void __exit read_task_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(read_task_init);
module_exit(read_task_exit);
