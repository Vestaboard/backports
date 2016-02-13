#ifndef __BACKPORT_LINUX_PHY_H
#define __BACKPORT_LINUX_PHY_H
#include_next <linux/phy.h>
#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,9,0))
#define phy_connect(dev, bus_id, handler, interface) \
	phy_connect(dev, bus_id, handler, 0, interface)
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(4,5,0))
#define phydev_name LINUX_BACKPORT(phydev_name)
static inline const char *phydev_name(const struct phy_device *phydev)
{
	return dev_name(&phydev->dev);
}

#define mdiobus_is_registered_device LINUX_BACKPORT(mdiobus_is_registered_device)
static inline bool mdiobus_is_registered_device(struct mii_bus *bus, int addr)
{
	return bus->phy_map[addr];
}
#endif /* < 4.5 */

#endif /* __BACKPORT_LINUX_PHY_H */
