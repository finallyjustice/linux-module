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
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/ksm.h>

#include <asm/mmzone.h>
#include <asm-generic/memory_model.h>

void print_zone(void)
{
	struct zone *pz;
	int nz;

	printk(KERN_ALERT "node_start_pfn     : %lu\n", node_data[0]->node_start_pfn);
	printk(KERN_ALERT "node_present_pages : %lu\n", node_data[0]->node_present_pages);
	printk(KERN_ALERT "node_spanned_pages : %lu\n", node_data[0]->node_spanned_pages);

	for(nz=0; nz<node_data[0]->nr_zones; nz++)
	{
		pz = &(node_data[0]->node_zones[nz]);
		printk(KERN_ALERT "Zone %d (%s): start_pfn(%lu), spanned_pages(%lu), present_pages(%lu)\n", nz, pz->name, pz->zone_start_pfn, pz->spanned_pages, pz->present_pages);
	}

	return;
}

void print_page(void)
{
	struct page *p;
	unsigned long pfn;
	unsigned long c;

	unsigned long tot_valid = 0;
	unsigned long tot_page  = 0;
	unsigned long tot_used  = 0;

	unsigned long tot_dma   = 0;
	unsigned long tot_dma32 = 0;
	unsigned long tot_other = 0;

	unsigned long tot_anon  = 0;
	unsigned long tot_file  = 0;
	unsigned long tot_ksm   = 0;

	for(c=0; c<node_data[0]->node_spanned_pages+4; c++)
	{
		p = pfn_to_page(c);
		pfn = page_to_pfn(p);
		if(pfn == c)
			tot_valid++;
		if(page_count(p) > 0)
			tot_used++;
		tot_page++;

		if(page_zonenum(p) == 0)
			tot_dma++;
		else if(page_zonenum(p) == 1)
			tot_dma32++;
		else
			tot_other++;

		// page type
		if(PageKsm(p))
		{
			tot_ksm++;
		}
		else if(PageAnon(p))
		{
			tot_anon++;
		}
		else
		{
			tot_file++;
		}
	}

	printk(KERN_ALERT "Total : %lu\n", tot_page);
	printk(KERN_ALERT "Valid : %lu\n", tot_valid);
	printk(KERN_ALERT "Used  : %lu\n", tot_used);

	printk(KERN_ALERT "tot_dma   : %lu\n", tot_dma);
	printk(KERN_ALERT "tot_dma32 : %lu\n", tot_dma32);
	printk(KERN_ALERT "tot_other : %lu\n", tot_other);

	printk(KERN_ALERT "tot_anon : %lu\n", tot_anon);
	printk(KERN_ALERT "tot_file : %lu\n", tot_file);
	printk(KERN_ALERT "tot_ksm  : %lu\n", tot_ksm);
	
	return;
}

static int __init multi_file_init(void)
{
	printk(KERN_ALERT "init the module\n");
	print_zone();
	print_page();
	return 0;
}

static void __exit multi_file_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(multi_file_init);
module_exit(multi_file_exit);
