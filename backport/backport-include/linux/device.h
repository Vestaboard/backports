#ifndef __BACKPORT_DEVICE_H
#define __BACKPORT_DEVICE_H
#include_next <linux/device.h>

#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(4, 1, 0)
#define dev_of_node LINUX_BACKPORT(dev_of_node)
static inline struct device_node *dev_of_node(struct device *dev)
{
#ifndef CONFIG_OF
	return NULL;
#else
	return dev->of_node;
#endif
}
#endif

#endif /* __BACKPORT_DEVICE_H */
