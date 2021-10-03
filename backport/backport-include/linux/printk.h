#ifndef _COMPAT_LINUX_PRINTK_H
#define _COMPAT_LINUX_PRINTK_H 1

#include <linux/version.h>
#include_next <linux/printk.h>

/* replace hex_dump_to_buffer() with a version which returns the length */
#if LINUX_VERSION_IS_LESS(4,0,0)
#define hex_dump_to_buffer LINUX_BACKPORT(hex_dump_to_buffer)
extern int hex_dump_to_buffer(const void *buf, size_t len, int rowsize,
			      int groupsize, char *linebuf, size_t linebuflen,
			      bool ascii);
#endif

#endif	/* _COMPAT_LINUX_PRINTK_H */
