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
#include <linux/version.h>
#include <linux/list.h>

// For other detail, please check print_modules(void) in kernel/module.c

struct module *m = &__this_module;
struct list_head *modules = (struct list_head *)0xffffffff81c34bb0;

void list_modules_from_current(void)
{
	int tot = 0;
	struct module *mod;
	// start from previous one. prev is list_head, not struct module!
	list_for_each_entry(mod, m->list.prev, list)
	{
		// module_core: the actual code+data, vfree'd on unload
		printk(KERN_ALERT "%s, 0x%016lx\n", mod->name, (unsigned long)mod->module_core);
		tot++;
	}
	printk(KERN_ALERT "TOTAL: %d\n", tot);
}

void list_modules_from_head(void)
{
	int tot = 0;
	struct module *mod;
	list_for_each_entry(mod, modules, list)
	{
		// module_core: the actual code+data, vfree'd on unload
		printk(KERN_ALERT "%s, 0x%016lx\n", mod->name, (unsigned long)mod->module_core);
		tot++;
	}
	printk(KERN_ALERT "TOTAL: %d\n", tot);
}

void list_modules_from_scratch(void)
{
	int tot = 0;
	struct module *mod;

	for(mod = list_entry(modules->next, struct module, list); 
		mod->list.next != modules->next; 
		mod = list_entry(mod->list.next, struct module, list))
	{
		// module_core: the actual code+data, vfree'd on unload
		printk(KERN_ALERT "%s, 0x%016lx\n", mod->name, (unsigned long)mod->module_core);
		tot++;
	}
	printk(KERN_ALERT "TOTAL: %d\n", tot);
}

static int __init read_module_init(void)
{
	printk(KERN_ALERT "read_module_from_current:\n");
	list_modules_from_current();
	printk(KERN_ALERT "\n");

	printk(KERN_ALERT "read_module_from_head:\n");
	list_modules_from_head();
	printk(KERN_ALERT "\n");

	printk(KERN_ALERT "read_module_from_scratch:\n");
	list_modules_from_scratch();

	return 0;
}

static void __exit read_module_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(read_module_init);
module_exit(read_module_exit);
