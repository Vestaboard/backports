#ifndef __BACKPORT_LINUX_OF_PLATFORM_H
#define __BACKPORT_LINUX_OF_PLATFORM_H
#include_next <linux/of_platform.h>
#include <linux/version.h>

#if LINUX_VERSION_IS_LESS(4,3,0) && !defined(CONFIG_OF_DEVICE)
struct of_dev_auxdata;
#define of_platform_default_populate \
	LINUX_BACKPORT(of_platform_default_populate)
static inline int
of_platform_default_populate(struct device_node *root,
			     const struct of_dev_auxdata *lookup,
			     struct device *parent)
{
	return -ENODEV;
}
#endif /* LINUX_VERSION_IS_LESS(4,3,0) */

#endif /* __BACKPORT_LINUX_OF_PLATFORM_H */
