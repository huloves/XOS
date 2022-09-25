#include <linux/sched.h>
#include <asm-i386/processor.h>
#include <linux/cache.h>
#include <asm-i386/segment.h>
#include <linux/threads.h>

/*
 * Initial task structure.
 *
 * We need to make sure that this is 8192-byte aligned due to the
 * way process stacks are handled. This is done by having a special
 * "init_task" linker map entry..
 */
union task_union init_task_union 
	__attribute__((__section__(".data.init_task"))) =
		{ INIT_TASK(init_task_union.task) };

/*
 * per-CPU TSS segments. Threads are completely 'soft' on Linux,
 * no more per-task TSS's. The TSS size is kept cacheline-aligned
 * so they are allowed to end up in the .data.cacheline_aligned
 * section. Since TSS's are completely CPU-local, we want them
 * on exact cacheline boundaries, to eliminate cacheline ping-pong.
 */ 
struct tss_struct init_tss[NR_CPUS] __cacheline_aligned = { [0 ... NR_CPUS-1] = INIT_TSS };
