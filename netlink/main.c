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
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>

#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <net/sock.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

#define NETLINK_USER 31

struct sock *netlink_test_nlsk;

void netlink_test_recv_msg(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	int rlen;
	void *data;

	while(skb->len > NLMSG_SPACE(0))
	{
		nlh = nlmsg_hdr(skb);
		if(nlh->nlmsg_len<sizeof(*nlh) || skb->len<nlh->nlmsg_len)
			return;

		rlen = NLMSG_ALIGN(nlh->nlmsg_len);
		if(rlen > skb->len)
			rlen = skb->len;

		data = NLMSG_DATA(nlh);

		printk(KERN_ALERT "link: %s\n", (char *)data);
		netlink_ack(skb, nlh, 0);
		skb_pull(skb, rlen);
	}
}

static int __init netlink_test_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.input = netlink_test_recv_msg,
	};
	
	netlink_test_nlsk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
	if(!netlink_test_nlsk)
	{
		printk(KERN_ALERT "create NETLINK_USER failed!\n");
		return -ENOMEM;
	}
	else
	{
		netlink_test_nlsk->sk_sndtimeo = MAX_SCHEDULE_TIMEOUT;
	}
	
	printk(KERN_ALERT "init the module\n");

	return 0;
}

static void __exit netlink_test_exit(void)
{
	netlink_kernel_release(netlink_test_nlsk);
	printk(KERN_ALERT "exit the module\n");
}

module_init(netlink_test_init);
module_exit(netlink_test_exit);
