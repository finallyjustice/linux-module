#include <linux/mm.h> 
#include <linux/mmzone.h> 
#include <linux/blkdev.h> 
#include <linux/list.h> 
#include <linux/swap.h> 
#include <linux/cpumask.h> 
#include <linux/kernel_stat.h>

// get the user cpu time for cpu c
unsigned long cpu_user_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_USER];
}

// get the nice cpu time for cpu c
unsigned long cpu_nice_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_NICE];
}

// get the system cpu time for cpu c
unsigned long cpu_sys_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_SYSTEM];
}

// get the idle cpu time for cpu c 
unsigned long cpu_idle_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_IDLE];
}

// get the iowait cpu time for cpu c
unsigned long cpu_iowait_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_IOWAIT];
}

// get the hardware irq cpu time for cpu c
unsigned long cpu_hirq_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_IRQ];
}

// get the soft irq cpu time for cpu c
unsigned long cpu_sirq_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_SOFTIRQ];
}

// get the steal cpu time for cpu c
unsigned long cpu_steal_time(int c)
{
	struct kernel_cpustat *base = (struct kernel_cpustat *)((unsigned long)__per_cpu_offset[c]+(unsigned long)&kernel_cpustat);
	return base->cpustat[CPUTIME_STEAL];
}
