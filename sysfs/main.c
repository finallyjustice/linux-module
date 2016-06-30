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
#include <linux/kobject.h>
#include <linux/sysfs.h> 

static int magic;
static struct kobject *magic_kobject;

static ssize_t magic_show(struct kobject *kobj, struct kobj_attribute *attr,
							char *buf)
{
	return sprintf(buf, "%d\n", magic);
}

static ssize_t magic_store(struct kobject *kobj, struct kobj_attribute *attr,
							char *buf, size_t count)
{
	sscanf(buf, "%du", &magic);
	return count;
}

static struct kobj_attribute magic_attribute = __ATTR(magic, 0660, magic_show,
														magic_store);

static int __init sysfs_test_init(void)
{
	int ret;
	printk(KERN_ALERT "init sysfs module\n");

	/* to create a directory into the /sys/kernel directory */
	magic_kobject = kobject_create_and_add("magic_kobject",
											kernel_kobj);

	if (!magic_kobject)
		return -ENOMEM;

	/* to create a single file attribute */
	ret = sysfs_create_file(magic_kobject, &magic_attribute.attr);
	if (ret)
		printk(KERN_ALERT "Failed to create entry in sysfs");

	return ret;
}

static void __exit sysfs_test_exit(void)
{
	/* to remove the kobject from the sysfs */
	kobject_put(magic_kobject);
	printk(KERN_ALERT "exit sysfs module\n");
}

module_init(sysfs_test_init);
module_exit(sysfs_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test sysfs");
MODULE_AUTHOR("Dongli Zhang");

