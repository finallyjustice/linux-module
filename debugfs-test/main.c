#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>

static struct dentry *d_test_debug;

static int swiotlb_usage_show(struct seq_file *m, void *v)
{
	seq_printf(m, "test usage\n");
	return 0;
}

static int swiotlb_usage_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, swiotlb_usage_show, NULL);
}

static const struct file_operations swiotlb_usage_fops = {
	.open		= swiotlb_usage_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init debugfs_test_init(void)
{
	d_test_debug = debugfs_create_dir("swiotlb", NULL);
	if (!d_test_debug) {
		pr_warning("Could not create 'swiotlb' debugfs directory\n");
		return -ENOMEM;
	}

	debugfs_create_file("usage", 0600, d_test_debug, NULL, &swiotlb_usage_fops);

	return 0;
}

static void __exit debugfs_test_exit(void)
{
	if (!IS_ERR_OR_NULL(d_test_debug))
		debugfs_remove_recursive(d_test_debug);
}

module_init(debugfs_test_init);
module_exit(debugfs_test_exit);

MODULE_LICENSE("GPL");
