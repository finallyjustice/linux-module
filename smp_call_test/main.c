#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/smp.h>

void print_smpid(void *data)
{
	int cpu = smp_processor_id();
	int master = (int) (long)data;
	printk(KERN_ALERT "master=%d, cpu=%d\n", master, cpu);
}

static int __init smp_call_test_init(void)
{
	int master = smp_processor_id();
	smp_call_function(print_smpid, (void *)(long)master, 1);
    return 0;
}

static void __exit smp_call_test_exit(void)
{
    printk(KERN_ALERT "exit the module\n");
}

module_init(smp_call_test_init);
module_exit(smp_call_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dongli Zhang");
