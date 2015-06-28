#ifndef __BACKPORT_MM_H
#define __BACKPORT_MM_H
#include_next <linux/mm.h>

#ifndef VM_NODUMP
/*
 * defined here to allow things to compile but technically
 * using this for memory regions will yield in a no-op on newer
 * kernels but on older kernels (v3.3 and older) this bit was used
 * for VM_ALWAYSDUMP. The goal was to remove this bit moving forward
 * and since we can't skip the core dump on old kernels we just make
 * this bit name now a no-op.
 *
 * For details see commits: 909af7 accb61fe cdaaa7003
 */
#define VM_NODUMP      0x0
#endif

#ifndef VM_DONTDUMP
#define VM_DONTDUMP    VM_NODUMP
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0))
#define vm_iomap_memory LINUX_BACKPORT(vm_iomap_memory)
int vm_iomap_memory(struct vm_area_struct *vma, phys_addr_t start, unsigned long len);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,15,0)
#define kvfree LINUX_BACKPORT(kvfree)
void kvfree(const void *addr);
#endif /* < 3.15 */

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,20,0))
#define get_user_pages_locked LINUX_BACKPORT(get_user_pages_locked)
long get_user_pages_locked(struct task_struct *tsk, struct mm_struct *mm,
		    unsigned long start, unsigned long nr_pages,
		    int write, int force, struct page **pages,
		    int *locked);
#define __get_user_pages_unlocked LINUX_BACKPORT(__get_user_pages_unlocked)
long __get_user_pages_unlocked(struct task_struct *tsk, struct mm_struct *mm,
			       unsigned long start, unsigned long nr_pages,
			       int write, int force, struct page **pages,
			       unsigned int gup_flags);
#define get_user_pages_unlocked LINUX_BACKPORT(get_user_pages_unlocked)
long get_user_pages_unlocked(struct task_struct *tsk, struct mm_struct *mm,
		    unsigned long start, unsigned long nr_pages,
		    int write, int force, struct page **pages);
#endif

#ifndef FOLL_TRIED
#define FOLL_TRIED	0x800	/* a retry, previous pass started an IO */
#endif

#endif /* __BACKPORT_MM_H */
