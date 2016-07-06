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

static int __init usermodehelper_test_init(void)
{
	int ret = 0;
	char cmd_path[] = "/bin/touch";
	char *cmd_argv[] = {cmd_path, "/tmp/usermodehelper.txt", NULL};
	char *cmd_envp[] = {"PATH=/sbin:/bin:/usr/bin", NULL};

	ret = call_usermodehelper(cmd_path, cmd_argv, cmd_envp, UMH_WAIT_PROC);

	printk(KERN_ALERT "init usermodehelper_test module\n");

	return 0;
}

static void __exit usermodehelper_test_exit(void)
{
	printk(KERN_ALERT "exit usermodehelper_test module\n");
}

module_init(usermodehelper_test_init);
module_exit(usermodehelper_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test call_usermodehelper");
MODULE_AUTHOR("Dongli Zhang");

