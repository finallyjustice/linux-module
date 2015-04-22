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
#include <linux/interrupt.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

int irq;
char *interface;

module_param(irq, int, 0644);
module_param(interface, charp, 0644);

irqreturn_t irq_handle_function(int irq, void *device_id)
{
	static int count = 1;
	printk(KERN_ALERT "[%d] receive the irq at %ld...\n", count, jiffies);
	count++;
	return IRQ_NONE;
}

static int __init test_irq_init(void)
{	
	printk(KERN_ALERT "init the module\n");
	if(request_irq(irq, irq_handle_function, IRQF_SHARED, interface, (void*)&irq))
	{
		printk(KERN_ALERT "failed to register irq\n");
		return -EIO;
	}

	printk(KERN_ALERT "interface=%s, irq=%d\n", interface, irq);
	return 0;
}

static void __exit test_irq_exit(void)
{
	free_irq(irq, &irq);
	printk(KERN_ALERT "exit the module\n");
}

module_init(test_irq_init);
module_exit(test_irq_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Module");
MODULE_AUTHOR("Dongli");
