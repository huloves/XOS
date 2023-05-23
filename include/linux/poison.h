#ifndef _LINUX_POISON_H
#define _LINUX_POISON_H

/********** include/linux/list.h **********/
/*
 * 这些是非空指针，在正常情况下会导致页面错误。
 * used to verify that nobody uses non-initialized list entries.
 */
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

#endif /* _LINUX_POISON_H */
