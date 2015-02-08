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

struct cdev cdev;
int dev_major = 50;
int dev_minor = 0;

int temp_open(struct inode *inode, struct file *filep)
{
	printk(KERN_ALERT "temp_open\n");
	return 0;
}

int temp_release(struct inode *inode, struct file *filep)
{
	printk(KERN_ALERT "temp_release\n");
	return 0;
}

ssize_t temp_read(struct file *filep, char __user *buf, size_t count, loff_t *offp)
{
	//char *my_data = (char *)vmalloc(100);
	//memset(my_data, 0, 100);
	//sprintf(my_data, "%s", "Hello World");
	//if(copy_to_user(buf, my_data, strlen(my_data)) != 0)
	//	printk(KERN_ALERT "copy_to_user failed\n");
	printk(KERN_ALERT "temp_read\n");
	printk(KERN_ALERT "taskname: %s\n", current->comm);
	printk(KERN_ALERT "timeslice: %lu\n", current->rt.time_slice);
	//vfree(my_data);
	//return strlen(my_data);
	return 0;
}

ssize_t temp_write(struct file *filep, const char __user *buf, size_t count, loff_t *offp)
{
	printk(KERN_ALERT "temp_write\n");
	return 0;
}

long temp_ioctl(struct file *filp, unsigned int ioctl, unsigned long arg)
{
	return 0;
}

struct file_operations temp_fops = {
	owner:   THIS_MODULE, 
	open:    temp_open,    
	read:    temp_read,
	write:   temp_write,
	release: temp_release,
	unlocked_ioctl : temp_ioctl,
};

int cdev_temp_install(void)
{
	int ret;
	dev_t devno = MKDEV(dev_major, dev_minor);

	if(dev_major)  // assign static dev number
	{
		ret = register_chrdev_region(devno, 2, "temp");
	}
	else  // assign dynamic dev number
	{
		ret = alloc_chrdev_region(&devno, 0, 2, "temp");
		dev_major = MAJOR(devno);
	}

	if(ret < 0)
	{
		printk(KERN_ALERT "/dev/temp register failed\n");
		return ret;
	}
	else
	{
		printk(KERN_ALERT "/dev/temp register successful\n");
	}

	// init the cdev
	cdev_init(&cdev, &temp_fops);
	cdev.owner = THIS_MODULE;
	cdev.ops = &temp_fops;
	// register the char device
	cdev_add(&cdev, MKDEV(dev_major, 0), 1);

	return 0;
}

void cdev_temp_uninstall(void)
{
	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(dev_major, 0), 1);
}

static int __init cdev_temp_init(void)
{
	int ret;
	printk(KERN_ALERT "init the module\n");
	ret = cdev_temp_install();
	return ret;
}

static void __exit cdev_temp_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
	cdev_temp_uninstall();
}

module_init(cdev_temp_init);
module_exit(cdev_temp_exit);
