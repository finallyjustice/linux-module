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
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/wait.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

#include "ioctl-io.h"

struct cdev cdev;
int dev_major = 50;
int dev_minor = 0;

static int global_flag = 0;
static DECLARE_WAIT_QUEUE_HEAD(my_outq);

//wait_queue_head_t my_outq;
//init_waitqueue_head(&my_outq);

int ioctl_dev_open(struct inode *inode, struct file *filep)
{
	printk(KERN_ALERT "ioctl_dev_open\n");
	return 0;
}

int ioctl_dev_release(struct inode *inode, struct file *filep)
{
	printk(KERN_ALERT "ioctl_dev_release\n");
	return 0;
}

long ioctl_dev_ioctl(struct file *filp, unsigned int ioctl, unsigned long arg)
{
	printk(KERN_ALERT "ioctl_dev_ioctl\n");
	
	switch(ioctl)
	{
		case TZV_IORD:
		{
			printk(KERN_ALERT "command: READ\n");
			if(wait_event_interruptible(my_outq, global_flag!=0))
			{
				printk(KERN_ALERT "wait_event_interruptible error!\n");
				//return -ERESTARTSYS;
			}
			printk("Read successfully!\n");
			break;
		}
		case TZV_IOWT:
		{
			printk(KERN_ALERT "command: WRITE\n");
			global_flag = 1;
			wake_up_interruptible(&my_outq);
			printk(KERN_ALERT "write successfully!\n");
			break;
		}
		default:
		{
			printk(KERN_ALERT "command: Unknown\n");
		}
	}
	return 0;
}

struct file_operations ioctl_dev_fops = {
	owner:   THIS_MODULE, 
	open:    ioctl_dev_open,    
	release: ioctl_dev_release,
	unlocked_ioctl :  ioctl_dev_ioctl,
};

int ioctl_dev_install(void)
{
	int ret;
	dev_t devno = MKDEV(dev_major, dev_minor);

	if(dev_major)  // assign static dev number
	{
		ret = register_chrdev_region(devno, 2, "tzvisor-vm");
	}
	else  // assign dynamic dev number
	{
		ret = alloc_chrdev_region(&devno, 0, 2, "tzvisor-vm");
		dev_major = MAJOR(devno);
	}

	if(ret < 0)
	{
		printk(KERN_ALERT "/dev/tzvisor-vm register failed\n");
		return ret;
	}
	else
	{
		printk(KERN_ALERT "/dev/tzvisor-vm register successful\n");
	}

	// init the cdev
	cdev_init(&cdev, &ioctl_dev_fops);
	cdev.owner = THIS_MODULE;
	cdev.ops = &ioctl_dev_fops;
	// register the char device
	cdev_add(&cdev, MKDEV(dev_major, 0), 1);

	return 0;
}

void ioctl_dev_uninstall(void)
{
	cdev_del(&cdev);
	unregister_chrdev_region(MKDEV(dev_major, dev_minor), 2);
}

static int __init wait_queue_init(void)
{
	int ret;
	printk(KERN_ALERT "init the module\n");
	ret = ioctl_dev_install();
	return ret;
}

static void __exit wait_queue_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
	ioctl_dev_uninstall();
}

module_init(wait_queue_init);
module_exit(wait_queue_exit);
