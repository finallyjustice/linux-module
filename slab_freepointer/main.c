#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/slub_def.h>

static int __init bug_mod_init(void)
{
	void *object, *base;
	struct page *page;
	int i;

	object = kmalloc(128, GFP_KERNEL);
	page = virt_to_head_page(object);
	base  = page_address(page);

	pr_alert("debug: slab=%s, inuse=%d, objects=%d, object=0x%016llx, base=0x%016llx\n",
		 page->slab_cache->name, page->inuse, page->objects, (u64) object, (u64) base);

	base = object;
	for (i = 0; i < 6; i++) {
		u64 *fp = (u64 *)(base + page->slab_cache->offset);
		pr_alert("debug (%d/6): current=0x%016llx, fp=0x%016llx, next=0x%016llx\n",
				 i, (u64) base, (u64) fp, *fp);
		base = (u64 *)(*fp);
		if (!base)
			break;
	}

	if (base != 0) {
		u64 *fp = (u64 *)(base + page->slab_cache->offset);
		*fp = 0x12345678;
	}
	
	kmalloc(128, GFP_KERNEL);
	kmalloc(128, GFP_KERNEL);

	return 0;
}

static void __exit bug_mod_exit(void)
{
}

module_init(bug_mod_init);
module_exit(bug_mod_exit);

MODULE_LICENSE("GPL");
