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
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

static struct tasklet_struct my_tasklet;

static void my_tasklet_handler(unsigned long data)
{
	printk(KERN_ALERT "This is the tasklet handler\n");
}

static int __init test_tasklet_init(void)
{
	printk(KERN_ALERT "init the module\n");
	tasklet_init(&my_tasklet, my_tasklet_handler, 0);
	tasklet_schedule(&my_tasklet);
	return 0;
}

static void __exit test_tasklet_exit(void)
{
	tasklet_kill(&my_tasklet);
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(test_tasklet_init);
module_exit(test_tasklet_exit);
