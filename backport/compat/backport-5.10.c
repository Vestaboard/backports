// SPDX-License-Identifier: GPL-2.0

#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>

int netif_rx_any_context(struct sk_buff *skb)
{
	/*
	 * If invoked from contexts which do not invoke bottom half
	 * processing either at return from interrupt or when softrqs are
	 * reenabled, use netif_rx_ni() which invokes bottomhalf processing
	 * directly.
	 */
	if (in_interrupt())
		return netif_rx(skb);
	else
		return netif_rx_ni(skb);
}
EXPORT_SYMBOL(netif_rx_any_context);
