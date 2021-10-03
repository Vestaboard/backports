#ifndef __BACKPORT_LINUX_NET_H
#define __BACKPORT_LINUX_NET_H
#include_next <linux/net.h>
#include <linux/static_key.h>


#if LINUX_VERSION_IS_LESS(4,2,0)
#define sock_create_kern(net, family, type, proto, res) \
	__sock_create(net, family, type, proto, res, 1)
#endif

#ifndef SOCKWQ_ASYNC_NOSPACE
#define SOCKWQ_ASYNC_NOSPACE   SOCK_ASYNC_NOSPACE
#endif
#ifndef SOCKWQ_ASYNC_WAITDATA
#define SOCKWQ_ASYNC_WAITDATA   SOCK_ASYNC_WAITDATA
#endif

#endif /* __BACKPORT_LINUX_NET_H */
