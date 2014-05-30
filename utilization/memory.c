#include <linux/mm.h> 
#include <linux/mmzone.h> 
#include <linux/blkdev.h> 
#include <linux/list.h> 
#include <linux/swap.h> 
#include <linux/cpumask.h> 
#include <linux/kernel_stat.h>

#define PAGE_KB 4

struct list_head *all_bdevs = (struct list_head *)0xffffffff81c3f540;
struct address_space *my_swapper_spaces = (struct address_space *)0xffffffff81c3b440;

unsigned long getMemTotal(void)
{
	return totalram_pages*PAGE_KB;
}

unsigned long getMemFree(void)
{
	return (*((unsigned long *)vm_stat+NR_FREE_PAGES))*4;
}

unsigned long getBuffers(void)
{
	unsigned long tot = 0;
	struct block_device *bdev;
	list_for_each_entry(bdev, all_bdevs, bd_list)
	{
		tot += bdev->bd_inode->i_mapping->nrpages;
	}
	return tot*4;
}

unsigned long getCached(void)
{
	unsigned long cached;
	
	unsigned long file_pages;
	unsigned long swapcache_pages;
	unsigned long bufferram;
	int i;

	file_pages = *((unsigned long *)vm_stat+NR_FILE_PAGES);

	swapcache_pages = 0;
	for(i=0; i<MAX_SWAPFILES; i++)
		swapcache_pages += my_swapper_spaces[i].nrpages;

	bufferram = getBuffers();

	cached = file_pages*4-swapcache_pages*4-bufferram;
	return cached;
}
