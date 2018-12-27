#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

#define NVME_VENDOR 0x8086
#define NVME_DEVICE 0xf1a6

static int __init getpci_init(void)
{
	struct pci_dev *dev = NULL;
	int count;

	dev = pci_get_device(NVME_VENDOR, NVME_DEVICE, dev);
	if (!dev) {
		pr_alert("no device found\n");
		return -EINVAL;
	}

	count = pci_msix_vec_count(dev);

	pr_alert("count = %d\n", count);

	return 0;
}

static void __exit getpci_exit(void)
{
}

MODULE_LICENSE("GPL");
module_init(getpci_init);
module_exit(getpci_exit);
