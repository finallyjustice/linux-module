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
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>

#include <asm/unistd.h>
#include <asm/uaccess.h>

#define FILE_PATH_RD "/file_rd"
#define FILE_PATH_WR "/file_wr"

struct file *file = NULL;
struct inode *inode = NULL;
int *file_buf;
loff_t fsize;
mm_segment_t old_fs;

static loff_t kernel_file_size(struct file *file)
{
	inode = file->f_dentry->d_inode;
	fsize = inode->i_size;
	printk(KERN_ALERT "size=%d\n", (int)fsize);
	return fsize;
}

static int __init file_operation_init(void)
{
	char *buf;
	loff_t *pos;
	//printk(KERN_ALERT "init the module\n");

	file = filp_open(FILE_PATH_RD, O_RDWR|O_CREAT, 0);
	if(IS_ERR(file))
	{
		printk("Open file %s failed\n", FILE_PATH_RD);
		return 0;
	}
	kernel_file_size(file);

	buf = (char *)kmalloc(fsize+10, GFP_KERNEL);
	pos = &(file->f_pos);
	pos = 0;
	vfs_read(file, buf, fsize, pos);
	printk(KERN_ALERT "content: %s\n", buf);
	return 0;
}

static void __exit file_operation_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(file_operation_init);
module_exit(file_operation_exit);
