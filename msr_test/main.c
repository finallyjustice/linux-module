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

static int __init msr_test_init(void)
{
	unsigned long sysenter_eip;
	rdmsrl(MSR_IA32_SYSENTER_EIP, sysenter_eip);
	printk(KERN_ALERT "MSR_IA32_SYSENTER_EIP: 0x%016lx\n", sysenter_eip);
	wrmsrl(MSR_IA32_SYSENTER_EIP, sysenter_eip);

	return 0;
}

static void __exit msr_test_exit(void)
{
	printk(KERN_ALERT "exit MSR module\n");
}

module_init(msr_test_init);
module_exit(msr_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test MSR");
MODULE_AUTHOR("Dongli Zhang");

