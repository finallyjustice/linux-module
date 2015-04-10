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
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

#include "ioctl-io.h"

#define KTHREAD_POOL_SIZE 3

struct kjob
{
	pid_t pid;          // pid submitted
	struct kjob *next; 
};

struct kthread_pool
{
	struct kjob *head;  // head of thread pool queue
	struct kjob *tail;  // tail of thread pool queue
	int num;            // number of threads
};

DEFINE_MUTEX(ktp_mutex);  // mutex for thread pool
struct kthread_pool ktp;  // thread pool 
static DECLARE_WAIT_QUEUE_HEAD(ktp_wq);
struct task_struct **kts_ptr;  // array of kernel threads

struct cdev cdev;
int dev_major = 50;
int dev_minor = 0;

void produce(void)
{
	struct kjob *j = (struct kjob *)kmalloc(sizeof(struct kjob), GFP_KERNEL);
	j->pid  = current->pid;
	j->next = NULL;

	mutex_lock(&ktp_mutex);
		
	// insert the job at the tail
	if(ktp.num == 0)
	{
		ktp.head = j;
		ktp.tail = j;
	}
	else
	{
		ktp.tail->next = j;
		ktp.tail = j;
	}
	ktp.num++;
	
	// notify the consumer
	wake_up_interruptible(&ktp_wq);

	mutex_unlock(&ktp_mutex);

	printk(KERN_ALERT "Job %d is submitted. current(%d)\n", current->pid, ktp.num);
}

int consume(void *data)
{
	struct kjob *nj;

	printk(KERN_ALERT "thread %d is created\n", current->pid);

	do {
		//printk(KERN_ALERT "Waiting for the new job...\n");
		wait_event_interruptible(ktp_wq, ktp.num>0);
		mutex_lock(&ktp_mutex);
		nj = NULL;
		if(ktp.num > 0)
		{
			// get the job from queue
			nj = ktp.head;
			ktp.head = ktp.head->next;
			nj->next = NULL;
			ktp.num--;

			if(ktp.num == 0)
				ktp.tail = NULL;
		}
		mutex_unlock(&ktp_mutex);

		// process the job
		if(nj != NULL)
		{
			printk(KERN_ALERT "Thread %d crocess job %d (current:%d)\n", current->pid, nj->pid, ktp.num);
			// free job
			kfree(nj);
		}

		//schedule();
	}while(!kthread_should_stop());

	return 0;
}

void pool_initialization(void)
{
	int i;

	// init the thread pool
	ktp.head = NULL;
	ktp.tail = NULL;
	ktp.num  = 0;

	// init kthreads
	kts_ptr = (struct task_struct **)kmalloc(sizeof(struct task_struct *)*KTHREAD_POOL_SIZE, GFP_KERNEL);	
	for(i=0; i<KTHREAD_POOL_SIZE; i++)
	{
		kts_ptr[i] = kthread_run(consume, NULL, "thread_pool_%d", i);
	}
}

void pool_reclaim(void)
{
	int i;
	struct kjob *head, *tmp;

	// lock before reclaiming the resource
	mutex_lock(&ktp_mutex);

	// kill all threads
	for(i=0; i<KTHREAD_POOL_SIZE; i++)
	{
		kthread_stop(kts_ptr[i]);
	}

	// free all pending jobs
	head = ktp.head;
	while(head)
	{
		tmp = head;
		head = head->next;
		kfree(tmp);
	}

	kfree(kts_ptr);

	mutex_unlock(&ktp_mutex);
}

int ioctl_dev_open(struct inode *inode, struct file *filep)
{
	//printk(KERN_ALERT "ioctl_dev_open\n");
	return 0;
}

int ioctl_dev_release(struct inode *inode, struct file *filep)
{
	//printk(KERN_ALERT "ioctl_dev_release\n");
	return 0;
}

long ioctl_dev_ioctl(struct file *filp, unsigned int ioctl, unsigned long arg)
{
	//printk(KERN_ALERT "ioctl_dev_ioctl\n");
	
	switch(ioctl)
	{
		case TZV_IOSUBMIT:
		{
			//printk(KERN_ALERT "command: SUBMIT\n");
			produce();
			break;
		}
		default:
		{
			//printk(KERN_ALERT "command: Unknown\n");
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

static int __init thread_pool_init(void)
{
	int ret;
	printk(KERN_ALERT "init the module\n");
	ret = ioctl_dev_install();
	pool_initialization();
	return ret;
}

static void __exit thread_pool_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
	ioctl_dev_uninstall();
}

module_init(thread_pool_init);
module_exit(thread_pool_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Module");
MODULE_AUTHOR("Dongli");
