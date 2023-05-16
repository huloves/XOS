#ifndef _I386_STRING_H_
#define _I386_STRING_H_

#include <linux/string.h>

/*
 * struct_cpy(x,y), copy structure *x into (matching structure) *y.
 *
 * We get link-time errors if the structure sizes do not match.
 * There is no runtime overhead, it's all optimized away at
 * compile time.
 */
extern void __struct_cpy_bug (void);

#define struct_cpy(x,y) 			\
({						\
	if (sizeof(*(x)) != sizeof(*(y))) 	\
		__struct_cpy_bug;		\
	memcpy(x, y, sizeof(*(x)));		\
})

#endif /* _I386_STRING_H */
