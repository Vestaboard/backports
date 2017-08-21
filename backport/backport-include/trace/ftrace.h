#undef __print_array
#define __print_array(array, count, el_size)				\
	({								\
		BUILD_BUG_ON(el_size != 1 && el_size != 2 &&		\
			     el_size != 4 && el_size != 8);		\
		ftrace_print_array_seq(p, array, count, el_size);	\
	})

#include_next <trace/ftrace.h>
