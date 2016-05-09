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
#include <linux/list.h>
#include <linux/bio.h>
#include <linux/device-mapper.h>

struct wrap_dm_target {
	struct dm_dev *dev;
	sector_t start;
};

static int wrap_target_map(struct dm_target *ti, struct bio *bio)
{
	struct wrap_dm_target *mdt = (struct wrap_dm_target *)ti->private;

	bio->bi_bdev = mdt->dev->bdev;
	if ((bio->bi_rw & READ) == WRITE)
		printk(KERN_ALERT "dm-wrap: this is a read request\n");
	else
		printk(KERN_ALERT "dm-wrap: this is a write request\n");

	submit_bio(bio->bi_rw, bio);
	return DM_MAPIO_SUBMITTED;
}

static int
wrap_target_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	struct wrap_dm_target *mdt;
	unsigned long start;

	if (argc != 2) {
		printk(KERN_ALERT "dm-wrap: invalid number of arg\n");
		ti->error = "invalid number of arg";
		return -EINVAL;
	}

	mdt = kmalloc(sizeof(struct wrap_dm_target), GFP_KERNEL);

	if (mdt == NULL) {
		printk(KERN_ALERT "dm-wrap: mdt is NULL\n");
		ti->error = "mdt is NULL";
		return -ENOMEM;
	}

	if (sscanf(argv[1], "%lu", &start) != 1) {
		printk(KERN_ALERT "dm-wrap: invalid start sector\n");
		ti->error = "invalid start sector";
		goto bad;
	}

	mdt->start = (sector_t)start;

	if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &mdt->dev)) {
		printk(KERN_ALERT "dm-wrap: device lookup failed\n");
		ti->error = "device lookup failed";
		goto bad;
	}

	ti->private = mdt;
	return 0;

bad:
	kfree(mdt);
	return -EINVAL;
}

static void wrap_target_dtr(struct dm_target *ti)
{
	struct wrap_dm_target *mdt = (struct wrap_dm_target *)ti->private;
	dm_put_device(ti, mdt->dev);
	kfree(mdt);
}

static struct target_type wrap_target = {
	.name = "wrap_target",
	.version = {1, 0, 0},
	.module = THIS_MODULE,
	.ctr = wrap_target_ctr,
	.dtr = wrap_target_dtr,
	.map = wrap_target_map,
};

static int __init dm_test_init(void)
{
	int result;
	printk(KERN_ALERT "dm-wrap: init the module\n");

	result = dm_register_target(&wrap_target);
	if (result < 0)
		printk(KERN_ALERT "dm-wrap: failed to register target\n");

	return 0;
}

static void __exit dm_test_exit(void)
{
	dm_unregister_target(&wrap_target);
	printk(KERN_ALERT "dm-wrap: exit the module\n");
}

module_init(dm_test_init);
module_exit(dm_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Device Mapper");
MODULE_AUTHOR("Dongli Zhang");
