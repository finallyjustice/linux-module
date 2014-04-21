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
#include "lib.h" // include/lib.h

static int __init multi_file_init(void)
{
	printk(KERN_ALERT "init the module\n");
	print_value(); // This is from lib file (lib.c).
	return 0;
}

static void __exit multi_file_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(multi_file_init);
module_exit(multi_file_exit);
