#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

static int __init kvm_track_init(void)
{
	u64 gpa;
	int i;
	char *p = kmalloc(4096, GFP_KERNEL);

	p[0] = 'x';

	gpa = __pa(p);

	pr_alert("kvm_track: gpa = 0x%016llx\n", gpa);

	__asm__ volatile("mov $10, %%rax\n\t"
					 "mov %0, %%rbx\n\t"
					 "vmcall\n\t"
					 :
					 : "m"(gpa)
					 : "%rax", "%rbx");

	for (i = 0; i < 30; i++) {
		p[i] = 'a';
	}

	return 0;
}

static void __exit kvm_track_exit(void)
{
}

module_init(kvm_track_init);
module_exit(kvm_track_exit);

MODULE_LICENSE("GPL");
