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
#include <linux/slab.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

static struct kmem_cache *my_cachep;

void init_my_cache(void)
{
	my_cachep = kmem_cache_create("my_cache", 32, 0, SLAB_HWCACHE_ALIGN, NULL);
	return;
}

void remove_my_cache(void)
{
	if(my_cachep)
		kmem_cache_destroy(my_cachep);
}

void slab_test(void)
{
	char *obj;

	printk(KERN_ALERT "Cache name: %s\n", my_cachep->name);
	printk(KERN_ALERT "Cache size: %d\n", my_cachep->size);

	obj = (char *)kmem_cache_alloc(my_cachep, GFP_KERNEL);
	if(obj == NULL)
	{
		printk(KERN_ALERT "failed to alloc slab memory\n");
		return;
	}

	memcpy(obj, "Hello World!\n\0", 14);
	printk(KERN_ALERT "msg: %s\n", obj);

	if(obj)
	{
		kmem_cache_free(my_cachep, obj);
	}
}

static int __init test_slab_init(void)
{	
	printk(KERN_ALERT "init the module\n");
	init_my_cache();
	slab_test();
	return 0;
}

static void __exit test_slab_exit(void)
{
	remove_my_cache();
	printk(KERN_ALERT "exit the module\n");
}

module_init(test_slab_init);
module_exit(test_slab_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Module");
MODULE_AUTHOR("Dongli");
