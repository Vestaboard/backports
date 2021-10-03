#ifndef __BACKPORT_LINUX_TTY_H
#define __BACKPORT_LINUX_TTY_H
#include_next <linux/tty.h>

#if LINUX_VERSION_IS_LESS(4,1,0) && \
    LINUX_VERSION_IS_GEQ(4,0,0)
extern int tty_set_termios(struct tty_struct *tty, struct ktermios *kt);
#endif /* LINUX_VERSION_IS_LESS(4,1,0) */

#ifndef N_NCI
#define N_NCI		25	/* NFC NCI UART */
#endif

#endif /* __BACKPORT_LINUX_TTY_H */
