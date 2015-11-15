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
#include <linux/module.h>       /* MODULE_LIcENSE */ 
#include <linux/kernel.h>       /* printk, pr_info */
#include <linux/errno.h>        /* EINVAL, EAGAIN, etc */ 
#include <linux/err.h>          /* IS_ERR */
#include <linux/fb.h>           /* FB header file */
#include <linux/semaphore.h>    /* init_MUTEX APIs */
#include <linux/mm.h>           /* vm_area_struct */
#include <linux/dma-mapping.h>  /* DMA APIs */
#include <linux/delay.h>        /* mdelay, msleep */
#include <linux/hrtimer.h>      
#include <linux/time.h>         /* struct timespec */

#define TIME_INTERVAL 2000 /* Time out setting, 2 seconds */
#define KER_PRINT(fmt, ...) printk("<ker-driver>"fmt, ##__VA_ARGS__);
static struct hrtimer vibe_timer;
static struct work_struct vibe_work;
static ktime_t ktime;

static void vibe_work_func(struct work_struct *word)
{
	KER_PRINT("vibe_work_func: msleep(50)\n");
	msleep(50);

	ktime = ktime_set(TIME_INTERVAL / 1000, (TIME_INTERVAL % 1000) * 1000000);
	hrtimer_start(&vibe_timer, ktime, HRTIMER_MODE_REL);
}

static enum hrtimer_restart vibrator_timer_func(struct hrtimer *timer)
{
	KER_PRINT("vibrator_timer_func\n");
	schedule_work(&vibe_work);
	return HRTIMER_NORESTART;
}

static int __init test_hrtimer_init(void)
{
	KER_PRINT("ker_driver_init\n");
	hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	vibe_timer.function = vibrator_timer_func;

	ktime = ktime_set(TIME_INTERVAL / 1000, (TIME_INTERVAL % 1000) * 1000000);
	hrtimer_start(&vibe_timer, ktime, HRTIMER_MODE_REL);

	INIT_WORK(&vibe_work, vibe_work_func);

	printk(KERN_ALERT "init the module\n");
	return 0;
}

static void __exit test_hrtimer_exit(void)
{
	hrtimer_cancel(&vibe_timer);
	printk(KERN_ALERT "exit the module\n");
}

module_init(test_hrtimer_init);
module_exit(test_hrtimer_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test hrtimer");
MODULE_AUTHOR("Dongli Zhang");
