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
#include <linux/time.h>
#include <linux/kernel.h>

static int __init gettime_init(void)
{
	printk(KERN_ALERT "init the module\n");

	struct timespec ts;
	ts =  current_kernel_time();
	printk(KERN_ALERT "ts.tv_sec:%ld,ts.tv_nsec:%ld\n",ts.tv_sec,ts.tv_nsec);
	
	return 0;
}

static void __exit gettime_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(gettime_init);
module_exit(gettime_exit);
