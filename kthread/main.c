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
#include <linux/kthread.h>
#include <linux/delay.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

struct task_struct *tsk;

int thread_function(void *data)
{
	int time_count = 0;
	do {
		printk(KERN_ALERT "thread_function %d times\n", ++time_count);
		schedule();
		//msleep(1000);
	//}while(!kthread_should_stop() && time_count<=30);
	}while(!kthread_should_stop());

	return time_count;
}

static int __init test_kthread_init(void)
{
	printk(KERN_ALERT "init the module\n");

	tsk = kthread_run(thread_function, NULL, "mythread");
	if(IS_ERR(tsk))
	{
		printk(KERN_ALERT "create kthread failed!\n");
	}
	else
	{
		printk(KERN_ALERT "create kthread ok\n");
	}

	return 0;
}

static void __exit test_kthread_exit(void)
{
	printk(KERN_ALERT "exit the module\n");

	if(!IS_ERR(tsk))
	{
		int ret = kthread_stop(tsk);
		printk(KERN_ALERT "thread function has run %ds\n", ret);
	}
}

module_init(test_kthread_init);
module_exit(test_kthread_exit);
