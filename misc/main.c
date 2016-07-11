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
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

static long misc_test_ioctl(struct file *filp, unsigned int cmd, 
								unsigned long arg)
{
	printk(KERN_ALERT "misc_test_ioctl\n");
	return 0;
}

static int misc_test_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "misc_test_open\n");
	return 0;
}

static int misc_test_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "misc_test_release\n");
	return 0;
}

static ssize_t misc_test_read(struct file *filp, char __user *buf,
								size_t count, loff_t *f_pos)
{
	printk(KERN_ALERT "misc_test_read\n");
	return 0;
}

static struct file_operations misc_test_fops = {
	.owner			 = THIS_MODULE,
	.read			 = misc_test_read,
	.unlocked_ioctl	 = misc_test_ioctl,
	.open			 = misc_test_open,
	.release		 = misc_test_release,
};

static struct miscdevice misc_test = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "misc_test",
	.fops  = &misc_test_fops,
};

static int __init misc_test_init(void)
{
	int ret = misc_register(&misc_test);
	printk(KERN_ALERT "init misc module\n");
	return ret;
}

static void __exit misc_test_exit(void)
{
	misc_deregister(&misc_test);
	printk(KERN_ALERT "exit misc module\n");
}

module_init(misc_test_init);
module_exit(misc_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test misc");
MODULE_AUTHOR("Dongli Zhang");
