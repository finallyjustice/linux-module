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

char cpu_oem[16];

static int __init testvmm_init(void)
{
	printk(KERN_ALERT "init the module\n");

	asm("xor %%eax, %%eax\n"
		"cpuid\n"
		"mov %%ebx, cpu_oem+0\n"
		"mov %%edx, cpu_oem+4\n"
		"mov %%ecx, cpu_oem+8\n"
		::: "ax", "bx", "cx", "dx");

	printk(KERN_ALERT "processor is %s\n", cpu_oem);

	return 0;
}

static void __exit testvmm_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(testvmm_init);
module_exit(testvmm_exit);
