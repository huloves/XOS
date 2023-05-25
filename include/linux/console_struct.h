#ifndef _LINUX_CONSOLE_STRUCT_H
#define _LINUX_CONSOLE_STRUCT_H

#include <linux/console.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/wait.h>

#define NPAR 16

struct vc_data {
	unsigned short	vc_num;			/* Console number */
	unsigned int	vc_cols;		/* [#] 屏幕大小 */
	unsigned int	vc_rows;
	unsigned int	vc_size_row;		/* 每行字节数 */
	unsigned int	vc_scan_lines;		/* # of scan lines */
	unsigned long	vc_origin;		/* [!] Start of real screen */
	unsigned long	vc_scr_end;		/* [!] End of real screen */
	unsigned long	vc_visible_origin;	/* [!] Top of visible window */
	unsigned int	vc_top, vc_bottom;	/* Scrolling region */
	const struct consw *vc_sw;
	unsigned short	*vc_screenbuf;		/* In-memory character/attribute buffer */
	unsigned int	vc_screenbuf_size;
	unsigned char	vc_mode;		/* KD_TEXT, ... */
	/**
	 *  屏幕上字符的属性
	 */
	unsigned char	vc_attr;		/* Current attributes */
	unsigned char	vc_def_color;		/* Default colors */
	unsigned char	vc_color;		/* Foreground & background */
	unsigned char	vc_s_color;		/* Saved foreground & background */
	unsigned char	vc_ulcolor;		/* Color for underline mode */
	unsigned char   vc_itcolor;
	unsigned char	vc_halfcolor;		/* Color for half intensity mode */
	/**
	 * 光标
	 */
	unsigned int	vc_cursor_type;
	unsigned short	vc_complement_mask;	/* [#] Xor mask for mouse pointer */
	unsigned short	vc_s_complement_mask;	/* Saved mouse pointer mask */
	unsigned int	vc_x, vc_y;		/* Cursor position */
	unsigned int	vc_saved_x, vc_saved_y;
	unsigned long	vc_pos;
	/**
	 * 字体
	 */
	unsigned short	vc_hi_font_mask;	/* [#] Attribute set for upper 256 chars of font or 0 if not supported */
	struct console_font vc_font;		/* Current VC font set */
	unsigned short	vc_video_erase_char;	/* Background erase character */
	/**
	 * VT 终端数据
	 */
	unsigned int	vc_state;		/* Escape sequence parser state */
	unsigned int	vc_npar,vc_par[NPAR];	/* Parameters of current escape sequence */
	// struct tty_struct *vc_tty;		/* TTY we are attached to */
	/**
	 * data for manual vt switching
	 */
	struct vt_mode	vt_mode;
	// struct pid 	*vt_pid;
	int		vt_newvt;
	wait_queue_head_t paste_wait;
	/**
	 * 模式标志
	 */
	unsigned int	vc_charset	: 1;	/* Character set G0 / G1 */
	unsigned int	vc_s_charset	: 1;	/* Saved character set */
	unsigned int	vc_disp_ctrl	: 1;	/* Display chars < 32? */
	unsigned int	vc_toggle_meta	: 1;	/* Toggle high bit? */
	unsigned int	vc_decscnm	: 1;	/* Screen Mode */
	unsigned int	vc_decom	: 1;	/* Origin Mode */
	unsigned int	vc_decawm	: 1;	/* Autowrap Mode */
	unsigned int	vc_deccm	: 1;	/* Cursor Visible */
	unsigned int	vc_decim	: 1;	/* Insert Mode */
	unsigned int	vc_deccolm	: 1;	/* 80/132 Column Mode */
	/**
	 * 属性标志
	 */
	unsigned int	vc_intensity	: 2;	/* 0=half-bright, 1=normal, 2=bold */
	unsigned int    vc_italic:1;
	unsigned int	vc_underline	: 1;
	unsigned int	vc_blink	: 1;
	unsigned int	vc_reverse	: 1;
	unsigned int	vc_s_intensity	: 2;	/* saved rendition */
	unsigned int    vc_s_italic:1;
	unsigned int	vc_s_underline	: 1;
	unsigned int	vc_s_blink	: 1;
	unsigned int	vc_s_reverse	: 1;
	/* 杂项 */
	unsigned int	vc_ques		: 1;
	unsigned int	vc_need_wrap	: 1;
	unsigned int	vc_can_do_color	: 1;
	unsigned int	vc_report_mouse : 2;
	unsigned int	vc_kmalloced	: 1;
	unsigned char	vc_utf		: 1;	/* Unicode UTF-8 encoding */
	unsigned char	vc_utf_count;
		 int	vc_utf_char;
	unsigned int	vc_tab_stop[8];		/* Tab stops. 256 columns. */
	unsigned char   vc_palette[16*3];       /* Colour palette for VGA+ */
	unsigned short * vc_translate;
	unsigned char 	vc_G0_charset;
	unsigned char 	vc_G1_charset;
	unsigned char 	vc_saved_G0;
	unsigned char 	vc_saved_G1;
	unsigned int    vc_resize_user;         /* resize request from user */
	unsigned int	vc_bell_pitch;		/* Console bell pitch */
	unsigned int	vc_bell_duration;	/* Console bell duration */
	struct vc_data **vc_display_fg;		/* [!] Ptr to var holding fg console for this display */
	unsigned long	vc_uni_pagedir;
	unsigned long	*vc_uni_pagedir_loc;  /* [!] Location of uni_pagedir variable for this console */
	/* additional information is in vt_kern.h */
};

#endif /* _LINUX_CONSOLE_STRUCT_H */
