#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#define SECOND_MAJOR 248

static int second_major = SECOND_MAJOR;

struct second_dev {
	struct cdev cdev;
	atomic_t counter;
	struct timer_list s_timer;
};

static struct second_dev *second_devp;

static void second_timer_handler(unsigned long arg) 
{
	mod_timer(&second_devp->s_timer, jiffies+HZ);
	atomic_inc(&second_devp->counter);
	printk(KERN_ALERT "current jiffies is %ld\n", jiffies);
}

static int second_open(struct inode *inode, struct file *filp)
{
	init_timer(&second_devp->s_timer);
	second_devp->s_timer.function = &second_timer_handler;
	second_devp->s_timer.expires = jiffies+HZ;

	add_timer(&second_devp->s_timer);
	atomic_set(&second_devp->counter, 0);
	return 0;
}

static int second_release(struct inode *inode, struct file *filp)
{
	del_timer(&second_devp->s_timer);
	return 0;
}

static ssize_t second_read(struct file *filp, char __user *buf, size_t count,
		                   loff_t *ppos)
{
	int counter;

	counter = atomic_read(&second_devp->counter);
	if (put_user(counter, (int *)buf))
		return -EFAULT;
	else
		return sizeof(unsigned int);
}

static const struct file_operations second_fops = {
	.owner = THIS_MODULE,
	.open = second_open,
	.release = second_release,
	.read = second_read,
};

static void second_setup_cdev(struct second_dev *dev, int index)
{
	int err, devno = MKDEV(second_major, index);

	cdev_init(&dev->cdev,  &second_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk(KERN_ALERT "Failed to add second device\n");
}

static int __init timer_cdev_init(void)
{
	int ret;
	dev_t devno = MKDEV(second_major, 0);

	if (second_major)
		ret = register_chrdev_region(devno, 1, "second");
	else {
		ret = alloc_chrdev_region(&devno, 0, 1, "second");
		second_major = MAJOR(devno);
	}
	if (ret < 0)
		return ret;

	second_devp = kzalloc(sizeof(*second_devp), GFP_KERNEL);
	if (!second_devp) {
		ret = -ENOMEM;
		goto fail_malloc;
	}

	second_setup_cdev(second_devp, 0);

	printk(KERN_ALERT "init the module\n");
	return 0;

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void __exit timer_cdev_exit(void)
{
	cdev_del(&second_devp->cdev);
	kfree(second_devp);
	unregister_chrdev_region(MKDEV(second_major, 0), 1);
	printk(KERN_ALERT "exit the module\n");
}

module_init(timer_cdev_init);
module_exit(timer_cdev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Block Device");
MODULE_AUTHOR("Dongli Zhang");

