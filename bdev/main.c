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
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/hdreg.h>
#include <linux/genhd.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/blk_types.h>

static int vmem_disk_major;
module_param(vmem_disk_major, int, 0);

#define HARDSECT_SIZE 512
#define NSECTORS 1024
#define NDEVICES 4

enum {
	VMEMD_QUEUE = 0, /* Use request_queue */
	VMEMD_NOQUEUE = 1, /* Use make_request */
};

static int request_mode = VMEMD_QUEUE;
module_param(request_mode, int, 0);

#define VMEM_DISK_MINOR     16
#define KERNEL_SECTOR_SIZE  512

struct vmem_disk_dev {
	int size;                     /* Device size in sectors */
	u8 *data;                     /* The data array */
	spinlock_t lock;              /* For mutual exclusion */
	struct request_queue *queue;  /* The device request queue */
	struct gendisk *gd;           /* The gendisk structure */
};

static struct vmem_disk_dev *devices;

/* 
 * Handle an I/O request.
 */
static void vmem_disk_transfer(struct vmem_disk_dev *dev, unsigned long sector,
		unsigned long nsect, char *buffer, int write)
{
	unsigned long offset = sector*KERNEL_SECTOR_SIZE;
	unsigned long nbytes = nsect*KERNEL_SECTOR_SIZE;

	if((offset + nbytes) > dev->size) {
		printk(KERN_ALERT "Beyond-end write (%ld %ld)\n", offset, nbytes);
		return;
	}
	if(write)
		memcpy(dev->data + offset, buffer, nbytes);
	else
		memcpy(buffer, dev->data + offset, nbytes);
}

/* 
 * Transfer a single BIO.
 */
static int vmem_disk_xfer_bio(struct vmem_disk_dev *dev, struct bio *bio)
{
	struct bio_vec bvec;
	struct bvec_iter iter;
	sector_t sector = bio->bi_iter.bi_sector;

	bio_for_each_segment(bvec, bio, iter) {
		char *buffer = __bio_kmap_atomic(bio, iter);
		vmem_disk_transfer(dev, sector, bio_cur_bytes(bio) >> 9, buffer,
				bio_data_dir(bio) == WRITE);
		sector += bio_cur_bytes(bio) >> 9;
		__bio_kunmap_atomic(buffer);
	}
	return 0;
}

static int __init test_blkdev_init(void)
{
	printk(KERN_ALERT "init the module\n");
	return 0;
}

static void __exit test_blkdev_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(test_blkdev_init);
module_exit(test_blkdev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Block Device");
MODULE_AUTHOR("Dongli Zhang");

