#ifndef _ASM_X86_SETUP_H
#define _ASM_X86_SETUP_H

#ifdef CONFIG_X86_VISWS
extern void visws_early_detect(void);
extern int is_visws_box(void);
#else
static inline void visws_early_detect(void) { }
static inline int is_visws_box(void) { return 0; }
#endif

#endif /* _ASM_X86_SETUP_H */
