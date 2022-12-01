#include <asm-i386/desc.h>

void load_LDT (struct mm_struct *mm)
{
	int cpu = 0;
	void *segments = mm->context.segments;
	int count = LDT_ENTRIES;

	if (!segments) {
		segments = &default_ldt[0];
		count = 5;
	}
		
	set_ldt_desc(cpu, segments, count);
	__load_LDT(cpu);
}
