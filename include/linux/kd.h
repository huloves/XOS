#ifndef _LINUX_KD_H
#define _LINUX_KD_H

struct console_font {
	unsigned int width, height;	/* 字体大小 */
	unsigned int charcount;
	unsigned char *data;		/* 高度固定为 32 的字体数据 */
};

#endif /* _LINUX_KD_H */
