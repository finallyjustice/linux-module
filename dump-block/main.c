#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>

unsigned long local_all_bdevs_addr;
struct list_head *local_all_bdevs = NULL;
module_param(local_all_bdevs_addr, ulong, 0644);

static int __init dump_block_init(void)
{
	struct block_device *bdev;
	struct gendisk *gd;
	int c = 0;

	local_all_bdevs = (struct list_head *)local_all_bdevs_addr;
	list_for_each_entry(bdev, local_all_bdevs, bd_list) {
		gd = bdev->bd_disk;
		printk(KERN_ALERT "dump-block: %s(%d): (%d, %d)\n",
				gd->disk_name, c,
				MAJOR(bdev->bd_dev), MINOR(bdev->bd_dev));
		c++;
	}

	return 0;
}

static void __exit dump_block_exit(void)
{
	printk(KERN_ALERT "dump-block: exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(dump_block_init);
module_exit(dump_block_exit);
