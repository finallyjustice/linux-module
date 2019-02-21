#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

#define NVME_VENDOR 0x8086
#define NVME_DEVICE 0xf1a6

static noinline void kmalloc_oob_right(void)
{
	char *ptr;
	size_t size = 123;

	pr_info("out-of-bounds to right\n");
	ptr = kmalloc(size, GFP_KERNEL);
	if (!ptr) {
		pr_err("Allocation failed\n");
		return;
	}

	ptr[size] = 'x';
	kfree(ptr);
}

static int __init kasan_test_init(void)
{
	kmalloc_oob_right();
	return 0;
}

static void __exit kasan_test_exit(void)
{
}

MODULE_LICENSE("GPL");
module_init(kasan_test_init);
module_exit(kasan_test_exit);
