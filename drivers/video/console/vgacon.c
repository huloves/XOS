#include <linux/console.h>
#include <linux/screen_info.h>

static const char *vgacon_startup(void)
{
	const char *distplay_desc = NULL;
	u16 saved1, saved2;
	volatile u16 *p;
}

const struct consw vga_con = {
};