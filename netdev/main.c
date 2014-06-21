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
#include <linux/if.h>
#include <linux/timer.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/if_arp.h>

// struct net_device - include/linux/netdevice.h
// check net/core/net_namespace.c
// struct net - include/linux/net_namespace.h

// print information for each networking device
void get_dev_info(void)
{
	struct net_device *nd;
	struct in_device *ind;
	struct in_ifaddr *ina;
	struct net_device_stats *stats;

	list_for_each_entry(nd, &(init_net.dev_base_head), dev_list)
	{
		printk(KERN_ALERT "Name: %s\n", nd->name);
		if(nd->type == ARPHRD_ETHER)
			printk(KERN_ALERT "  Type: Ethernet\n");	
		else if(nd->type == ARPHRD_LOOPBACK)
			printk(KERN_ALERT "  Type: Local Loopback\n");
	
		printk(KERN_ALERT "  HWarrd: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\n", 
				nd->dev_addr[0], nd->dev_addr[1],
				nd->dev_addr[2], nd->dev_addr[3],
				nd->dev_addr[4], nd->dev_addr[5]);

		// net_device->ip_ptr
		ind = __in_dev_get_rcu(nd);
		if(ind)
		{
			ina = (struct in_ifaddr *)ind->ifa_list;
			if(ina)
			{
				printk(KERN_ALERT "  IP Address: 0x%08x\n", (unsigned int)ina->ifa_address);
				printk(KERN_ALERT "  Broadcast : 0x%08x\n", (unsigned int)ina->ifa_broadcast);
				printk(KERN_ALERT "  IP Mask   : 0x%08x\n", (unsigned int)ina->ifa_mask);
			}
		}

		if(nd->flags & IFF_UP) printk(KERN_ALERT "  Status: UP\n");	
		if(nd->flags & IFF_BROADCAST) printk(KERN_ALERT "  Status: BROADCAST\n");	
		if(nd->flags & IFF_RUNNING) printk(KERN_ALERT "  Status: RUNNING\n");	
		if(nd->flags & IFF_LOOPBACK) printk(KERN_ALERT "  Status: LOOPBACK\n");	
		if(nd->flags & IFF_MULTICAST) printk(KERN_ALERT "  Status: MULTICAST\n");

		// MTU info
		printk(KERN_ALERT "  MTU: %d\n", nd->mtu);

		stats = &nd->stats;
		// RX Packets
		printk(KERN_ALERT "  RX Packets:%lu errors:%lu dropped:%lu overruns:%lu frame:%lu\n",
				stats->rx_packets, stats->rx_errors, stats->rx_dropped,
				stats->rx_over_errors, stats->rx_frame_errors);
		// TX Packets
		printk(KERN_ALERT "  TX Packets:%lu errors:%lu dropped:%lu overruns:%lu carrier:%lu\n",
				stats->tx_packets, stats->tx_errors, stats->tx_dropped,
				stats->tx_aborted_errors, stats->tx_carrier_errors);

		printk(KERN_ALERT "  RX Bytes:%lu TX Bytes:%lu\n", stats->rx_bytes, stats->tx_bytes);
	}
}

static int __init netdev_init(void)
{
	printk(KERN_ALERT "init the module\n");
	get_dev_info();
	return 0;
}

static void __exit netdev_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

MODULE_LICENSE("GPL");
module_init(netdev_init);
module_exit(netdev_exit);
