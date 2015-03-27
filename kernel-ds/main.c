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

/* run dmesg to see the list */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kfifo.h>

struct ds_node 
{
	int val;
	struct list_head list;
};

LIST_HEAD(ds_ll_head);
void test_linkedlist(void)
{
	struct ds_node *n1;
	struct ds_node *n2;
	struct ds_node *n3;
	struct list_head *tmp;
	struct ds_node *tn;

	n1 = vmalloc(sizeof(struct ds_node));
	n2 = vmalloc(sizeof(struct ds_node));
	n3 = vmalloc(sizeof(struct ds_node));

	n1->val = 10;
	n2->val = 20;
	n3->val = 30;

	INIT_LIST_HEAD(&n1->list);
	INIT_LIST_HEAD(&n2->list);
	INIT_LIST_HEAD(&n3->list);

	list_add(&n1->list, &ds_ll_head);
	list_add(&n2->list, &ds_ll_head);
	list_add(&n3->list, &ds_ll_head);

	list_for_each(tmp, &ds_ll_head)
	{
		tn = list_entry(tmp, struct ds_node, list);
		printk(KERN_ALERT "data: %d\n", tn->val);
	}

	vfree(n1);
	vfree(n2);
	vfree(n3);
}

void test_queue(void)
{
	struct kfifo fifo;
	int ret, i, tmp;

	ret = kfifo_alloc(&fifo, PAGE_SIZE, GFP_KERNEL);
	if(ret)
	{
		printk(KERN_ALERT "kfifo creation failed!\n");
		return;
	}

	for(i=0; i<10; i++)
		kfifo_in(&fifo, &i, sizeof(int));

	kfifo_out_peek(&fifo, &tmp, sizeof(int));
	printk(KERN_ALERT "1st: %d\n", tmp);

	kfifo_out(&fifo, &tmp, sizeof(int));
	printk(KERN_ALERT "data: %d, %d\n", tmp, kfifo_avail(&fifo));
	kfifo_out(&fifo, &tmp, sizeof(int));
	printk(KERN_ALERT "data: %d, %d\n", tmp, kfifo_avail(&fifo));


	kfifo_free(&fifo);
}

static int __init kernel_ds_init(void)
{
	printk(KERN_ALERT "init the module\n");
	//test_linkedlist();
	test_queue();
	return 0;
}

static void __exit kernel_ds_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(kernel_ds_init);
module_exit(kernel_ds_exit);
