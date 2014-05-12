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
#include <linux/proc_fs.h>

static int test_read_proc(char *buf, char **start, off_t offset,
							int size, int *eof, void *data)
{
	char *str = "Hello World!\n";
	int len = strlen(str);
	strcpy(buf, str);
	return len;
}

static int __init test_proc_init(void)
{
	printk(KERN_ALERT "init the module\n");

	if(create_proc_read_entry("my_test", 0, NULL,
				       test_read_proc, NULL) == 0)
	{
		printk(KERN_ALERT "Failed to register under /proc\n");
		return -ENOMEM;
	}

	return 0;
}

static void __exit test_proc_exit(void)
{
	remove_proc_entry("my_test", NULL);
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(test_proc_init);
module_exit(test_proc_exit);
