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
#include <linux/timer.h>

#define TIME_SLEEP 1000 /* in msec. 1000 means 1 second */

static struct timer_list timer_s;
static struct workqueue_struct *wq; // to avoid degrading performance, we put job into workqueue

static void my_job_handler(struct work_struct *w)
{
	printk(KERN_ALERT "This is the job function, add your job here\n");
}
static DECLARE_DELAYED_WORK(my_job, my_job_handler);

static void my_timer_handler(unsigned long data)
{
	unsigned long onesec;

	//printk("This is my timer handler\n");
	// add job into queue
	onesec = msecs_to_jiffies(1000);
	queue_delayed_work(wq, &my_job, onesec);
	// re-trigger the timer
	if(mod_timer(&timer_s, jiffies+msecs_to_jiffies(TIME_SLEEP)))
		printk("Failed to continue the timer\n");
}

static int __init test_timer_init(void)
{
	printk(KERN_ALERT "init the module\n");

	// setup workqueue
	wq = create_singlethread_workqueue("timer_workqueue");
	// setup the timer
	setup_timer(&timer_s, my_timer_handler, 0);
	// mod timer only triggers once. 
	// we need to re-trigger it everytime
	if(mod_timer(&timer_s, jiffies+msecs_to_jiffies(TIME_SLEEP)))
	{
		printk(KERN_ALERT "Failed to setup timer\n");
		return -ECANCELED;
	}
	printk(KERN_ALERT "setup timer successfully\n");

	return 0;
}

static void __exit test_timer_exit(void)
{
	if(wq)
		destroy_workqueue(wq);
	del_timer(&timer_s);
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(test_timer_init);
module_exit(test_timer_exit);
