#include <linux/mm.h>

mem_map_t * mem_map;
unsigned long max_mapnr;
unsigned long num_physpages;
unsigned long num_mappedpages;
void * high_memory;