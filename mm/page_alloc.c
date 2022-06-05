#include <linux/mmzone.h>

pg_data_t *pgdat_list;

// void free_area_init(unsigned long *zones_size)
// {
// 	free_area_init_core(0, &contig_page_data, &mem_map, zones_size, 0, 0, 0);
// }