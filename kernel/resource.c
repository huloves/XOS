#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <asm-i386/types.h>
#include <asm-i386/error.h>
#include <asm-i386/io.h>

struct resource ioport_resource = { "PCI IO", 0x0000, IO_SPACE_LIMIT, IORESOURCE_IO };
struct resource iomem_resource = { "PCI mem", 0x00000000, 0xffffffff, IORESOURCE_MEM };

static rwlock_t resource_lock = RW_LOCK_UNLOCKED;

/* Return the conflict entry if you can't request it */
static struct resource * __request_resource(struct resource *root, struct resource *new)
{
	unsigned long start = new->start;
	unsigned long end = new->end;
	struct resource *tmp, **p;

	if (end < start)
		return root;
	if (start < root->start)
		return root;
	if (end > root->end)
		return root;
	p = &root->child;
	for (;;) {
		tmp = *p;
		if (!tmp || tmp->start > end) {
			new->sibling = tmp;
			*p = new;
			new->parent = root;
			return NULL;
		}
		p = &tmp->sibling;
		if (tmp->end < start)
			continue;
		return tmp;
	}
}

/**
 * request_resource - 把一个给定范围分配给一个 I/O 设备
 * @root: 资源树的根节点
 * @new:  要插入的新资源数据结构的地址
 * **/
int request_resource(struct resource *root, struct resource *new)
{
	struct resource *conflict;
	write_lock(&resource_lock);
	conflict = __request_resource(root, new);
    write_unlock(&resource_lock);
    return conflict ? -EBUSY : 0;
}
