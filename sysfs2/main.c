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
static struct kobject magic_kobject;

void magic_release(struct kobject *kobject)
{
	printk(KERN_ALERT "release magic sysfs\n");
}

ssize_t magic_show(struct kobject *kobject, struct attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", magic);
}

ssize_t magic_store(struct kobject *kobject, struct attribute *attr, 
					const char *buf, size_t count)
{
	sscanf(buf, "%du", &magic);
	return count;
}

static struct attribute magic_attr = {
	.name = "magic_config",
	.mode = S_IRWXUGO,
};

static struct attribute *magic_attrs[] = {
	&magic_attr,
	NULL,
};

static struct sysfs_ops magic_sysops = {
	.show = magic_show,
	.store = magic_store,
};

static struct kobj_type ktype = {
	.release = magic_release,
	.sysfs_ops = &magic_sysops,
	.default_attrs = magic_attrs,
};

static int __init sysfs_test_init(void)
{
	printk(KERN_ALERT "init sysfs module\n");
	kobject_init_and_add(&magic_kobject, &ktype, NULL, "magic_test");
	return 0;
}

static void __exit sysfs_test_exit(void)
{
	printk(KERN_ALERT "exit sysfs module\n");
	kobject_del(&magic_kobject); 
}

module_init(sysfs_test_init);
module_exit(sysfs_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test sysfs");
MODULE_AUTHOR("Dongli Zhang");

