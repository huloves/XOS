#ifndef _LINUX_CONSOLE_H
#define _LINUX_CONSOLE_H

#include <asm/types.h>
#include <linux/kd.h>

struct vc_data;

/**
 * this is what the terminal answers to a ESC-Z or csi0c query.
 */
#define VT100ID "\033[?1;2c"
#define VT102ID "\033[?6c"

struct consw {
	const char *(*con_startup)(void);
	void	(*con_init)(struct vc_data *, int);
	void	(*con_deinit)(struct vc_data *);
	void	(*con_clear)(struct vc_data *, int, int, int, int);
	void	(*con_putc)(struct vc_data *, int, int, int);
	void	(*con_putcs)(struct vc_data *, const unsigned short *, int, int, int);
	void	(*con_cursor)(struct vc_data *, int);
	int	(*con_scroll)(struct vc_data *, int, int, int, int);
	void	(*con_bmove)(struct vc_data *, int, int, int, int, int, int);
	int	(*con_switch)(struct vc_data *);
	int	(*con_blank)(struct vc_data *, int, int);
	int	(*con_font_set)(struct vc_data *, struct console_font *, unsigned);
	int	(*con_font_get)(struct vc_data *, struct console_font *);
	int	(*con_font_default)(struct vc_data *, struct console_font *, char *);
	int	(*con_font_copy)(struct vc_data *, int);
	int     (*con_resize)(struct vc_data *, unsigned int, unsigned int,
			       unsigned int);
	int	(*con_set_palette)(struct vc_data *, unsigned char *);
	int	(*con_scrolldelta)(struct vc_data *, int);
	int	(*con_set_origin)(struct vc_data *);
	void	(*con_save_screen)(struct vc_data *);
	u8	(*con_build_attr)(struct vc_data *, u8, u8, u8, u8, u8, u8);
	void	(*con_invert_region)(struct vc_data *, u16 *, int);
	u16    *(*con_screen_pos)(struct vc_data *, int);
	unsigned long (*con_getxy)(struct vc_data *, unsigned long, int *, int *);
};

#endif /* _LIXNU_CONSOLE_H */
