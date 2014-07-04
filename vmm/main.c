/** File:		main.c
 ** Author:		Dongli Zhang
 ** Contact:	dongli.zhang0129@gmail.com
 **
 ** Copyright (C) Dongli Zhang 2013
 **
 ** This program is free software;  you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY;  without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 ** the GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program;  if not, write to the Free Software 
 ** Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/smp.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/io.h>

#define N_ARENAS      11		// number of 64KB memory allocations
#define ARENA_LENGTH  (64<<10)  // size of each allocated memory-arena
#define MSR_VMX_CAPS  0x480     // index for VMX-Capabilities MSRs
#define LEGACY_REACH  0x110000  // end of 'real-addressible' memory

#define PAGE_DIR_OFFSET 0x2000
#define PAGE_TBL_OFFSET 0x3000
#define IDT_KERN_OFFSET 0x4000
#define GDT_KERN_OFFSET 0x4800
#define LDT_KERN_OFFSET 0x4A00
#define TSS_KERN_OFFSET 0x4C00
#define TOS_KERN_OFFSET 0x8000
#define MSR_KERN_OFFSET 0x8000
#define __SELECTOR_TASK 0x0008
#define __SELECTOR_LDTR 0x0010
#define __SELECTOR_CODE 0x0004
#define __SELECTOR_DATA 0x000C
#define __SELECTOR_VRAM 0x0014
#define __SELECTOR_FLAT 0x001C

char modname[] = "linuxvmm";
int my_major = 88;
char cpu_oem[16];
int cpu_features;
void *kmem[N_ARENAS];
unsigned long msr0x480[11];
unsigned long cr0, cr4;
unsigned long msr_efer;

unsigned long vmxon_region;
unsigned long guest_region;
unsigned long pgdir_region;
unsigned long pgtbl_region;
unsigned long g_IDT_region;
unsigned long g_GDT_region;
unsigned long g_LDT_region;
unsigned long g_TSS_region;
unsigned long g_TOS_region;
unsigned long h_MSR_region;

long my_ioctl(struct file *filp, unsigned int ioctl, unsigned long arg)
{
	return 0;
}

struct file_operations my_fops = {
	unlocked_ioctl : my_ioctl,
	//mmap:	my_mmap,
};

int my_info(char *buf, char **start, off_t off, int count, int *eof, void *data)
{
	int i, len;
	
	len = 0;
	len += sprintf(buf+len, "\n\t%s\n\n", "VMX-Capability MSRs");
	for(i = 0; i < 11; i++)
	{
		len += sprintf(buf+len, "\tMSR0x%X=", 0x480 + i);
		len += sprintf(buf+len, "%016lX \n", msr0x480[i]);
	}
	
	len += sprintf(buf+len, "\n");
	len += sprintf(buf+len, "\n");

	len += sprintf(buf+len, "CR0=%016lX  ", cr0);
	len += sprintf(buf+len, "CR4=%016lX  ", cr4);
	len += sprintf(buf+len, "EFER=%016lX  ", msr_efer);
	len += sprintf(buf+len, "\n");

	len += sprintf(buf+len, "\n\t\t\t");
	len += sprintf(buf+len, "vmxon_region=%016lX \n", vmxon_region);
	len += sprintf(buf+len, "\n");

	return  len;	
}

void set_CR4_vmxe(void *dummy)
{
	asm(    " mov %%cr4, %%rax  \n"\
			" bts $13, %%rax    \n"\
			" mov %%rax, %%cr4  " ::: "ax" );
}

void clear_CR4_vmxe(void *dummy)
{
	asm(    " mov %%cr4, %%rax  \n"\
			" btr $13, %%rax    \n"\
			" mov %%rax, %%cr4  " ::: "ax" );
}

static int __init test_vmm_init(void)
{
	int i, j;

	printk(KERN_ALERT "init the module\n");
	printk(KERN_ALERT "Installing \'%s\' module\n", modname);
	printk(KERN_ALERT "(major=%d)\n", my_major);

	// verify processor supports Intel Virtualization Technology
	asm(    " xor   %%eax, %%eax        \n"\
			" cpuid						\n"\
			" mov   %%ebx, cpu_oem+0    \n"\
			" mov   %%edx, cpu_oem+4    \n"\
			" mov   %%ecx, cpu_oem+8    \n"\
			::: "ax", "bx", "cx", "dx"  );
	printk(KERN_ALERT "processor is \'%s\' \n", cpu_oem);

	if(strncmp(cpu_oem, "GenuineIntel", 12) == 0)
	{
		asm(    " mov   $1, %%eax			\n"\
				" cpuid						\n"\
				" mov   %%ecx, cpu_features \n"\
				::: "ax", "bx", "cx", "dx"  );
	}

	if((cpu_features & (1<<5)) == 0)
	{
		printk(KERN_ALERT "Virtualization Technology is unsupported\n");
		return -ENODEV;
	}
	else
	{
		printk(KERN_ALERT "Virtualization Technology is supported\n");
	}

	// read contents of the VMX-Capability Model-Specific Registers
	asm(    " xor   %%rbx, %%rbx					\n"\
			" mov   %0, %%rcx						\n"\
			"nxcap:									\n"\
			" rdmsr									\n"\
			" mov   %%eax, msr0x480+0(, %%rbx, 8)   \n"\
			" mov   %%edx, msr0x480+4(, %%rbx, 8)   \n"\
			" inc   %%rcx							\n"\
			" inc   %%rbx							\n"\
			" cmp   $11, %%rbx						\n"\
			" jb    nxcap							\n"\
			:: "i" (MSR_VMX_CAPS) : "ax", "bx", "cx", "dx"  );

	// preserve the initial values in relevant system registers
	asm( " mov %%cr0, %%rax \n mov %%rax, cr0 " ::: "ax" );
	asm( " mov %%cr4, %%rax \n mov %%rax, cr4 " ::: "ax" );

	asm(    " mov   %0, %%ecx       \n"\
			" rdmsr             \n"\
			" mov   %%eax, msr_efer+0   \n"\
			" mov   %%edx, msr_efer+4   \n"\
			:: "i" (MSR_EFER) : "ax", "cx", "dx" );

	// allocate page-aligned blocks of non-pageable kernel memory
	for(i=0; i<N_ARENAS; i++)
	{
		kmem[i] = kmalloc(ARENA_LENGTH, GFP_KERNEL);
		if(kmem[i] == NULL)
		{
			for(j=0; j<i; j++) 
				kfree(kmem[j]);
			return -ENOMEM;
		}
		else
		{
			memset(kmem[i], 0x00, ARENA_LENGTH);
		}
	}

	// assign usages to allocated kernel memory areas
	vmxon_region = virt_to_phys( kmem[ 10 ] + 0x0000 );
	guest_region = virt_to_phys( kmem[ 10 ] + 0x1000 );
	pgdir_region = virt_to_phys( kmem[ 10 ] + PAGE_DIR_OFFSET );
	pgtbl_region = virt_to_phys( kmem[ 10 ] + PAGE_TBL_OFFSET );
	g_IDT_region = virt_to_phys( kmem[ 10 ] + IDT_KERN_OFFSET );
	g_GDT_region = virt_to_phys( kmem[ 10 ] + GDT_KERN_OFFSET );
	g_LDT_region = virt_to_phys( kmem[ 10 ] + LDT_KERN_OFFSET );
	g_TSS_region = virt_to_phys( kmem[ 10 ] + TSS_KERN_OFFSET );
	g_TOS_region = virt_to_phys( kmem[ 10 ] + TOS_KERN_OFFSET );
	h_MSR_region = virt_to_phys( kmem[ 10 ] + MSR_KERN_OFFSET );

	// enable virtual machine extensions (bit 13 in CR4)
	set_CR4_vmxe(NULL);
	smp_call_function(set_CR4_vmxe, NULL, 1);

	create_proc_read_entry(modname, 0, NULL, my_info, NULL );

	printk(KERN_ALERT "Hello World\n");

	return 0;
}

static void __exit test_vmm_exit(void)
{
	int i;

	smp_call_function( clear_CR4_vmxe, NULL, 1);
	clear_CR4_vmxe( NULL );

	//unregister_chrdev( my_major, modname );
	remove_proc_entry( modname, NULL );
	for (i = 0; i < N_ARENAS; i++) 
		kfree( kmem[ i ] );
	printk(KERN_ALERT "Removing \'%s\' module\n", modname );
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(test_vmm_init);
module_exit(test_vmm_exit);
