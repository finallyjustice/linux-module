#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/moduleparam.h>
#include <linux/blkpg.h>
#include <linux/bio.h>
#include <linux/buffer_head.h>
#include <linux/mempool.h>
#include <linux/slab.h>
#include <linux/idr.h>
#include <linux/hdreg.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/blkdev.h>

#define NUM_BUCKETS 64
#define MASK_BUCKETS (NUM_BUCKETS - 1)

long local_names_addr;
module_param(local_names_addr, ulong, 0644);

char *local_dm_name;
module_param(local_dm_name, charp, 0644);

static struct list_head *local_names;

/*
 * Work processed by per-device workqueue.
 */
struct mapped_device {
	struct rw_semaphore io_lock;
	struct mutex suspend_lock;
	rwlock_t map_lock;
	atomic_t holders;
	atomic_t open_count;

	unsigned long flags;

	struct request_queue *queue;
	unsigned type;
	/* Protect queue and type against concurrent access. */
	struct mutex type_lock;

	struct gendisk *disk;
	char name[16];

	void *interface_ptr;

	/*
	 * A list of ios that arrived while we were suspended.
	 */
	atomic_t pending[2];
	wait_queue_head_t wait;
	struct work_struct work;
	struct bio_list deferred;
	spinlock_t deferred_lock;

	/*
	 * Processing queue (flush)
	 */
	struct workqueue_struct *wq;

	/*
	 * The current mapping.
	 */
	struct dm_table *map;

	/*
	 * io objects are allocated from here.
	 */
	mempool_t *io_pool;
	mempool_t *tio_pool;

	struct bio_set *bs;

	/*
	 * Event handling.
	 */
	atomic_t event_nr;
	wait_queue_head_t eventq;
	atomic_t uevent_seq;
	struct list_head uevent_list;
	spinlock_t uevent_lock; /* Protect access to uevent_list */

	/*
	 * freeze/thaw support require holding onto a super block
	 */
	struct super_block *frozen_sb;
	struct block_device *bdev;

	/* forced geometry settings */
	struct hd_geometry geometry;

	/* For saving the address of __make_request for request based dm */
	make_request_fn *saved_make_request_fn;

	/* sysfs handle */
	struct kobject kobj;

	/* zero-length flush that will be cloned and submitted to targets */
	struct bio flush_bio;
};

struct hash_cell {
	struct list_head name_list;
	struct list_head uuid_list;

	char *name;
	char *uuid;
	struct mapped_device *md;
	struct dm_table *new_map;
};


/*-----------------------------------------------------------------
 * Hash function:
 * We're not really concerned with the str hash function being
 * fast since it's only used by the ioctl interface.
 *---------------------------------------------------------------*/
static unsigned int hash_str(const char *str)
{
	const unsigned int hash_mult = 2654435387U;
	unsigned int h = 0;

	while (*str)
		h = (h + (unsigned int) *str++) * hash_mult;

	return h & MASK_BUCKETS;
}

/*-----------------------------------------------------------------
 * Code for looking up a device by name
 *---------------------------------------------------------------*/
static struct hash_cell *__get_name_cell(const char *str)
{
	struct hash_cell *hc;
	unsigned int h = hash_str(str);

	list_for_each_entry (hc, local_names + h, name_list)
		if (!strcmp(hc->name, str)) {
			return hc;
		}

	return NULL;
}

/* dmnfs is always bio based */
static int dm_request_based(struct mapped_device *md)
{
	return blk_queue_stackable(md->queue);
}

static void dump_spec_dm(void)
{
	struct hash_cell *cell;
	struct mapped_device *md;

	cell = __get_name_cell(local_dm_name);
	if (cell == NULL) {
		printk(KERN_ALERT "dump-dm: cell is NULL\n");
		return;
	}

	md = cell->md;
	if (dm_request_based(md))
		printk(KERN_ALERT "dump-dm: request basded\n");
	else
		printk(KERN_ALERT "dump-dm: bio based\n");
	printk(KERN_ALERT "dump-dm: name %s, 0x%016lx, 0x%016lx\n", cell->name, md->queue, md->disk->queue);
}

static int __init dump_dm_init(void)
{
	local_names = (struct list_head *) local_names_addr;
	printk(KERN_ALERT "dump-dm: init the module "
			"local_names_addr=0x%016lx," 
			"local_dm_name=%s\n", 
			local_names_addr, local_dm_name);
	dump_spec_dm();

	return 0;
}

static void __exit dump_dm_exit(void)
{
	printk(KERN_ALERT "dump-dm: exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(dump_dm_init);
module_exit(dump_dm_exit);
