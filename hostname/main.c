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
#include <linux/utsname.h>

// __NEW_UTS_LEN is 64

static int __init hostname_init(void)
{
	printk(KERN_ALERT "init the module\n");
	printk(KERN_ALERT "sysname    : %s\n", init_task.nsproxy->uts_ns->name.sysname);
	printk(KERN_ALERT "nodename   : %s\n", init_task.nsproxy->uts_ns->name.nodename);
	printk(KERN_ALERT "release    : %s\n", init_task.nsproxy->uts_ns->name.release);
	printk(KERN_ALERT "version    : %s\n", init_task.nsproxy->uts_ns->name.version);
	printk(KERN_ALERT "machine    : %s\n", init_task.nsproxy->uts_ns->name.machine);
	printk(KERN_ALERT "domainname : %s\n", init_task.nsproxy->uts_ns->name.domainname);
	return 0;
}

static void __exit hostname_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(hostname_init);
module_exit(hostname_exit);
